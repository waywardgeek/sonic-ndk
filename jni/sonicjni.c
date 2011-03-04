#include <jni.h>
#include <android/log.h>
#include <stdlib.h>
#include "sonic.h"

// For debug messages:
#if SONIC_DEBUG
#define APPNAME "Sonic"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, __VA_ARGS__);
#else
#define LOGV(...)
#endif

static sonicStream stream = 0;
static short *byteBuf;
static int byteBufSize;

/* Initialize the C data structure */
void Java_org_vinuxproject_sonic_Sonic_init(
    JNIEnv *env,
    jobject thiz,
    jint sampleRate,
    jint channels)
{
    LOGV("Creating sonic stream");
    stream = sonicCreateStream(sampleRate, channels);
    byteBufSize = 100;
    byteBuf = (short *)calloc(byteBufSize, sizeof(short));
}

/* Put bytes into the input buffer of the sound alteration object
   lenBytes bytes will be read from buffer into the sound alteration object
   buffer is not guaranteed not to change after this function is called,
   so data should be copied from it */
void Java_org_vinuxproject_sonic_Sonic_putBytes(
    JNIEnv *env,
    jobject thiz,
    jbyteArray buffer,
    jint lenBytes)
{
    int samples = lenBytes/(sizeof(short)*sonicGetNumChannels(stream));
    int remainingBytes = lenBytes - samples*sizeof(short)*sonicGetNumChannels(stream);

// TODO: deal with case where remainingBytes is not 0.
if(remainingBytes != 0) {
    LOGV("Remaining bytes == %d!!!", remainingBytes);
}
    if(lenBytes > byteBufSize*sizeof(short)) {
        byteBufSize = lenBytes*(2/sizeof(short));
        byteBuf = (short *)realloc(byteBuf, byteBufSize*sizeof(short));
    }
    LOGV("Writing %d bytes to stream", lenBytes);
    (*env)->GetByteArrayRegion(env, buffer, 0, lenBytes, (jbyte *)byteBuf);
    sonicWriteShortToStream(stream, byteBuf, samples);
}

// Get bytes representing sped up/slowed down sound and put up to lenBytes
// into ret.
// Returns number of bytes read.
jint Java_org_vinuxproject_sonic_Sonic_receiveBytes(
    JNIEnv *env,
    jobject thiz,
    jbyteArray ret,
    jint lenBytes)
{
    int available = sonicSamplesAvailable(stream)*sizeof(short)*sonicGetNumChannels(stream);
    int samplesRead, bytesRead;

    LOGV("Reading %d bytes from stream", lenBytes);
    if(lenBytes > available) {
        lenBytes = available;
    }
    if(lenBytes > byteBufSize*sizeof(short)) {
        byteBufSize = lenBytes*(2/sizeof(short));
        byteBuf = (short *)realloc(byteBuf, byteBufSize*sizeof(short));
    }
    //LOGV("Doing read %d", lenBytes);
    samplesRead = sonicReadShortFromStream(stream, byteBuf,
	lenBytes/(sizeof(short)*sonicGetNumChannels(stream)));
    bytesRead = samplesRead*sizeof(short)*sonicGetNumChannels(stream); 
    //LOGV("Returning %d", samplesRead);
    (*env)->SetByteArrayRegion(env, ret, 0, bytesRead, (jbyte *)byteBuf);
    return bytesRead;
}

// Set pitch in sound alteration object
void Java_org_vinuxproject_sonic_Sonic_setPitch(
    JNIEnv *env,
    jobject thiz,
    jfloat newPitch)
{
    LOGV("Set pitch to %f", newPitch);
    sonicSetPitch(stream, newPitch);
}

// Get the current pitch.
jfloat Java_org_vinuxproject_sonic_Sonic_getPitch(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading pitch");
    return sonicGetPitch(stream);
}

// Speed up the sound and increase the pitch, or slow down the sound and
// decrease the pitch.
void Java_org_vinuxproject_sonic_Sonic_setRate(
    JNIEnv *env,
    jobject thiz,
    jfloat newRate)
{
    LOGV("Set rate to %f", newRate);
    sonicSetRate(stream, newRate);
}

// Return the current playback rate.
jfloat Java_org_vinuxproject_sonic_Sonic_getRate(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading rate");
    return sonicGetRate(stream);
}

// Get the current sample rate.
jint Java_org_vinuxproject_sonic_Sonic_getSampleRate(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading sample rate");
    return sonicGetSampleRate(stream);
}

// Set the sample rate.
void Java_org_vinuxproject_sonic_Sonic_setSampleRate(
    JNIEnv *env,
    jobject thiz,
    jint newSampleRate)
{
    LOGV("Set sample rate to %d", newSampleRate);
    sonicSetSampleRate(stream, newSampleRate);
}

// Get the current number of channels.
jint Java_org_vinuxproject_sonic_Sonic_getNumChannels(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading num channels");
    return sonicGetNumChannels(stream);
}

// Set the number of channels.
void Java_org_vinuxproject_sonic_Sonic_setNumChannels(
    JNIEnv *env,
    jobject thiz,
    jint newNumChannels)
{
    LOGV("Set sample rate to %d", newNumChannels);
    sonicSetNumChannels(stream, newNumChannels);
}

// Get the current speed.
jint Java_org_vinuxproject_sonic_Sonic_getSpeed(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading speed");
    return sonicGetSpeed(stream);
}

// Change the speed.
void Java_org_vinuxproject_sonic_Sonic_setSpeed(
    JNIEnv *env,
    jobject thiz,
    jfloat newSpeed)
{
    LOGV("Set speed to %f", newSpeed);
    sonicSetSpeed(stream, newSpeed);
}

// Get the current chord pitch setting.
jboolean Java_org_vinuxproject_sonic_Sonic_getChordPitch(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading chord pitch");
    return sonicGetChordPitch(stream);
}

// Set chord pitch mode on or off.  Default is off.
void Java_org_vinuxproject_sonic_Sonic_setChordPitch(
    JNIEnv *env,
    jobject thiz,
    jboolean useChordPitch)
{
    LOGV("Set chord pitch to %d", useChordPitch);
    sonicSetChordPitch(stream, useChordPitch);
}

// Returns the number of bytes that can be read from the speed alteration
// object
jint Java_org_vinuxproject_sonic_Sonic_availableBytes(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Reading samples available = %d", sonicSamplesAvailable(stream)*sizeof(short)*sonicGetNumChannels(stream));

    return sonicSamplesAvailable(stream)*sizeof(short)*sonicGetNumChannels(stream);
}

// Process any samples still in a sonic buffer.
void Java_org_vinuxproject_sonic_Sonic_flush(
    JNIEnv *env,
    jobject thiz)
{
    LOGV("Flushing stream");
    sonicFlushStream(stream);
}

// Teardown the C data structure.
void Java_org_vinuxproject_sonic_Sonic_close(
    JNIEnv *env,
    jobject thiz)
{
    if(stream != 0) {
        LOGV("Destroying stream");
        sonicDestroyStream(stream);
        free(byteBuf);
        byteBuf = 0;
        byteBufSize = 0;
    } else {
        LOGV("Stream already destroyed");
    }
    stream = 0;
}
