test : test.o lib/log.o lib/acceptor.o lib/event_loop.o lib/utils.o lib/channel_map.o lib/event_dispatcher.o lib/buffer.o lib/tcp_connection.o lib/tcp_server.o lib/channel.o  lib/thread_pool.o  lib/event_loop_thread.o lib/http_request.o lib/http_response.o lib/http_server.o lib/hashmap.o 
	cc -o test test.o lib/log.o  lib/acceptor.o lib/utils.o lib/event_loop.o lib/channel_map.o lib/event_dispatcher.o lib/buffer.o lib/tcp_connection.o lib/tcp_server.o lib/channel.o  lib/thread_pool.o lib/event_loop_thread.o lib/http_request.o lib/http_response.o lib/http_server.o lib/hashmap.o  -lpthread

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

lib/channel.o:
	cc -o lib/channel.o -c lib/channel.c

lib/tcp_connection.o:
	cc -o lib/tcp_connection.o -c lib/tcp_connection.c

lib/tcp_server.o:
	cc -o lib/tcp_server.o -c lib/tcp_server.c

lib/buffer.o:
	cc -o lib/buffer.o -c lib/buffer.c

lib/event_loop_thread.o:
	cc -o lib/event_loop_thread.o -c lib/event_loop_thread.c -lpthread

lib/thread_pool.o:
	cc -o lib/thread_pool.o -c lib/thread_pool.c

lib/http_request.o:
	cc -o lib/http_request.o -c lib/http_request.c

lib/http_response.o:
	cc -o lib/http_response.o -c lib/http_response.c

lib/http_server.o:
	cc -o lib/http_server.o -c lib/http_server.c

lib/hashmap.o:
	cc -o  lib/hashmap.o -c lib/hashmap.c

clean:
	rm test test.o lib/log.o lib/acceptor.o lib/utils.o lib/event_loop.o lib/channel_map.o lib/event_dispatcher.o lib/buffer.o lib/tcp_connection.o lib/tcp_server.o lib/channel.o lib/thread_pool.o lib/event_loop_thread.o lib/http_request.o lib/http_response.o lib/http_server.o lib/hashmap.o 