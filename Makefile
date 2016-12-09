all: 
	gcc -o myshell myshell.c cd.c mypwd.c myls.c mycat.c myhead.c mytail.c mycmp.c mycp.c mymv.c mymkdir.c myrmdir.c myrm.c

object:
	gcc -c myshell.c

compile:
	gcc -o myshell myshell.o

clean:
	rm -rf myshell.o myshell
