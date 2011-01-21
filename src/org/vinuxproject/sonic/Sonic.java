package org.vinuxproject.sonic;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

public class Sonic extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        setContentView(tv);
               
    }

    /* A native method that is implemented by the
     * 'sonic' native library, which is packaged
     * with this application.
     */
    public native String  stringFromJNI();

    public native void init(int sampleRate, int channels);
    public native void close();
    public native void setPitch(float newPitch);
    public native float getPitch();
    public native void setSpeed(float newSpeed);
    public native float getSpeed();
    public native void putBytes(byte[] buffer, int lenBytes);
    public native int receiveBytes(byte[] ret, int lenBytes);
    public native int availableBytes();

    /* Public so it wont get optimized away */
// TODO: replace this with cleaner version
    public int ptr;

    static {
        System.loadLibrary("sonic");
    }
}
