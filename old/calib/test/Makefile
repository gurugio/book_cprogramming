
include ../calib.mk
include ../config.mk


UNITTEST_TARGETS = atomic_list_unittest \
                   btrace_glib          \
                   btrace_builtin       \
                   test_caos_types

THRSAFE_TARGETS = thrsafe_example \
                  atomic_list_insert \
                  atomic_list_delete \
                  atomic_list_delete_first \
                  atomic_list_all
#                  calib_errno

test_caos_types_NAME = test_caos_types
test_caos_types_SRCS = test_caos_types.c
test_caos_types_LIBS = $(CALIB_LIB)

atomic_list_unittest_NAME = atomic_list_unittest
atomic_list_unittest_SRCS = atomic_list_unittest.c
atomic_list_unittest_LIBS = $(CALIB_LIB)

btrace_glib_NAME = btrace_glib
btrace_glib_SRCS = btrace_glib.c
btrace_glib_LIBS = $(CALIB_LIB) 

btrace_builtin_NAME = btrace_builtin
btrace_builtin_SRCS = btrace_builtin.c
btrace_builtin_LIBS = $(CALIB_LIB)

thrsafe_example_NAME = thrsafe_example
thrsafe_example_SRCS = thrsafe_example.c
thrsafe_example_LIBS = $(CALIB_LIB) $(UTIL_LIB) pthread

atomic_list_insert_NAME = atomic_list_insert
atomic_list_insert_SRCS = atomic_list_insert.c
atomic_list_insert_LIBS = $(CALIB_LIB) $(UTIL_LIB) pthread

atomic_list_delete_NAME = atomic_delete_insert
atomic_list_delete_SRCS = atomic_list_delete.c
atomic_list_delete_LIBS = $(CALIB_LIB) $(UTIL_LIB) pthread

atomic_list_delete_first_NAME = atomic_list_delete_first
atomic_list_delete_first_SRCS = atomic_list_delete_first.c
atomic_list_delete_first_LIBS = $(CALIB_LIB) $(UTIL_LIB) pthread

atomic_list_all_NAME = atomic_list_all
atomic_list_all_SRCS = atomic_list_all.c
atomic_list_all_LIBS = $(CALIB_LIB) $(UTIL_LIB) pthread

# calib_errno_NAME = calib_errno
# calib_errno_SRCS = calib_errno.c
# calib_errno_SRCS = $(CALIB_LIB) $(UTIL_LIB) pthread




.PHONY: all
all: $(UNITTEST_TARGETS) $(THRSAFE_TARGETS) before_unittest run_unittest after_unittest


#
# create rules to build & execute unit-test
#
$(foreach target,$(UNITTEST_TARGETS),$(eval $(call unittest_template,$(target))))

#
# create rules to build & execute thread-safety test
#
$(foreach target,$(THRSAFE_TARGETS),$(eval $(call unittest_template,$(target))))

before_unittest:
	@echo ""
	@echo "Unittest Begins"

after_unittest:
	@echo "Unittest Ends"
	@echo ""

clean:
	rm -f $(UNITTEST_TARGETS) $(THRSAFE_TARGETS) *.unittest

