#include <jni.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "ApplicationTemplate.hpp"

platform::filesystem_interface *platform::api::filesystem = new posix::filesystem();
platform::graphics_interface *platform::api::graphics = new opengl::graphics();
platform::audio_interface *platform::api::audio = new opensles::audio();
platform::asset_interface *platform::api::asset = new art::asset();

extern "C" {
	JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1startup(JNIEnv * env, jclass cls, jobject java_asset_manager);
	JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1resize(JNIEnv * env, jclass cls, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1draw(JNIEnv* env, jclass cls);
	JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1touch_1press(JNIEnv* env, jclass cls, jfloat normalized_x, jfloat normalized_y);
	JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1touch_1drag(JNIEnv* env, jclass cls, jfloat normalized_x, jfloat normalized_y);
};

JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1startup(JNIEnv * env, jclass cls, jobject java_asset_manager) {

  // ANativeActivity* activity = state->activity;
  // JNIEnv* env=0;

  // (*activity->vm)->AttachCurrentThread(activity->vm, &env, 0);

  // jclass clazz = (*env)->GetObjectClass(env, activity->clazz);
  // jmethodID methodID = (*env)->GetMethodID(env, clazz, "getPackageCodePath", "()Ljava/lang/String;");
  // jobject result = (*env)->CallObjectMethod(env, activity->clazz, methodID);

  // const char* str;
  // jboolean isCopy;
  // str = (*env)->GetStringUTFChars(env, (jstring)result, &isCopy);
  // LOGI("Looked up package code path: %s", str);

  // (*activity->vm)->DetachCurrentThread(activity->vm);
    
/*
  struct engine engine;

  ANativeActivity* activity = state->activity;
  JNIEnv* env = activity->env;

  jclass clazz = (*env)->GetObjectClass(env, activity->clazz);
  jmethodID methodID = (*env)->GetMethodID(env, clazz, "getPackageCodePath", "()Ljava/lang/String;");
  jobject result = (*env)->CallObjectMethod(env, activity->clazz, methodID);

  const char* str;
  jboolean isCopy;
  str = (*env)->GetStringUTFChars(env, (jstring)result, &isCopy);
  LOGI("Looked up package code path: %s", str);
*/
	on_startup((void *)AAssetManager_fromJava(env, java_asset_manager));
}

JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1resize(JNIEnv * env, jclass cls, jint width, jint height) {
	on_resize(width, height);
}

JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1draw(JNIEnv* env, jclass cls) {
	on_draw();
}

JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1touch_1press(JNIEnv* env, jclass cls, jfloat normalized_x, jfloat normalized_y) {
	on_touch_press(normalized_x*100+240, normalized_y*100+320);
}

JNIEXPORT void JNICALL Java_com_android_ApplicationTemplate_ApplicationTemplateInterop_on_1touch_1drag(JNIEnv* env, jclass cls, jfloat normalized_x, jfloat normalized_y) {
	on_touch_drag(normalized_x*100+240, normalized_y*100+320);
}
