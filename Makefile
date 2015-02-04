all:
	gcc -o gentable gentable.c aes.c -lm -I.
	gcc -o crack crack.c aes.c -lm
