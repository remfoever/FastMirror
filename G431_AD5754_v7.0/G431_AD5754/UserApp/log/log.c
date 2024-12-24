#include "log.h"
#include <stdarg.h>

void EM_LOG(const int level, const char* file, const char* fun, const int line, const char* fmt, ...)
{
    // ��־�����ַ���ӳ��
    const char* levelStr;
    switch (level) {
        case LOG_DEBUG: levelStr = "DEBUG"; break;
        case LOG_INFO:  levelStr = "INFO"; break;
        case LOG_WARN:  levelStr = "WARN"; break;
        case LOG_ERROR: levelStr = "ERROR"; break;
        default: levelStr = "UNKNOWN"; break;
    }

    // ��ӡ��־
    printf("[%s] [%s] [%s:%d] ", levelStr, file, fun, line);

    // �ɱ��������
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf("\n");
}
