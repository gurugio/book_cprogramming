
include ../config.mk

CFLAGS=-Wall -I../include
LFLAGS=../lib/libca.a

all: test_hello test_sys_info

test_hello: test_hello.c ../lib/libca.a
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

test_sys_info: test_sys_info.c ../lib/libca.a
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

clean:
	rm -rf test_hello test_sys_info
