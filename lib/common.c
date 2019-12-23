#include "common.h"

void make_nonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}