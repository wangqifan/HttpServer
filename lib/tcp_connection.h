#ifndef TCP_CONNECTION
#define TCP_CONNECTION

#include "event_loop.h"
#include "channel.h"
#include "tcp_server.h"
#include "buffer.h"

struct tcp_connection {
    struct event_loop *eventLoop;
    struct channel *channel;
    char *name;

    struct buffer *input_buffer;   //接收缓冲区
    struct buffer *output_buffer;  //发送缓冲区


    connection_completed_call_back connectionCompletedCallBack;
    message_call_back messageCallBack;
    write_completed_call_back writeCompletedCallBack;
    connection_closed_call_back connectionClosedCallBack;

};

struct tcp_connection *
tcp_connection_new(int fd, struct event_loop *eventLoop, connection_completed_call_back connectionCompletedCallBack,
                   connection_closed_call_back connectionClosedCallBack,
                   message_call_back messageCallBack, write_completed_call_back writeCompletedCallBack);

#endif