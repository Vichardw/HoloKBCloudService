package com.lenovo.holokbcloudservice;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

/**
 * Created by weijj3 on 2017/7/18.
 */

public class KBCollectService {
    static private String TAG = "HoloKBCollectService";
    static private boolean DEBUG = true;
    static private String ConfigFile = "UserConfig";
    static private String AllowCollection = "allowedDataCollection";
    static private String Prompt = "shouldPromptLater";
    static private KBCollectService sCollectService = null;

    private volatile Looper mServiceLooper;
    private volatile ServiceHandler mServiceHandler;
    private volatile HoloDataService mDataBuffer;

    private final class ServiceHandler extends Handler {
        public ServiceHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {

        }
    }

    private KBCollectService(Looper loop){
        mServiceLooper = loop;
        mServiceHandler = new ServiceHandler(mServiceLooper);
        mDataBuffer = new HoloDataService();
    }

    public boolean startService(){
        mDataBuffer.initService();
        mDataBuffer.startService();
        return true;
    }

    public void detroyService(){
        if(DEBUG)
            Log.d(TAG, "Quit KBCollectService");

        mDataBuffer.stopService();
        mServiceLooper.quitSafely();
    }

    static public KBCollectService getInstance(){
        if(sCollectService != null){
            return sCollectService;
        }

        if(DEBUG)
            Log.d(TAG, "Create KBCollectService");

        HandlerThread thread = new HandlerThread("HoloKBService");
        thread.start();

        Looper looper = thread.getLooper();
        sCollectService = new KBCollectService(looper);
        return sCollectService;
    }

    static public boolean setCollectionAllowed(Context context, boolean allowed, boolean prompt){
        Editor editor  = context.getSharedPreferences(ConfigFile, Context.MODE_PRIVATE).edit();
        editor.putBoolean(AllowCollection, allowed);
        editor.putBoolean(Prompt, prompt);
        return editor.commit();
    }

    static public boolean getCollectionAllowed(Context context){
        SharedPreferences refer  = context.getSharedPreferences(ConfigFile, Context.MODE_PRIVATE);
        return refer.getBoolean(AllowCollection,false);
    }

    static public boolean getPromptLater(Context context){
        SharedPreferences refer  = context.getSharedPreferences(ConfigFile, Context.MODE_PRIVATE);
        return refer.getBoolean(Prompt,true);
    }
}
