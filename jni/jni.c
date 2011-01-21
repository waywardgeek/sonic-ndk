#include <jni.h>
#include "sonic.h"

jstring
Java_org_vinuxproject_sonic_Sonic_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from JNI !");
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
	// We know that the user will have passed in lenBytes in jbyte, but putSamples needs # samples in SAMPLETYPE
	// TODO: Push bytes into speed alteration object buffer.
	// I believe they'll be shorts.
    
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
	// We know that the user will have passed in lenBytes in jbyte, but putSamples needs # samples in SAMPLETYPE
	// TODO: Read bytes into ret buffer.  I believe they'll be shorts.

    bytesRead = sonicReadShortFromStream(stream, (short *)(void *)ret,
	lenBytes/(sizeof(short)*sonicGetNumChannels(stream)));
    return bytesRead; 
}

// I pass the number of channels to init.  Is there a reason to have the
// following method?

// Set number of chanels used in sound alteration object
// 1 = mono, 2 = stereo.  In practice nothing else has mattered.
/*
void Java_org_vinuxproject_Sonic_setChannels(
    JNIEnv env,
    jobject thiz,
    jint channels)
{
    jclass clazz = env->FindClass("org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(clazz, "ptr", "J");
    sonicStream *stream = (sonicStream)env->GetIntField(thiz, fid);

	// TODO: Set channels in sound alteration object
}
*/

// Question: what is the "C" for at the end of methods?  I've removed them, but
// they probably belong there.

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

// Set pitch in sound alteration object
jfloat Java_org_vinuxproject_Sonic_getPitch(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(&env, thiz, fid);

	// TODO: Set pitch in sound alteration object
    return sonicGetPitch(sonic);
}

// Sample rate is also set in the constructor.  Is this method required?

// Set sample rate of input data.  Usually this will be 44100,
// but other numbers are common enough that I've fixed bugs caused
// by a sample rate of 22050
/*
void Java_org_vinuxproject_Sonic_setSampleRate(
    JNIEnv env,
    jobject thiz,
    jint sampleRate)
{
    jclass clazz = env->FindClass("org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(thiz, fid);

	// TODO: Set input sample rate in the sound alteration object
}
*/

// I can add the following method easily enough, but it's not supported yet.
// While I'm at it, I could add sample rate changing functions.

// Speed up the sound and increase the pitch, or slow down the sound and
// decrease the pitch
/*
void Java_org_vinuxproject_Sonic_setRate(
    JNIEnv env,
    jobject thiz,
    jfloat newRate)
{
    jclass clazz = env->FindClass("org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(thiz, fid);

	// TODO: Set the speed and pitch of the speed alteration object
}
*/

// Instead of "tempo", I used "speed".  Tempo is too closely associated with
// music, and it makes perfect sense with WSOLA, but sonic butchers music.

// Get the current speed.
jint Java_org_vinuxproject_Sonic_getSpeed(
    JNIEnv env,
    jobject thiz)
{
    jclass clazz = env->FindClass(&env, "org/vinuxproject/Sonic");
    jfieldID fid = env->GetFieldID(&env, clazz, "ptr", "J");
    sonicStream sonic = (sonicStream) env->GetIntField(&env, thiz, fid);

	// TODO: Set the speed of the speed alteration object
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

	// TODO: Set the speed of the speed alteration object
    sonicSetSpeed(sonic, newSpeed);
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

	// TODO: Return the number of bytes available from the speed
	// alteration object
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
