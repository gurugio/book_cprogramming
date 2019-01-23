

all:
	gcc -Wall -o gen serial_core.c serial_gen.c desc_greentea.c -DVERBOSE_MODE
	gcc -Wall -o veri serial_core.c serial_verify.c desc_greentea.c -DVERBOSE_MODE
	gcc -Wall -o view serial_core.c serial_view.c desc_greentea.c -DVERBOSE_MODE

