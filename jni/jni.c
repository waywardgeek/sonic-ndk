#include <jni.h>
#include "sonic.h"

jstring
Java_org_vinuxproject_sonic_Sonic_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from Sonic!");
}

/* Initialize the C data structure */
void Java_org_vinuxproject_Sonic_init(
    JNIEnv env,
    jobject thiz,
    jint sampleRate,
    jint channels)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    sonicStream stream = sonicCreateStream(sampleRate, channels);
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");

    env->SetIntField(&env, thiz, fid, (int)(void *)stream);
}

/* Put bytes into the input buffer of the sound alteration object
   lenBytes bytes will be read from buffer into the sound alteration object
   buffer is not guaranteed not to change after this function is called,
   so data should be copied from it */
void Java_org_vinuxproject_Sonic_putBytes(
    JNIEnv env,
    jobject thiz,
    jbyteArray buffer,
    jint lenBytes)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream stream = (sonicStream)env->GetIntField(&env, thiz, fid);
    
    sonicWriteShortToStream(stream, (short *)(void *)buffer,
        lenBytes/(sizeof(short)*sonicGetNumChannels(stream)));
}

// Get bytes representing sped up/slowed down sound and put up to lenBytes
// into ret.
// Returns number of bytes read.
jint Java_org_vinuxproject_Sonic_receiveBytes(
    JNIEnv env,
    jobject thiz,
    jbyteArray ret,
    jint lenBytes)
{
    int bytesRead;

    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream stream = (sonicStream)env->GetIntField(&env, thiz, fid);

    bytesRead = sonicReadShortFromStream(stream, (short *)(void *)ret,
	lenBytes/(sizeof(short)*sonicGetNumChannels(stream)));
    return bytesRead; 
}

// Set pitch in sound alteration object
void Java_org_vinuxproject_Sonic_setPitch(
    JNIEnv env,
    jobject thiz,
    jfloat newPitch)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(&env, thiz, fid);

	// TODO: Set pitch in sound alteration object
    sonicSetPitch(sonic, newPitch);
}

// Get the current pitch.
jfloat Java_org_vinuxproject_Sonic_getPitch(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream)env->GetIntField(&env, thiz, fid);

    return sonicGetPitch(sonic);
}

// Speed up the sound and increase the pitch, or slow down the sound and
// decrease the pitch.
void Java_org_vinuxproject_Sonic_setRate(
    JNIEnv env,
    jobject thiz,
    jfloat newRate)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream)env->GetIntField(&env, thiz, fid);

    sonicSetRate(sonic, newRate);
}

// Return the current playback rate.
jfloat Java_org_vinuxproject_Sonic_getRate(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream)env->GetIntField(&env, thiz, fid);

    return sonicGetRate(sonic);
}

// Get the current speed.
jint Java_org_vinuxproject_Sonic_getSpeed(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream)env->GetIntField(&env, thiz, fid);

    return sonicGetSpeed(sonic);
}

// Change the speed.
void Java_org_vinuxproject_Sonic_setSpeed(
    JNIEnv env,
    jobject thiz,
    jfloat newSpeed)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(&env, thiz, fid);

    sonicSetSpeed(sonic, newSpeed);
}

// Get the current chord pitch setting.
jboolean Java_org_vinuxproject_Sonic_getChordPitch(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(&env, thiz, fid);

    return sonicGetChordPitch(sonic);
}

// Set chord pitch mode on or off.  Default is off.
void Java_org_vinuxproject_Sonic_setChordPitch(
    JNIEnv env,
    jobject thiz,
    jboolean useChordPitch)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(&env, thiz, fid);

   sonicSetChordPitch(sonic, useChordPitch);
}

// Returns the number of bytes that can be read from the speed alteration
// object
jint Java_org_vinuxproject_Sonic_availableBytes(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream stream = (sonicStream) env->GetIntField(&env, thiz, fid);

    return sonicSamplesAvailable(stream)*sizeof(short);
}

// Teardown the C data structure.
void Java_org_vinuxproject_Sonic_close(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream stream = (sonicStream) env->GetIntField(&env, thiz, fid);

    sonicDestroyStream(stream);
    env->SetIntField(&env, thiz, fid, 0);
}
