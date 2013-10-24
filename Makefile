CC=gcc
DEBUG=-g

.c.o:
	$(CC) -c $(DEBUG) $<

entropypic : entropypic.o
	$(CC) -lbmp -o $@ $<

clean :
	rm -f core* entropypic.o

clobber : clean
	rm -f entropypic
