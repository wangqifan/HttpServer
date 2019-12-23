test : test.o lib/log.o
	cc -o test test.o lib/log.o

test.o: test.c
	cc -c test.c

lib/log.o: lib/log.c lib/log.h lib/common.h
	cc -o lib/log.o -c lib/log.c

clean:
	rm test test.o lib/log.o