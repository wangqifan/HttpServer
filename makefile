test : test.o lib/log.o lib/acceptor.o lib/event_loop.o lib/utils.o lib/channel_map.o lib/event_dispatcher.o
	cc -o test test.o lib/log.o  lib/acceptor.o lib/utils.o lib/event_loop.o lib/channel_map.o lib/event_dispatcher.o

test.o: test.c
	cc -c test.c

lib/log.o:
	cc -o lib/log.o -c lib/log.c

lib/acceptor.o:
	cc -o lib/acceptor.o -c lib/acceptor.c
lib/utils.o:
	cc -o lib/utils.o -c lib/utils.c

lib/event_loop.o:
	cc -o lib/event_loop.o -c lib/event_loop.c

lib/channel_map.o:
	cc -o lib/channel_map.o -c lib/channel_map.c

lib/event_dispatcher.o:
	cc -o lib/event_dispatcher.o -c lib/event_dispatcher.c

clean:
	rm test test.o lib/log.o lib/acceptor.o lib/utils.o lib/event_loop.o lib/channel_map.o lib/event_dispatcher.o