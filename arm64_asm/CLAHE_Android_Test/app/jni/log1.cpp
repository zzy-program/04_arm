#include "log1.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef ANDROID
#include <android/log.h>
#endif
#include <pthread.h>
#include<sys/stat.h> 
#include<fcntl.h>  


//#define LOG2FILE


int isDoLog = 1;



void setMediaLogFlag(int isdoLog)
{
	isDoLog = isdoLog;
}



void _LOG(int loglevel, const char* tag, const char *_func, int _line, const char * fmt, ...)
{
	if(isDoLog == 0)
	{
		return;
    }
	char argBuf[1024*10]={0};
	va_list args;
	va_start(args, fmt);
	vsprintf(argBuf, fmt, args);
	va_end(args);

	//output to logcat
	char func_line[1024*10]={0};
	sprintf(func_line,"%s.%s(%lu) %d: ",tag,_func,pthread_self(),_line);
#ifdef ANDROID
	switch(loglevel)
	{
	case LOG_INFO:
		__android_log_print(ANDROID_LOG_INFO , func_line, "%s",argBuf);
	break;
	case LOG_DEBUG:
		__android_log_print(ANDROID_LOG_DEBUG , func_line,"%s", argBuf);
	break;
	case LOG_ERROR:
		__android_log_print(ANDROID_LOG_ERROR , func_line, "%s",argBuf);
	break;
	case LOG_WARN:
		__android_log_print(ANDROID_LOG_WARN , func_line, "%s",argBuf);
	break;
    }
#else
    switch(loglevel){
    case LOG_INFO:
		fprintf(stderr,"[I]");
	break;
	case LOG_DEBUG:
		fprintf(stderr,"[D]");
	break;
	case LOG_ERROR:
		fprintf(stderr,"[E]");
	break;
	case LOG_WARN:
		fprintf(stderr,"[W]");
	break;
    
}
    fprintf(stderr,"%s %s\n", func_line, argBuf);
 
#endif
}





