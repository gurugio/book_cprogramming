
include ../calib.mk
include $(ROOT_DIR)/config.mk

all: $(CALIB_NAME)

$(CALIB_NAME): $(addprefix $(SRC_DIR)/,$(CALIB_OBJS))
	ar sruv $@ $^


clean:
	rm $(CALIB_NAME)
