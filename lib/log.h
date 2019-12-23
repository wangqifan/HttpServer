#ifndef LOG_H
#define LOG_H

#include "stdarg.h"

#define LOG_DEBUG_TYPE 0
#define LOG_MSG_TYPE   1
#define LOG_WARN_TYPE  2
#define LOG_ERR_TYPE   3

void app_log(int severity, const char *msg);
void app_logx(int severity, const char *errstr, const char *fmt, va_list ap);
void app_msgx(const char *fmt, ...);
void app_debugx(const char *fmt, ...);

#endif