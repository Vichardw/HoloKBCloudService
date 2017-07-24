//
// Created by weijj3 on 2017/7/17.
//

#include "NativeHoloDataBuffer.h"
#include "holoKBAnalysisService.h"

int g_pUpdateBuffer[40];

NativeHoloDataBuffer::NativeHoloDataBuffer(){
    LOG_FUNC_ENTRY;
}

int NativeHoloDataBuffer::getUpdateBuffer(int** pBuffer){
    LOG_FUNC_ENTRY;
    for(int i = 0; i < 40; i++){
        g_pUpdateBuffer[i] = 77 + i;
    }

    *pBuffer = g_pUpdateBuffer;

    notifyUpdate();
    return 40;
}
