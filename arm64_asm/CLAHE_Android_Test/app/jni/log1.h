#ifndef __STACK__DEBUG_H__
#define __STACK__DEBUG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef ANDROID
#include <android/log.h>
#endif


//int doAudioLog;
#define TRACE_LOG_ON 1
enum LOGLEVEL
{
	LOG_INFO,
    LOG_DEBUG,
    LOG_ERROR,
    LOG_WARN
};


void setMediaLogFlag(int isdoLog);


void _LOG(int loglevel, const char* tag, const char *_func, int _line, const char * fmt, ...);



#define LOG_I(tag, fmt, arg...) do{ if(TRACE_LOG_ON)                    \
            _LOG(LOG_INFO, tag, __FUNCTION__, __LINE__, fmt, ## arg);   \
    }while (0)
#define LOG_D(tag, fmt, arg...) do{ if(TRACE_LOG_ON)                    \
            _LOG(LOG_DEBUG, tag, __FUNCTION__, __LINE__, fmt, ## arg);  \
    }while (0)
#define LOG_E(tag, fmt, arg...) do{ if(TRACE_LOG_ON)                    \
            _LOG(LOG_ERROR, tag,  __FUNCTION__, __LINE__, fmt, ## arg); \
    }while (0)
#define LOG_W(tag, fmt, arg...) do{ if(TRACE_LOG_ON)                    \
            _LOG(LOG_WARN, tag, __FUNCTION__, __LINE__, fmt, ## arg);   \
    }while (0)
     
     
#endif
