# common for all
UNITTEST = unittest.c unittest_main.c

# add target source
LIB_SOURCES = cstring.c
# add unittest case
UNITTEST_CASES = unittest_dummy.c unittest_cstring.c


all: $(LIB_SOURCES:.c=.o) $(UNITTEST:.c=.o) $(UNITTEST_CASES:.c=.o)
	gcc -o run $^

%.o : %.c
	gcc -Wall -c $<

clean:
	rm -f run *.o
