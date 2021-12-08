#pragma once

#include <jni.h>
#include <string>
#include <string.h>
#include <android/log.h>
#include <thread>
#include <sys/mman.h>
#include <unistd.h> // system api
#include <sys/mman.h>
#include <assert.h> // assert()
#include <dlfcn.h> // dlopen

#define LOG_TAG "chucker-patch"
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);

uintptr_t searchLibrary(const char* library);
uintptr_t getChuckerLib();
uintptr_t exploreMethod(const char *libname, const char *name);
void installHook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
void initialize_hooks();
void initialize_patches();
void* destruct_level_engine();