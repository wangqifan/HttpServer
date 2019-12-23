#include "lib/log.h"
#include "lib/acceptor.h"

int main() {
    app_msgx("test log app starting");
    app_debugx("test log app starting");
    struct acceptor *acceptor  = acceptor_init(80);
    char str[80];
    sprintf(str, "listen fd is:   %d", acceptor->listen_fd);
    app_msgx(str);
    sprintf(str, "listen port is:  %d", acceptor->listen_port);
    app_msgx(str);
    return 0;
}