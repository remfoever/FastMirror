#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// ��־������
typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} E_LOGLEVEL;

// ��־�����������
void EM_LOG(const int level, const char* file, const char* fun, const int line, const char* fmt, ...);

// ��־�꣬�������
#define EMLOG(level, fmt, ...) \
    EM_LOG(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#endif // LOG_H
