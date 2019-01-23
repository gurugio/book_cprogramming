#
# templates for all build types
#

# rules
# OBJ_TARGETS: label that defines source files
# LIB_TARGETS: label that defines library files
# UNITTEST_TARGETS: label that defines unittest programs



#
# 0. naming convention
#
OBJ_SUF=.o
LIB_PRE=lib
LIB_SUF=.a



#
# 1. define howto build
#


# obj_template: make obj files
# A label has object file names that will be linked together
# so they must be built together
# $($(1)_SRCS) is source file names
# (patsubst %,...) make source file names into dir/obj-names format
# eg) a.c -> build/src/a.o
# $(1) : label name
define obj_template
.PHONY: build_objs
build_objs:$(BUILD_DIR) $(patsubst %,$(BUILD_DIR)/%$(OBJ_SUF),$(basename $($(1)_SRCS)))
endef



# lib_template: link objects to generate static-library file
# $(1) : label name
define lib_template
.PHONY: build_libs
build_libs: $(BUILD_DIR) $(BUILD_DIR)/$(LIB_PRE)$($(1)_NAME)$(LIB_SUF)
$(BUILD_DIR)/$(LIB_PRE)$($(1)_NAME)$(LIB_SUF): $(patsubst %,$(BUILD_DIR_SRC)/%,$($(1)_OBJS))
	$(AR) $(AR_FLAGS) $$@ $(patsubst %,$(BUILD_DIR_SRC)/%,$($(1)_OBJS))
endef


# build unittest and run it
# $(1): target label
# $(2): test file name
define unittest_template
.PHONY: run_unittest
run_unittest: $(BUILD_DIR) $(BUILD_DIR)/$(1).unittest
$(BUILD_DIR)/$(1).unittest: $(2) # new execute-file -> run again
	@echo ""
	@echo "testing [$(1)]"
	@$(2) && touch $$@
	@echo ""
$(2): $(patsubst %,$(BUILD_DIR)/%$(OBJ_SUF),$(basename $($(1)_SRCS))) $(BUILD_DIR_LIB)/$(LIB_PRE)$($(1)_LIBS)$(LIB_SUF)
	$(CC) $(LD_FLAGS) -o $$@ $$< $(BUILD_DIR_LIB)/$(LIB_PRE)$($(1)_LIBS)$(LIB_SUF)
endef



#
# 2. expand template to define build rules
#
# build object
$(foreach target, $(OBJ_TARGETS), $(eval $(call obj_template,$(target))))

# build library
$(foreach target, $(LIB_TARGETS), $(eval $(call lib_template,$(target))))

# build unittest program
$(foreach target, $(UNITTEST_TARGETS), $(eval $(call unittest_template,$(target),$(BUILD_DIR)/$($(target)_NAME))))



#
# 3. implicit rules for compile sources
#


# .c files

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) -o $@ $<

# rules for other language files can be defined here...


clean:
	rm -rf $(BUILD_DIR)



