#include "lib/log.h"
#include "lib/acceptor.h"

int main() {
    app_msgx("test log app starting");
    app_debugx("test log app starting");
    struct acceptor *acceptor  = acceptor_init(80);
    app_msgx("listen fd is:   %d", acceptor->listen_fd);
    app_msgx("listen port is:  %d", acceptor->listen_port);


    struct event_loop *eventLoop = event_loop_init();


    
    event_loop_run(eventLoop);
    
    return 0;
}