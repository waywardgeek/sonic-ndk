#include <jni.h>
#include <android/log.h>
#include <stdlib.h>
#include "sonic.h"

// For debug messages, use:
// __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "1 + 1 is %d", 1 + 1);
#define APPNAME "Sonic"

static sonicStream stream = 0;
static short *outBuf;
static int outBufSize;

/* Initialize the C data structure */
void Java_org_vinuxproject_sonic_SonicAudio_init(
    JNIEnv *env,
    jobject thiz,
    jint sampleRate,
    jint channels)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Creating sonic stream");
    stream = sonicCreateStream(sampleRate, channels);
    outBufSize = 100;
    outBuf = (short *)calloc(outBufSize, sizeof(short));
}

/* Put bytes into the input buffer of the sound alteration object
   lenBytes bytes will be read from buffer into the sound alteration object
   buffer is not guaranteed not to change after this function is called,
   so data should be copied from it */
void Java_org_vinuxproject_sonic_SonicAudio_putBytes(
    JNIEnv *env,
    jobject thiz,
    jbyteArray buffer,
    jint lenBytes)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Writing %d bytes to stream", lenBytes);
    sonicWriteShortToStream(stream, (short *)(void *)buffer,
        lenBytes/(sizeof(short)*sonicGetNumChannels(stream)));
}

// Get bytes representing sped up/slowed down sound and put up to lenBytes
// into ret.
// Returns number of bytes read.
jint Java_org_vinuxproject_sonic_SonicAudio_receiveBytes(
    JNIEnv *env,
    jobject thiz,
    jbyteArray ret,
    jint lenBytes)
{
    int available = sonicSamplesAvailable(stream)*sizeof(short)*sonicGetNumChannels(stream);
    int samplesRead, bytesRead;

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading %d bytes from stream", lenBytes);
    if(lenBytes > available) {
        lenBytes = available;
    }
    if(lenBytes > outBufSize*sizeof(short)) {
        outBufSize = lenBytes*(2/sizeof(short));
        outBuf = (short *)realloc(outBuf, outBufSize*sizeof(short));
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Doing read %d", lenBytes);
    samplesRead = sonicReadShortFromStream(stream, outBuf,
	lenBytes/(sizeof(short)*sonicGetNumChannels(stream)));
    bytesRead = samplesRead*sizeof(short)*sonicGetNumChannels(stream); 
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Returning %d", samplesRead);
     (*env)->SetByteArrayRegion(env, ret, 0, bytesRead, (jbyte *)outBuf);
    return bytesRead;
}

// Set pitch in sound alteration object
void Java_org_vinuxproject_sonic_SonicAudio_setPitch(
    JNIEnv *env,
    jobject thiz,
    jfloat newPitch)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Set pitch to %f", newPitch);
    sonicSetPitch(stream, newPitch);
}

// Get the current pitch.
jfloat Java_org_vinuxproject_sonic_SonicAudio_getPitch(
    JNIEnv *env,
    jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading pitch");
    return sonicGetPitch(stream);
}

// Speed up the sound and increase the pitch, or slow down the sound and
// decrease the pitch.
void Java_org_vinuxproject_sonic_SonicAudio_setRate(
    JNIEnv *env,
    jobject thiz,
    jfloat newRate)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Set rate to %f", newRate);
    sonicSetRate(stream, newRate);
}

// Return the current playback rate.
jfloat Java_org_vinuxproject_sonic_SonicAudio_getRate(
    JNIEnv *env,
    jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading rate");
    return sonicGetRate(stream);
}

// Get the current speed.
jint Java_org_vinuxproject_sonic_SonicAudio_getSpeed(
    JNIEnv *env,
    jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading speed");
    return sonicGetSpeed(stream);
}

// Change the speed.
void Java_org_vinuxproject_sonic_SonicAudio_setSpeed(
    JNIEnv *env,
    jobject thiz,
    jfloat newSpeed)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Set speed to %f", newSpeed);
    sonicSetSpeed(stream, newSpeed);
}

// Get the current chord pitch setting.
jboolean Java_org_vinuxproject_sonic_SonicAudio_getChordPitch(
    JNIEnv *env,
    jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading chord pitch");
    return sonicGetChordPitch(stream);
}

// Set chord pitch mode on or off.  Default is off.
void Java_org_vinuxproject_sonic_SonicAudio_setChordPitch(
    JNIEnv *env,
    jobject thiz,
    jboolean useChordPitch)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Set chord pitch to %d", useChordPitch);
    sonicSetChordPitch(stream, useChordPitch);
}

// Returns the number of bytes that can be read from the speed alteration
// object
jint Java_org_vinuxproject_sonic_SonicAudio_availableBytes(
    JNIEnv *env,
    jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading samples available");

    return sonicSamplesAvailable(stream)*sizeof(short)*sonicGetNumChannels(stream);
}

// Process any samples still in a sonic buffer.
void Java_org_vinuxproject_sonic_SonicAudio_flush(
    JNIEnv *env,
    jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Flushing stream");
    sonicFlushStream(stream);
}

// Teardown the C data structure.
void Java_org_vinuxproject_sonic_SonicAudio_close(
    JNIEnv *env,
    jobject thiz)
{
    if(stream != 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Destroying stream");
        sonicDestroyStream(stream);
        free(outBuf);
        outBuf = 0;
        outBufSize = 0;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Stream already destroyed");
    }
    stream = 0;
}
