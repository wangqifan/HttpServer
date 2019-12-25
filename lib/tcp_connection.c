#include "tcp_connection.h"
#include "utils.h"

int handle_read(void *data) {
    struct tcp_connection *tcpConnection = (struct tcp_connection *) data;
    struct buffer *input_buffer = tcpConnection->input_buffer;
    struct channel *channel = tcpConnection->channel;

    if (buffer_socket_read(input_buffer, channel->fd) > 0) {
        //应用程序真正读取Buffer里的数据
        if (tcpConnection->messageCallBack != NULL) {
            tcpConnection->messageCallBack(input_buffer, tcpConnection);
        }
    } else {
       // handle_connection_closed(tcpConnection);
    }
}

//发送缓冲区可以往外写
//把channel对应的output_buffer不断往外发送
int handle_write(void *data) {
    struct tcp_connection *tcpConnection = (struct tcp_connection *) data;
    struct event_loop *eventLoop = tcpConnection->eventLoop;
    assertInSameThread(eventLoop);

    struct buffer *output_buffer = tcpConnection->output_buffer;
    struct channel *channel = tcpConnection->channel;

    ssize_t nwrited = write(channel->fd, output_buffer->data + output_buffer->readIndex,
                            buffer_readable_size(output_buffer));
    if (nwrited > 0) {
        //已读nwrited字节
        output_buffer->readIndex += nwrited;
        //如果数据完全发送出去，就不需要继续了
        if (buffer_readable_size(output_buffer) == 0) {
            channel_write_event_disable(channel);
        }
        //回调writeCompletedCallBack
        if (tcpConnection->writeCompletedCallBack != NULL) {
            tcpConnection->writeCompletedCallBack(tcpConnection);
        }
    } else {
        app_msgx("handle_write for tcp connection %s", tcpConnection->name);
    }

}

struct tcp_connection *
tcp_connection_new(int connected_fd, struct event_loop *eventLoop,
                   connection_completed_call_back connectionCompletedCallBack,
                   connection_closed_call_back connectionClosedCallBack,
                   message_call_back messageCallBack, write_completed_call_back writeCompletedCallBack) {
    struct tcp_connection *tcpConnection = malloc(sizeof(struct tcp_connection));
    tcpConnection->writeCompletedCallBack = writeCompletedCallBack;
    tcpConnection->messageCallBack = messageCallBack;
    tcpConnection->connectionCompletedCallBack = connectionCompletedCallBack;
    tcpConnection->connectionClosedCallBack = connectionClosedCallBack;
    tcpConnection->eventLoop = eventLoop;

    char *buf = malloc(16);
    sprintf(buf, "connection-%d\0", connected_fd);
    tcpConnection->name = buf;

    // add event read for the new connection
    struct channel *channel1 = channel_new(connected_fd, EVENT_READ, handle_read, handle_write, tcpConnection);
    tcpConnection->channel = channel1;

    tcpConnection->input_buffer = buffer_new();
    tcpConnection->output_buffer = buffer_new();

    //connectionCompletedCallBack callback
    if (tcpConnection->connectionCompletedCallBack != NULL) {
        tcpConnection->connectionCompletedCallBack(tcpConnection);
    }

    event_loop_add_channel_event(tcpConnection->eventLoop, connected_fd, tcpConnection->channel);
    return tcpConnection;
}
