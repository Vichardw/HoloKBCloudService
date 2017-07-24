//
// Created by weijj3 on 2017/7/14.
//

#ifndef HOLOKBCLOUDSERVICE_LOGGER_H
#define HOLOKBCLOUDSERVICE_LOGGER_H
#include <android/log.h>
#define	LOG_TAG  "HOLOKB"

#define	LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define	LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define	LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define LOG_FUNC_ENTRY __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Entry %s ( %s %d )", __func__, __FILE__, __LINE__)
#define LOG_FUNC_EXIT __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Entry %s ( %s %d )", __func__, __FILE__, __LINE__)
#endif //HOLOKBCLOUDSERVICE_LOGGER_H
