#ifndef COMMON_H
#define COMMON_H

#include    <stdio.h>
#include    <errno.h>
#include    <string.h>
#include    <stdlib.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <fcntl.h>


void make_nonblocking(int fd);
void error(int status, int err, char *fmt, ...);


#define    LISTENQ        1024

#endif