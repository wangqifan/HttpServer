#include "event_loop.h"
#include <assert.h>
#include "log.h"
#include "channel.h"
#include "utils.h"


int event_loop_handle_pending_channel(struct event_loop *eventLoop) {
    //get the lock
    pthread_mutex_lock(&eventLoop->mutex);
    eventLoop->is_handle_pending = 1;

    struct channel_element *channelElement = eventLoop->pending_head;
    while (channelElement != NULL) {
        //save into event_map
        struct channel *channel = channelElement->channel;
        int fd = channel->fd;
        app_msgx("type %d", channelElement->type);
        if (channelElement->type == 1) {
            event_loop_handle_pending_add(eventLoop, fd, channel);
        } else if (channelElement->type == 2) {
            event_loop_handle_pending_remove(eventLoop, fd, channel);
        } else if (channelElement->type == 3) {
            event_loop_handle_pending_update(eventLoop, fd, channel);
        }
        channelElement = channelElement->next;
    }

    eventLoop->pending_head = eventLoop->pending_tail = NULL;
    eventLoop->is_handle_pending = 0;

    //release the lock
    pthread_mutex_unlock(&eventLoop->mutex);

    return 0;
}

// in the i/o thread
int event_loop_handle_pending_remove(struct event_loop *eventLoop, int fd, struct channel *channel1) {
    struct channel_map *map = eventLoop->channelMap;
    assert(fd == channel1->fd);

    if (fd < 0)
        return 0;

    if (fd >= map->nentries)
        return (-1);

    struct channel *channel2 = map->entries[fd];
    int retval = 0;
    if (event_delete(eventLoop, channel2) == -1) {
        retval = -1;
    } else {
        retval = 1;
    }

    map->entries[fd] = NULL;
    return retval;
}


// in the i/o thread
int event_loop_handle_pending_update(struct event_loop *eventLoop, int fd, struct channel *channel) {
    app_msgx("update channel fd == %d, %s", fd, eventLoop->thread_name);
    struct channel_map *map = eventLoop->channelMap;

    if (fd < 0)
        return 0;

    if ((map)->entries[fd] == NULL) {
        return (-1);
    }

    //update channel
    event_update(eventLoop, channel);
}

struct event_loop *event_loop_init() {
    return event_loop_init_with_name(NULL);
}

struct event_loop *event_loop_init_with_name(char *thread_name) {
    struct event_loop *eventLoop = malloc(sizeof(struct event_loop));
    pthread_mutex_init(&eventLoop->mutex, NULL);

    if (thread_name != NULL) {
        eventLoop->thread_name = thread_name;
    } else {
        eventLoop->thread_name = "main thread";
    }

    eventLoop->channelMap = malloc(sizeof(struct channel_map));
    map_init(eventLoop->channelMap);

    eventLoop->quit = 0;
    
   
    app_msgx("dispatcher init, %s", eventLoop->thread_name);
    eventLoop->event_dispatcher_data = dispatcher_data_init(eventLoop);

    //add the socketfd to event
    eventLoop->owner_thread_id = pthread_self();
   
    eventLoop->is_handle_pending = 0;
    eventLoop->pending_head = NULL;
    eventLoop->pending_tail = NULL;

    return eventLoop;
}


int event_loop_run(struct event_loop *eventLoop) {
    assert(eventLoop != NULL);
    if (eventLoop->owner_thread_id != pthread_self()) {
        exit(1);
    }

    app_msgx("event loop run, %s", eventLoop->thread_name);
    struct timeval timeval;
    timeval.tv_sec = 1;

    while (!eventLoop->quit) {
        //block here to wait I/O event, and get active channels
        app_msgx("event_dispatch");
        event_dispatch(eventLoop, &timeval);

        app_msgx("event_loop_handle_pending_channel");
        //handle the pending channel
        event_loop_handle_pending_channel(eventLoop);
    }

    app_msgx("event loop end, %s", eventLoop->thread_name);
    return 0;
}

void event_loop_channel_buffer_nolock(struct event_loop *eventLoop, int fd, struct channel *channel1, int type) {
    //add channel into the pending list
    struct channel_element *channelElement = malloc(sizeof(struct channel_element));
    channelElement->channel = channel1;
    channelElement->type = type;
    channelElement->next = NULL;
    //第一个元素
    if (eventLoop->pending_head == NULL) {
        eventLoop->pending_head = eventLoop->pending_tail = channelElement;
    } else {
        eventLoop->pending_tail->next = channelElement;
        eventLoop->pending_tail = channelElement;
    }
}

int event_loop_do_channel_event(struct event_loop *eventLoop, int fd, struct channel *channel1, int type) {
    //get the lock
    app_log("get mutex of event_loop %s","eve");
    pthread_mutex_lock(&eventLoop->mutex);
    assert(eventLoop->is_handle_pending == 0);
    // 加入待处理队列
    event_loop_channel_buffer_nolock(eventLoop, fd, channel1, type);
    //release the lock
    pthread_mutex_unlock(&eventLoop->mutex);
    if (!isInSameThread(eventLoop)) {
     //   event_loop_wakeup(eventLoop);
    } else {
        event_loop_handle_pending_channel(eventLoop);
    }

    return 0;

}

int event_loop_add_channel_event(struct event_loop *eventLoop, int fd, struct channel *channel1) {
    return event_loop_do_channel_event(eventLoop, fd, channel1, 1);
}

int event_loop_handle_pending_add(struct event_loop *eventLoop, int fd, struct channel *channel1) {
    app_msgx("add channel fd == %d, %s", fd, eventLoop->thread_name);
    struct channel_map *map = eventLoop->channelMap;

    if (fd < 0)
        return 0;

    if (fd >= map->nentries) {
        if (map_make_space(map, fd, sizeof(struct channel *)) == -1)
            return (-1);
    }

    //第一次创建，增加
    if ((map)->entries[fd] == NULL) {
        map->entries[fd] = calloc(1, sizeof(struct channel *));
        map->entries[fd] = channel1;
        //add channel
        event_add(eventLoop, channel1);
        return 1;
    }

    return 0;
}

int channel_event_activate(struct event_loop *eventLoop, int fd, int revents) {
    struct channel_map *map = eventLoop->channelMap;
    app_msgx("activate channel fd == %d, revents=%d, %s", fd, revents, eventLoop->thread_name);

    if (fd < 0)
        return 0;

    if (fd >= map->nentries)return (-1);

    struct channel *channel = map->entries[fd];
    assert(fd == channel->fd);

    if (revents & (EVENT_READ)) {
        if (channel->eventReadCallback) channel->eventReadCallback(channel->data);
    }
    if (revents & (EVENT_WRITE)) {
        if (channel->eventWriteCallback) channel->eventWriteCallback(channel->data);
    }

    return 0;

}
