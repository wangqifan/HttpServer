test : test.o lib/log.o lib/acceptor.o 
	cc -o test test.o lib/log.o lib/acceptor.o 

test.o: test.c
	cc -c test.c

lib/log.o: lib/log.c lib/log.h lib/common.h
	cc -o lib/log.o -c lib/log.c

lib/acceptor.o:
	cc -o lib/acceptor.o -c lib/acceptor.c
lib/utils.o:
	cc -o lib/utils.o -c lib/utils.c

clean:
	rm test test.o lib/log.o lib/acceptor.o lib/common.o lib/utils.o