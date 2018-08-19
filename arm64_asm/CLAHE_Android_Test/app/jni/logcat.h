#include <android/log.h>

#define LOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, ("[YC]ZJNI"), __VA_ARGS__)
#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG  , ("[YC]ZJNI"), __VA_ARGS__)
#define LOGI(...)	__android_log_print(ANDROID_LOG_INFO   , ("[YC]ZJNI"), __VA_ARGS__)
#define LOGW(...)	__android_log_print(ANDROID_LOG_WARN   , ("[YC]ZJNI"), __VA_ARGS__)
#define LOGE(...)	__android_log_print(ANDROID_LOG_ERROR  , ("[YC]ZJNI"), __VA_ARGS__)
