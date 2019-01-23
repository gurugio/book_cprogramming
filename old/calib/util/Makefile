

include ../calib.mk
include ../config.mk


.PHONY: all
all: $(UTIL_LIB_NAME)

#
# rule to build library
#
$(eval $(call link_template,$(UTIL_LIB_NAME),$(UTIL_LIB_OBJS)))

#
# create rules to build & execute unit-test
#
$(foreach src,$(UTIL_LIB_SRCS),$(eval $(call compile_template,$(src))))

clean:
	rm -f *.a *.o
