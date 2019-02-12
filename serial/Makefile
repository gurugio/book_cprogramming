# common for all
UNITTEST = serial_interface.c serial_main.c
# add info
UNITTEST_CASES = serialinfo_id.c serialinfo_product.c


all: $(LIB_SOURCES:.c=.o) $(UNITTEST:.c=.o) $(UNITTEST_CASES:.c=.o)
	gcc -g -o run $^

%.o : %.c
	gcc -g -Wall -c $<

clean:
	rm -f run *.o
