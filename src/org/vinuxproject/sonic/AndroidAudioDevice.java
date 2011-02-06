package org.vinuxproject.sonic;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

public class AndroidAudioDevice
{
    AudioTrack track;
    byte[] buffer = new byte[2048];
 
    private int findFormatFromChannels(int numChannels)
    {
    	switch(numChannels) {
    	case 1: return AudioFormat.CHANNEL_OUT_MONO;
    	case 2: return AudioFormat.CHANNEL_OUT_STEREO;
    	default: return -1; // Error
    	}
    }
    
    public AndroidAudioDevice(int sampleRate, int numChannels)
    {
    	int format = findFormatFromChannels(numChannels);
    	int minSize =AudioTrack.getMinBufferSize(sampleRate, format, AudioFormat.ENCODING_PCM_16BIT);        
        track = new AudioTrack( AudioManager.STREAM_MUSIC, sampleRate, 
            format, AudioFormat.ENCODING_PCM_16BIT, 
            minSize, AudioTrack.MODE_STREAM);
        track.play();        
    }	   
 
    public void flush()
    {
        track.flush();
    }
    
    public void writeSamples(byte[] samples, int length) 
    {	
        fillBuffer( samples );
        track.write( buffer, 0, length);
    }
 
    private void fillBuffer(byte[] samples)
    {
        if(buffer.length < samples.length)
            buffer = new byte[samples.length];
        System.arraycopy(samples, 0, buffer, 0, samples.length);
    }		
}
