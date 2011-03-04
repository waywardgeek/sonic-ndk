// This file was written by me, Bill Cox in 2011.
// I place this file into the public domain.  Feel free to copy from it.
// Note, however that libsonic, which this application links to,
// is licensed under LGPL.  You can link to it in your commercial application,
// but any changes you make to sonic.c or sonic.h need to be shared under
// the LGPL license.

package org.vinuxproject.sonic;

import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class SonicTest extends Activity
{
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);                      
        setContentView(R.layout.main);
    }
    
    public void play(View view)
    {
        new Thread(new Runnable() 
        {
            public void run()
            {
            	final EditText speedEdit = (EditText) findViewById(R.id.speed);
            	final EditText pitchEdit = (EditText) findViewById(R.id.pitch);
            	final EditText rateEdit = (EditText) findViewById(R.id.rate);
            	float speed = Float.parseFloat(speedEdit.getText().toString());
            	float pitch = Float.parseFloat(pitchEdit.getText().toString());
            	float rate = Float.parseFloat(rateEdit.getText().toString());
                AndroidAudioDevice device = new AndroidAudioDevice(22050, 1);
                Sonic sonic = new Sonic(22050, 1);
                byte samples[] = new byte[4096];
                byte modifiedSamples[] = new byte[2048];
                InputStream soundFile = getResources().openRawResource(R.raw.talking);
				int bytesRead;

				if(soundFile != null) {
				    sonic.setSpeed(speed);
				    sonic.setPitch(pitch);
				    sonic.setRate(rate);
				    do {
				        try {
							bytesRead = soundFile.read(samples, 0, samples.length);
						} catch (IOException e) {
							e.printStackTrace();
							return;
						}
				        if(bytesRead > 0) {
				        	sonic.putBytes(samples, bytesRead);
				        } else {
						    sonic.flush();
				        }
			        	int available = sonic.availableBytes(); 
			        	if(available > 0) {
			        		if(modifiedSamples.length < available) {
			        		    modifiedSamples = new byte[available*2];
			        		}
			        		sonic.receiveBytes(modifiedSamples, available);
			        		device.writeSamples(modifiedSamples, available);
			        	}
				    } while(bytesRead > 0);
				    device.flush();
				}
            }
        } ).start();
    }
}
