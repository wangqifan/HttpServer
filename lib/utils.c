#include "utils.h"
#include "log.h"
#include "common.h"


void assertInSameThread(struct event_loop *eventLoop) {
    if (eventLoop->owner_thread_id != pthread_self()) {
        APP_LOG_MSG("not in the same thread");
        exit(-1);
    }
}

//1： same thread: 0： not the same thread
int isInSameThread(struct event_loop *eventLoop){
    return eventLoop->owner_thread_id == pthread_self();
}

void make_nonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}