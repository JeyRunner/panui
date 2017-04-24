package com.panui.panuiExample;

import android.app.Activity;
import android.util.Log;
import android.os.Bundle;
import android.widget.TextView;
import org.libsdl.app.SDLActivity;

public class main extends SDLActivity
{
    static
    {
       // System.loadLibrary("bluePrintCmake");
    }


    @Override
    protected String[] getLibraries() {
        return new String[] {
            //"SDL2-2.0",
            // "SDL2_image",
            // "SDL2_mixer",
            // "SDL2_net",
            // "SDL2_ttf",
            "panuiExample"
        };
    }
    
    /** Called when the activity is first created. */
    /*
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        Log.i("GO", "start app");

        // load libs
        System.loadLibrary("bluePrintCmake");


        TextView tv = (TextView) findViewById(R.id.mytest);
        if (tv != null){
            tv.setText(String.valueOf(stringFromJNI(1000)));
        }
        else
        {
            Log.i("c++", stringFromJNI(1000));
        }


        setContentView(R.layout.main);
    }

     public native String stringFromJNI(int howMuch);
     */
}
