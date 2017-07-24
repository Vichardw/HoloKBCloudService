package com.lenovo.holokbcloudservice;

import android.util.Log;

import java.nio.IntBuffer;

//
// Created by weijj3 on 2017/7/18.
//

public class HoloDataService{
    static private String TAG = "HoloDataService";
    static private boolean DEBUG = true;
    //the pointer to the class object in native code be related with this object.
    private long mPtr;

    static {
        System.loadLibrary("holoKBAnalysisService");   //defaultConfig.ndk.moduleName
    }

    public HoloDataService(){
        if(DEBUG)
            Log.d(TAG, "Entry HoloDataService");
    }

    public void initService(){
        mPtr = nativeInit();
    }

    public void startService(){
        if(!nativeStartService()){
            Log.e(TAG, "Failed to start native service!");
        }
    }

    public void updateNotify(){
        if(DEBUG)
            Log.d(TAG, "Update called from native");
    }

    public void stopService(){
        nativeStopService();
    }

    private native long nativeInit();
    private native boolean nativeStartService();
    private native int[] nativReadBuffer(long ptr);
    private native boolean nativeStopService();
}