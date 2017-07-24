package com.lenovo.holokbcloudservice;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;

public class MainActivity extends Activity {
    static private String TAG = "Holo MainActivity";
    static private boolean DEBUG = true;
    private Button mAllowButton;
    private Button mDenyButton;
    private CheckBox mNeverCheckbox;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(DEBUG){
            Log.d(TAG, "Entry onCreate");
        }
        Window win = getWindow();
        WindowManager.LayoutParams attrs = win.getAttributes();
        //attrs.width = 720;
        //attrs.height = 480;
        attrs.format = PixelFormat.OPAQUE;
        attrs.type = WindowManager.LayoutParams.TYPE_SYSTEM_DIALOG;
        attrs.flags |= WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON;
        win.setAttributes(attrs);
        win.setGravity(Gravity.CENTER);
        win.setLayout(720,480);
        win.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        mAllowButton = (Button)findViewById(R.id.allowed_start_collection);
        mDenyButton = (Button)findViewById(R.id.deny_collection_service);

        mNeverCheckbox = (CheckBox)findViewById(R.id.checkBox);

        mAllowButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(DEBUG){
                    Log.d(TAG, "User allow Holo keyboard collect.");
                }
                KBCollectService.setCollectionAllowed(MainActivity.this,true,false);
                MyService.launchMyService(MainActivity.this);
                MainActivity.this.finish();
            }
        });

        mDenyButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(DEBUG){
                    Log.d(TAG, "User deny Holo keyboard collect.");
                }
                boolean shouldPromptLater = mNeverCheckbox.isChecked();
                KBCollectService.setCollectionAllowed(MainActivity.this,true,!shouldPromptLater);
                MyService.destroyMyService(MainActivity.this);
                MainActivity.this.finish();
            }
        });
    }

}
