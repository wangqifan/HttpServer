#include  <sys/epoll.h>

#include "event_dispatcher.h"

#define MAXEVENTS 128

typedef struct {
    int event_count;
    int nfds;
    int realloc_copy;
    int efd;
    struct epoll_event *events;
} epoll_dispatcher_data;

void* dispatcher_data_init(struct event_loop * eventLoop) {
    epoll_dispatcher_data *epollDispatcherData = malloc(sizeof(epoll_dispatcher_data));

    epollDispatcherData->event_count = 0;
    epollDispatcherData->nfds = 0;
    epollDispatcherData->realloc_copy = 0;
    epollDispatcherData->efd = 0;

    epollDispatcherData->efd = epoll_create1(0);
    if (epollDispatcherData->efd == -1) {
        error(1, errno, "epoll create failed");
    }

    epollDispatcherData->events = calloc(MAXEVENTS, sizeof(struct acceptor));

    return epollDispatcherData;
}

int event_dispatch(struct event_loop * eventLoop, struct timeval *timeval) {
    epoll_dispatcher_data *epollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;
    int i, n;

    n = epoll_wait(epollDispatcherData->efd, epollDispatcherData->events, MAXEVENTS, -1);
    app_msgx("epoll_wait wakeup, %s", eventLoop->thread_name);
    for (i = 0; i < n; i++) {
        if ((epollDispatcherData->events[i].events & EPOLLERR) || (epollDispatcherData->events[i].events & EPOLLHUP)) {
            fprintf(stderr, "epoll error\n");
            close(epollDispatcherData->events[i].data.fd);
            continue;
        }

        if (epollDispatcherData->events[i].events & EPOLLIN) {
            app_msgx("get message channel fd==%d for read, %s", epollDispatcherData->events[i].data.fd, eventLoop->thread_name);
            channel_event_activate(eventLoop, epollDispatcherData->events[i].data.fd, EVENT_READ);
        }

        if (epollDispatcherData->events[i].events & EPOLLOUT) {
            app_msgx("get message channel fd==%d for write, %s", epollDispatcherData->events[i].data.fd,eventLoop->thread_name);
            channel_event_activate(eventLoop, epollDispatcherData->events[i].data.fd, EVENT_WRITE);
        }
    }

    return 0;
}

void event_clear(struct event_loop * eventLoop) {
    epoll_dispatcher_data *epollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    free(epollDispatcherData->events);
    close(epollDispatcherData->efd);
    free(epollDispatcherData);
    eventLoop->event_dispatcher_data = NULL;

    return;
}

int event_add(struct event_loop * eventLoop, struct channel * channel1) {
    epoll_dispatcher_data *pollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;
    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | EPOLLIN;
    }
    if (channel1->events & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(pollDispatcherData->efd, EPOLL_CTL_ADD, fd, &event) == -1) {
        error(1, errno, "epoll_ctl add  fd failed");
    }

    return 0;
}

int event_delete(struct event_loop * eventLoop, struct channel * channel1) {
    epoll_dispatcher_data *pollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;

    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | EPOLLIN;
    }

    if (channel1->events & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(pollDispatcherData->efd, EPOLL_CTL_DEL, fd, &event) == -1) {
        error(1, errno, "epoll_ctl delete fd failed");
    }

    return 0;
}

int event_update(struct event_loop * eventLoop, struct channel *channel1) {
    epoll_dispatcher_data *pollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;

    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | EPOLLIN;
    }

    if (channel1->events & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(pollDispatcherData->efd, EPOLL_CTL_MOD, fd, &event) == -1) {
        error(1, errno, "epoll_ctl modify fd failed");
    }

    return 0;
}