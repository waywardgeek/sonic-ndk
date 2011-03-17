// This file was written by me, Bill Cox in 2011.
// I place this file into the public domain.  Feel free to copy from it.
// Note, however that libsonic.so, which this application links to,
// is licensed under LGPL.  You can link to it in your commercial application,
// but any changes you make to sonic.c or sonic.h need to be shared under
// the LGPL license.

package org.vinuxproject.sonic;

public class Sonic
{
    // Called when the activity is first created.
    public Sonic(int sampleRate, int numChannels)
    {
        init(sampleRate, numChannels);
    }

    // Just insure that close gets called, in case the user forgot.
    protected void finalize()
    {
    	// It is safe to call this twice, in case the user already did.
        close();
    }
    
    // Since this is called in the constructor, you probably don't need to call init.
    public native void init(int sampleRate, int channels);
    // When done with sound processing, it's best to call this method to clean up memory.
    public native void close();
    public native void flush();
    // Note that changing the sample rate or num channels will cause a flush.
    public native void setSampleRate(int newSampleRate);
    public native int getSampleRate();
    public native void setNumChannels(int newNumChannels);
    public native int getNumChannels();
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
    public native void setVolume(float newVolume);
    public native float getVolume();

    static {
        System.loadLibrary("sonic");
    }
}
