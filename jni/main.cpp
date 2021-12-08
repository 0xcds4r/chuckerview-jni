#include "main.h"
#include "bot.h"

extern "C"
JNIEXPORT jboolean JNICALL
Java_net_kentandcds4r_chuckerview_MainActivity_doExecuteHTTPRequest(JNIEnv *env, jobject thiz, jstring url, jint type, jboolean b_use_proxy, jstring host, jint port)
{
    const char *nativeString_URL = env->GetStringUTFChars(url, 0);
    const char *nativeString_HOST = env->GetStringUTFChars(host, 0);

    bool bResult = ChuckerViewBot::doExecuteHTTPRequest(nativeString_URL, (int)type, (bool)b_use_proxy, nativeString_HOST, (int)port);
    //LOG("Java_net_kentandcds4r_chuckerview_MainActivity_doExecuteHTTPRequest");

    env->ReleaseStringUTFChars(url, nativeString_URL);
    env->ReleaseStringUTFChars(host, nativeString_HOST);

    return bResult;
}

uintptr_t g_ChuckerMainLib = 0;

void initialize_plugin()
{
	initialize_hooks();
	initialize_patches();
}

void* plugin_thread_func(void *p)
{
	LOG("trying to initialize chucker-patch..");

	while(!g_ChuckerMainLib) 
	{
		LOG("initializing libraries..");
		g_ChuckerMainLib = searchLibrary("libchuckerview.so");
		if(!g_ChuckerMainLib) LOG("[libchuckerview.so] initialization failed! Retrying..");
		usleep(5);
	}

	LOG("[libchuckerview.so] Base Address -> 0x%X", g_ChuckerMainLib);

	initialize_plugin();
	LOG("chucker-patch initialized!");

	pthread_exit(0);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	LOG("chucker-patch injected!");

	pthread_t plugin_thread;
	pthread_create(&plugin_thread, 0, plugin_thread_func, 0);
	
	return JNI_VERSION_1_4;
}

uintptr_t getChuckerLib() {
	return g_ChuckerMainLib;
}