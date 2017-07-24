//
// Created by weijj3 on 2017/7/17.
//

#ifndef HOLOKBCLOUDSERVICE_NATIVEHOLODATABUFFER_H
#define HOLOKBCLOUDSERVICE_NATIVEHOLODATABUFFER_H
#include "logger.h"

class NativeHoloDataBuffer{
public:
    NativeHoloDataBuffer();
    int getUpdateBuffer(int** pBuffer);
};

#endif //HOLOKBCLOUDSERVICE_NATIVEHOLODATABUFFER_H
