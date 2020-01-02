#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "hashmap.h"
#include  "common.h"
#include "tcp_server.h"
#include "http_request.h"
#include "http_response.h"
#include "tcp_connection.h"

typedef int (*request_callback)(struct http_server *httpServer,struct http_request *httpRequest, struct http_response *httpResponse);
typedef char* (*httpHandle)(struct http_request *httpRequest, struct http_response *httpResponse);

struct http_server {
    struct TCPserver *tcpServer;
    request_callback requestCallback;
    hashmap_map* routeMap;

};

struct http_server *http_server_new(struct event_loop *eventLoop, int port,
                                    int threadNum
                                    );


void http_server_start(struct http_server *httpServer);

int parse_http_request(struct buffer *input, struct http_request *httpRequest);

void add_handle(struct http_server *httpServer, char* url, httpHandle handle1);

#endif