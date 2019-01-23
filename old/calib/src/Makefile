
include ../calib.mk
include ../config.mk


all: $(CALIB_NAME)

#
# rule to build library
#
$(eval $(call link_template,$(CALIB_NAME),$(CALIB_OBJS)))

#
# create rules to build & execute unit-test?? 유닛테스트는 아닌거 같은데 
#
$(foreach src,$(CALIB_SRCS),$(eval $(call compile_template,$(src))))


#
# build example program
#
$(eval $(call exec_template,$(CALIB_EXAMPLE_NAME)))


clean:
	rm -f *.o *.a
