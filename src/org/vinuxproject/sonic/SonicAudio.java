package org.vinuxproject.sonic;

public class SonicAudio
{
    // Called when the activity is first created.
    public SonicAudio(int sampleRate, int numChannels)
    {
        init(sampleRate, numChannels);
    }

    // Just insure that close gets called, in case the user forgot.
    protected void finalize()
    {
        if(ptr != 0) {
            close();
        }
    }
    
    // Since this is called in the constructor, you probably don't need to call init.
    public native void init(int sampleRate, int channels);
    // When done with sound processing, it's best to call this method to clean up memory.
    public native void close();
    public native void flush();
    public native void setPitch(float newPitch);
    public native float getPitch();
    public native void setSpeed(float newSpeed);
    public native float getSpeed();
    public native void setRate(float newRate);
    public native float getRate();
    public native void setChordPitch(boolean useChordPitch);
    public native boolean getChordPitch();
    public native void putBytes(byte[] buffer, int lenBytes);
    public native int receiveBytes(byte[] ret, int lenBytes);
    public native int availableBytes();

    // Public so it wont get optimized away
// TODO: replace this with cleaner version
    public int ptr;

    static {
        System.loadLibrary("sonic");
    }
}
