//
// Created by weijj3 on 2017/7/14.
//

#include "holoKBAnalysisService.h"

static int gKBDeviceFd;
static JNIEnv* gJNIEnv;
static jobject gHoloDataServiveObj;
static jmethodID gNotifyUpdateMethodId;

void notifyUpdate(){
    if(gHoloDataServiveObj == NULL){
        return;
    }

    jclass cls = gJNIEnv->GetObjectClass(gHoloDataServiveObj);
    gNotifyUpdateMethodId = gJNIEnv->GetMethodID(cls, "updateNotify", "()V");
    gJNIEnv->CallVoidMethod(gHoloDataServiveObj, gNotifyUpdateMethodId);
}
#if 0
extern "C"
JNIEXPORT jstring JNICALL
Java_com_lenovo_holokbcloudservice_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    LOG_FUNC_ENTRY;

    JNIEnv *env = NULL;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI GetEnv Error");
        return JNI_ERR;
    }

    LOGD("start load...");

    return JNI_VERSION_1_6;
}
#endif

extern "C"
JNIEXPORT jlong JNICALL
Java_com_lenovo_holokbcloudservice_HoloDataService_nativeInit(JNIEnv *env, jobject instance) {
    LOG_FUNC_ENTRY;
    gHoloDataServiveObj = instance;
    gJNIEnv = env;
    NativeHoloDataBuffer* buffer = new NativeHoloDataBuffer();
    return reinterpret_cast<jlong>(buffer);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_lenovo_holokbcloudservice_HoloDataService_nativeStartService(JNIEnv *env,
                                                                      jobject instance) {
    int fd;
    fd = openHoloKB();
    if(fd > 0){
        gKBDeviceFd = fd;
        startCollection(fd);
        return JNI_TRUE;
    }

    return JNI_FALSE;
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_lenovo_holokbcloudservice_HoloDataService_nativReadBuffer(JNIEnv *env, jobject instance,
                                                                   jlong ptr) {
    LOG_FUNC_ENTRY;
    NativeHoloDataBuffer* buffer = reinterpret_cast<NativeHoloDataBuffer*>(ptr);
    int size = 0;
    int *pBuffer = NULL;
    size = buffer->getUpdateBuffer(&pBuffer);
    if(size > 0){
        jintArray array = env->NewIntArray( size);
        env->SetIntArrayRegion(array, 0, size, pBuffer);
        return array;
    }

    return NULL;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_lenovo_holokbcloudservice_HoloDataService_nativeStopService(JNIEnv *env,
                                                                     jobject instance) {
    stopCollection(gKBDeviceFd);
    return JNI_TRUE;
}