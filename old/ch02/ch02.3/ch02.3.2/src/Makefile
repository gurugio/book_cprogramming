include ../config.mk

SRCS = sys_info.c hello.c
OBJS = $(SRCS:.c=.o)
TARGET = libca.a


all: $(TARGET)

$(TARGET): $(OBJS)
	ar sruv $@ $^
	cp $@ ../lib

%.o:%.c
	$(CC) -c -Wall -I../include -o $@ $<

clean:
	rm $(OBJS) $(TARGET)
