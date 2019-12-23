#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "common.h"
#include "channel.h"
#include "acceptor.h"
#include "log.h"
#include "event_loop.h"

/**  初始化函数 */
void* dispatcher_data_init(struct event_loop * eventLoop);

/** 实现事件分发，然后调用event_loop的event_activate方法执行callback*/
int event_dispatch(struct event_loop * eventLoop, struct timeval *);

/** 通知dispatcher新增一个channel事件*/
int event_add(struct event_loop * eventLoop, struct channel * channel1);

/** 通知dispatcher删除一个channel事件 */ 
int event_delete(struct event_loop *  eventLoop, struct channel * channel1);

/** 通知dispatcher更新channel对应的事件*/
int event_update(struct event_loop * eventLoop, struct channel * channel1);

/** 清除数据 */
void event_clear(struct event_loop * eventLoop);

#endif