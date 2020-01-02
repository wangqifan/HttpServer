#include "lib/http_server.h"
#include "lib/common.h"
#include "lib/event_loop.h"

char* homeHandle(struct http_request *httpRequest, struct http_response *httpResponse)
{
        httpResponse->statusCode = OK;
        httpResponse->statusMessage = "OK";
        httpResponse->contentType = "text/html";
        httpResponse->body = "<html><head><title>This is network programming</title></head><body><h1>Hello, network programming</h1></body></html>";
}


int main() {
   
    //主线程event_loop
    struct event_loop *eventLoop = event_loop_init();

    //初始tcp_server，可以指定线程数目，如果线程是0，就是在这个线程里acceptor+i/o；如果是1，有一个I/O线程
    //tcp_server自己带一个event_loop
    struct http_server *httpServer = http_server_new(eventLoop, 80, 4);
    add_handle(httpServer, "/",homeHandle);
    add_handle(httpServer, "/home",homeHandle);
    http_server_start(httpServer);

    // main thread for acceptor
    event_loop_run(eventLoop);
    return 0;
}