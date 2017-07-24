package com.lenovo.holokbcloudservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

//
// Created by weijj3 on 2017/7/14.
//

public class SelfStartReciever extends BroadcastReceiver {
    static private String TAG = "HoloKBCollectService";
    static private boolean DEBUG = true;
    @Override
    public void onReceive(Context context, Intent intent) {
        if(DEBUG)
            Log.d(TAG,"Entry SelfStartReciever onReceive");

        if(Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction())){
            MyService.checkOrStartMyService(context);
        }

        if(DEBUG)
            Log.d(TAG,"Exit SelfStartReciever onReceive");
    }
}