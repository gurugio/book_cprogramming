
#
# build option
#

# BUILD_MODE setup
# eg) make build_mode=debug
AVAILABLE_BUILD_MODE = debug release kernel
DEFAULT_BUILD_MODE = debug

ifneq ($(DEFAULT_BUILD_MODE),)
ifeq ($(origin build_mode), undefined)
BUILD_MODE = $(DEFAULT_BUILD_MODE)
else
BUILD_MODE = $(filter $(build_mode), $(AVAILABLE_BUILD_MODE))
ifneq ($(BUILD_MODE), $(build_mode))
$(error Unknown build mode: $(build_mode))
endif
endif
endif




# UNITTEST_MODE setup
# eg) make unittest_mode=y
AVAILABLE_UNITTEST_MODE = y n
DEFAULT_UNITTEST_MODE = y

ifneq ($(DEFAULT_UNITTEST_MODE),)
ifeq ($(origin unittest_mode), undefined)
UNITTEST_MODE = $(DEFAULT_UNITTEST_MODE)
else
UNITTEST_MODE = $(filter $(unittest_mode), $(AVAILABLE_UNITTEST_MODE))
ifneq ($(UNITTEST_MODE), $(unittest_mode))
$(error Unknown unittest mode: $(unittest_mode))
endif
endif
endif


#
# Silent mode
#
ifneq ($(findstring s,$(filter-out -%,$(MAKEFLAGS))),)
SILENT_MODE = 1
ifneq ($(MAKELEVEL),0)
ifeq ($(findstring w,$(filter-out -%,$(MAKEFLAGS))),)
MAKEFLAGS := w$(MAKEFLAGS)
endif
endif
endif


#
# directory setting
#

ROOT_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
LIB_DIR = $(ROOT_DIR)/lib
SRC_DIR = $(ROOT_DIR)/src
TEST_DIR = $(ROOT_DIR)/test


#
# library files used by test cases
#

# Every module must define SRCS & OBJS & NAME
# SRCS: source file names
# OBJS: object file names
# NAME: target file name (only one file)

# static-library calib
CALIB_SRCS = atomic_list.c btrace.c
CALIB_OBJS = $(CALIB_SRCS:.c=.o)
CALIB_NAME = libcalib.a

# example program 
CALIB_EXAMPLE_SRCS = example.c
CALIB_EXAMPLE_OBJS = $(CALIB_EXAMPLE_SRCS:.c=.o)
CALIB_EXAMPLE_NAME = example

# util for thread safety test
UTIL_LIB_SRCS = thrsafe.c
UTIL_LIB_OBJS = $(UTIL_LIB_SRCS:.c=.o)
UTIL_LIB_NAME = libutil.a

CALIB_LIB = calib
UTIL_LIB = util


#
# compile option
#

INCLUDES += $(ROOT_DIR)/include

DEFINES += CALIB_CFG_CPU=$(CPU)
DEFINES += CALIB_CFG_CPU_$(CPU)
DEFINES += CALIB_CFG_BUILDMODE=$(BUILD_MODE)

ifeq ($(BUILD_MODE), debug)
 DEFINES += CALIB_CFG_BUILDMODE_DEBUG
else
 ifeq ($(BUILD_MODE), release)
  DEFINES += CALIB_CFG_BUILDMODE_RELEASE
 else
  ifeq ($(BUILD_MODE), kernel)
   DEFINES += CALIB_CFG_BUILDMODE_KERNEL
  endif
 endif
endif

CFLAGS += -Wextra -Wall -rdynamic -L$(LIB_DIR)

INCLUDE_OPT = $(addprefix -I,$(INCLUDES))
DEFINE_OPT = $(addprefix -D,$(DEFINES))


ifeq ($(BUILD_MODE), debug)
CFLAGS += -g
else
 ifeq ($(BUILD_MODE), release)
  CFLAGS += -O2
 else
  ifeq ($(BUILD_MODE), kernel)
   KERNEL_VIRTUAL_ADDR=0xFFFF800000100000  # BUGBUG: check caos64
   KERNEL_STACK_ADDR=0xFFFF800000200000

   CFLAGS += -Wall -Winline -fno-stack-protector -O2 -nostdinc -nostdlib -m64 -ffreestanding -nostdlib -mcmodel=large -DKERNEL_VIRTUAL_ADDR=$(KERNEL_VIRTUAL_ADDR) -DKERNEL_STACK_ADDR=$(KERNEL_STACK_ADDR) -c
  endif
 endif
endif


#
# templates
#

# link objects to generate static-library file
# $(1) : library name
# $(2) : object file names
define link_template
$(1): $(2)
ifneq ($(SILENT_MODE),)
	@echo "Linking [$(1)]"
endif
	ar rc $(1) $(2)
	cp $(1) $(LIB_DIR)
# if the target is moved by mv, the target will be build repeatly
# even though nothing is changed.
	@rm -f $(TEST_DIR)/*.unittest # run unittest again
endef


# compile source file to generate object file with the same name
# $(1) : source file name
define compile_template
$(1:.c=.o): $(1)
ifneq ($(SILENT_MODE),)
	@echo "Compiling [$(1)]"
endif
	gcc $(CFLAGS) $(INCLUDE_OPT) $(DEFINE_OPT) -c $(1)
endef


# build an excutable file
# $(1) : executable file name (prefix of variable definition)
define exec_template
# a double parenthesis needs to access variables
# $(1)_SRCS ==> somename_SRCS
# $($(1)_SRCS) ==> value of somename_SRCS = $(somename_SRCS)
$(1): $($(1)_SRCS) # source change -> exec again
ifneq ($(SILENT_MODE),)
	@echo "Making [$(1)]"
endif
	gcc $(CFLAGS) $(INCLUDE_OPT) $(DEFINE_OPT) -o $$@ $($(1)_SRCS) $(addprefix -l,$($(1)_LIBS))
endef


# build unittest and run it
# $(1) : executable file name
define unittest_template
$(eval $(call exec_template,$(1)))
$(1).unittest: $(1) # new execute-file -> run again
	@echo "testing [$(1)]"
	@$(TEST_DIR)/$(1) && touch $$@ # create an empty file when success
# If the XX.unittest is already created, it means that the test successed.
# So do not run unittest again
run_unittest: $(1).unittest
endef

