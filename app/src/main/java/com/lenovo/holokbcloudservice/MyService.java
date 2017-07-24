package com.lenovo.holokbcloudservice;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.HandlerThread;
import android.os.IBinder;
import android.util.Log;

//
// Created by weijj3 on 2017/7/14.
//

public class MyService extends Service {
    static private String TAG = "HoloMyService";
    static private boolean DEBUG = true;
    static private String ACTION_CHECK_OR_START_SERVICE = "com.lenovo.holokbdataservice.checkservice";
    static private String ACTION_LAUNCH_SERVICE = "com.lenovo.holokbdataservice.launchservice";
    static private String ACTION_DESTROY_SERVICE = "com.lenovo.holokbdataservice.destroyservice";
    private KBCollectService mService = null;

    public MyService() {
    }

    static public void launchMyService(Context context){
        final Intent intentStart = new Intent(context, MyService.class);
        intentStart.setAction(ACTION_LAUNCH_SERVICE);
        context.startService(intentStart);
    }

    static public void destroyMyService(Context context){
        Intent intentStart = new Intent(context, MyService.class);
        intentStart.setAction(ACTION_DESTROY_SERVICE);
        context.startService(intentStart);
    }

    static public void checkOrStartMyService(Context context){
        Intent intentStart = new Intent(context, MyService.class);
        intentStart.setAction(ACTION_CHECK_OR_START_SERVICE);
        context.startService(intentStart);
    }

    @Override
    public void onCreate() {
        // TODO: It would be nice to have an option to hold a partial wakelock
        // during processing, and to have a static startService(Context, Intent)
        // method that would launch the service & hand off a wakelock.
        if(DEBUG)
            Log.d(TAG, "Entry MyService onCreeate");
        super.onCreate();

    }

    private boolean startCollectionService(){
        if(DEBUG)
            Log.d(TAG, "Launch service");
        mService = KBCollectService.getInstance();
        return mService.startService();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(DEBUG)
            Log.d(TAG, "Entry MyService onStartCommand");

        if(ACTION_CHECK_OR_START_SERVICE.equals(intent.getAction())){
            if(!KBCollectService.getCollectionAllowed(this)&&KBCollectService.getPromptLater(this)){
                final Intent i = new Intent();
                i.setClass(this,MainActivity.class);
                startActivity(i);
            }else if(KBCollectService.getCollectionAllowed(this)){
                startCollectionService();
            }else{
                if(DEBUG)
                    Log.d(TAG, "User choose deny collection, stop itself");
                stopSelf();
                return START_NOT_STICKY;
            }
        } else if(ACTION_LAUNCH_SERVICE.equals(intent.getAction())){
            startCollectionService();
        }else if(ACTION_DESTROY_SERVICE.equals(intent.getAction())) {
            if(DEBUG)
                Log.d(TAG, "Destroy service");
            if(mService == null){
                Log.e(TAG, "Should not happen, Have a check");
            }else{
                mService.detroyService();
            }

            stopSelf();
        }

        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("The device don't support bind communication.");
    }
}
