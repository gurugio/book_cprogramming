
include ../calib.mk
include $(ROOT_DIR)/config.mk

all: $(CALIB_OBJS)

%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) -o $@ $<

clean:
	rm $(CALIB_OBJS)
