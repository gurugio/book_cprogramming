
all:
	gcc -o gen serial_gen.c
	gcc -o view serial_view.c
	../../../../join_text/join_text.scm book.txt serial_gen.c serial_view.c > final_book.txt
