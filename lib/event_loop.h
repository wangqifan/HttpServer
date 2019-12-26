#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "common.h"
#include "channel_map.h"

struct event_loop {
    int quit;
    void *event_dispatcher_data;
    pthread_t owner_thread_id;
    int is_handle_pending;

    struct channel_map *channelMap;
    
    // to handle event list
    struct channel_element *pending_head;
    struct channel_element *pending_tail;

    int socketPair[2];

    pthread_mutex_t mutex;
    char *thread_name;
};

struct channel_element {
    int type; //1: add  2: delete
    struct channel *channel;
    struct channel_element *next;
};

// 初始化
struct event_loop *event_loop_init();

struct event_loop *event_loop_init_with_name(char * thread_name);

int event_loop_add_channel_event(struct event_loop *eventLoop, int fd, struct channel *channel1);

int channel_event_activate(struct event_loop *eventLoop, int fd, int res);

int event_loop_handle_pending_add(struct event_loop *eventLoop, int fd, struct channel *channel);

int event_loop_handle_pending_remove(struct event_loop *eventLoop, int fd, struct channel *channel);

int event_loop_handle_pending_update(struct event_loop *eventLoop, int fd, struct channel *channel);

#include "event_dispatcher.h"
#endif