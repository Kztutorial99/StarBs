#pragma once

#include "jni_stuff.h"
#include "enc/StrEnc.h"
#include "enc/json.hpp"
#include "enc/md5.h"
#include "Oxorany/oxorany.h"
#include "curl/curl-android-arm64-v8a/include/curl/curl.h"

JavaVM *jvm;

using json = nlohmann::json;
std::string g_Token, g_Auth;
bool bValid = false;
std::string EXP = " ";
std::string mod_status = " ";
std::string DeviceID;
std::string DeviceModel = " ";
std::string DeviceBrand;
std::string GameVersion = "Unknown";

std::string GetAndroidID(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID getContentResolverMethod = env->GetMethodID(contextClass,OBFUSCATE("getContentResolver"),OBFUSCATE("()Landroid/content/ContentResolver;"));
    jclass settingSecureClass = env->FindClass(OBFUSCATE("android/provider/Settings$Secure"));
    jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass,OBFUSCATE("getString"), OBFUSCATE("(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;"));

    jstring androidIdStr = env->NewStringUTF(OBFUSCATE("android_id"));
    auto obj = env->CallObjectMethod(context, getContentResolverMethod);
    auto str = (jstring) env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj, androidIdStr);

    std::string result = "Unknown";
    if (str) {
        const char *chars = env->GetStringUTFChars(str, 0);
        result = chars;
        env->ReleaseStringUTFChars(str, chars);
        env->DeleteLocalRef(str);
    }

    env->DeleteLocalRef(androidIdStr);
    env->DeleteLocalRef(obj);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(settingSecureClass);
    return result;
}

std::string GetDeviceModel(JNIEnv *env) {
    jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("MODEL"),OBFUSCATE("Ljava/lang/String;"));

    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    std::string result = "Unknown";
    if (str) {
        const char *chars = env->GetStringUTFChars(str, 0);
        result = chars;
        env->ReleaseStringUTFChars(str, chars);
        env->DeleteLocalRef(str);
    }
    env->DeleteLocalRef(buildClass);
    return result;
}

std::string GetDeviceBrand(JNIEnv *env) {
    jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("BRAND"),OBFUSCATE("Ljava/lang/String;"));

    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    std::string result = "Unknown";
    if (str) {
        const char *chars = env->GetStringUTFChars(str, 0);
        result = chars;
        env->ReleaseStringUTFChars(str, chars);
        env->DeleteLocalRef(str);
    }
    env->DeleteLocalRef(buildClass);
    return result;
}

std::string GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid) {
    jclass uuidClass = env->FindClass(OBFUSCATE("java/util/UUID"));

    auto len = strlen(uuid);

    jbyteArray myJByteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *) uuid);

    jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass,OBFUSCATE("nameUUIDFromBytes"),OBFUSCATE("([B)Ljava/util/UUID;"));
    jmethodID toStringMethod = env->GetMethodID(uuidClass, OBFUSCATE("toString"),OBFUSCATE("()Ljava/lang/String;"));

    auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
    auto str = (jstring) env->CallObjectMethod(obj, toStringMethod);

    std::string result = "Unknown";
    if (str) {
        const char *chars = env->GetStringUTFChars(str, 0);
        result = chars;
        env->ReleaseStringUTFChars(str, chars);
        env->DeleteLocalRef(str);
    }

    env->DeleteLocalRef(obj);
    env->DeleteLocalRef(myJByteArray);
    env->DeleteLocalRef(uuidClass);
    return result;
}

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *) userp;
	
	mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		return 0;
	}
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return realsize;
}

int ShowSoftKeyboardInput() {
	jint result;
	jint flags = 0;
	
	JNIEnv *env;
	jvm->AttachCurrentThread(&env, NULL);
	
	jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
	auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
	env->CallStaticVoidMethod(looperClass, prepareMethod);
	
	jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
	jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
	jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
	
	jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
	jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
	
	jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
	jfieldID fieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(contextClass, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
	jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(contextClass, fieldINPUT_METHOD_SERVICE);
	jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
	jobject callObjectMethod = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, INPUT_METHOD_SERVICE);
	
	jclass classInputMethodManager = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = env->GetMethodID(classInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
	
	if (result) {
		env->CallVoidMethod(callObjectMethod, toggleSoftInputId, 2, flags);
	} else {
		env->CallVoidMethod(callObjectMethod, toggleSoftInputId, flags, flags);
	}
	
	env->DeleteLocalRef(classInputMethodManager);
	env->DeleteLocalRef(callObjectMethod);
	env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
	jvm->DetachCurrentThread();
	
	return result;
}

int PollUnicodeChars() {
	JNIEnv *env;
	jvm->AttachCurrentThread(&env, NULL);
	
	jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
	auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
	env->CallStaticVoidMethod(looperClass, prepareMethod);
	
	jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
	jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
	jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
	
	jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
	jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
	
	jclass keyEventClass = env->FindClass(OBFUSCATE("android/view/KeyEvent"));
	jmethodID getUnicodeCharMethod = env->GetMethodID(keyEventClass, OBFUSCATE("getUnicodeChar"), OBFUSCATE("(I)I"));
	
	ImGuiIO& io = ImGui::GetIO();
    
	int return_key = env->CallIntMethod(keyEventClass, getUnicodeCharMethod);
	
	env->DeleteLocalRef(keyEventClass);
	env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
	jvm->DetachCurrentThread();
	
	return return_key;
}

std::string getClipboard() {
    std::string result;
    JNIEnv *env;

    jvm->AttachCurrentThread(&env, NULL);

    auto looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"),
                                                OBFUSCATE("()V"));
    env->CallStaticVoidMethod(looperClass, prepareMethod);

    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE(
            "sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass,
                                                               sCurrentActivityThreadField);

    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass,
                                                        OBFUSCATE("mInitialApplication"),
                                                        OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread,
                                                      mInitialApplicationField);

    auto contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    auto getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"),
                                                   OBFUSCATE(
                                                           "(Ljava/lang/String;)Ljava/lang/Object;"));

    auto str = env->NewStringUTF(OBFUSCATE("clipboard"));
    auto clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    env->DeleteLocalRef(str);

    jclass ClipboardManagerClass = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    auto getText = env->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"),
                                    OBFUSCATE("()Ljava/lang/CharSequence;"));

    jclass CharSequenceClass = env->FindClass(OBFUSCATE("java/lang/CharSequence"));
    auto toStringMethod = env->GetMethodID(CharSequenceClass, OBFUSCATE("toString"),
                                           OBFUSCATE("()Ljava/lang/String;"));

    auto text = env->CallObjectMethod(clipboardManager, getText);
    if (text) {
        str = (jstring) env->CallObjectMethod(text, toStringMethod);
        result = env->GetStringUTFChars(str, 0);
        env->DeleteLocalRef(str);
        env->DeleteLocalRef(text);
    }

    env->DeleteLocalRef(CharSequenceClass);
    env->DeleteLocalRef(ClipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    jvm->DetachCurrentThread();
    return result.c_str();
}



void UpdateGameVersion() {
    JNIEnv *env;
    bool attached = false;
    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        if (jvm->AttachCurrentThread(&env, NULL) != 0) return;
        attached = true;
    }

    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThreadMethod = env->GetStaticMethodID(activityThreadClass, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->CallStaticObjectMethod(activityThreadClass, currentActivityThreadMethod);
    
    if (!sCurrentActivityThread) {
        jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
        sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    }

    if (sCurrentActivityThread) {
        jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
        jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);

        if (mInitialApplication) {
            jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
            jmethodID getPackageManagerMethod = env->GetMethodID(contextClass, OBFUSCATE("getPackageManager"), OBFUSCATE("()Landroid/content/pm/PackageManager;"));
            jmethodID getPackageNameMethod = env->GetMethodID(contextClass, OBFUSCATE("getPackageName"), OBFUSCATE("()Ljava/lang/String;"));
            jobject packageManager = env->CallObjectMethod(mInitialApplication, getPackageManagerMethod);
            jstring packageName = (jstring) env->CallObjectMethod(mInitialApplication, getPackageNameMethod);

            jclass packageManagerClass = env->FindClass(OBFUSCATE("android/content/pm/PackageManager"));
            jmethodID getPackageInfoMethod = env->GetMethodID(packageManagerClass, OBFUSCATE("getPackageInfo"), OBFUSCATE("(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;"));
            jobject packageInfo = env->CallObjectMethod(packageManager, getPackageInfoMethod, packageName, 0);

            if (packageInfo) {
                jclass packageInfoClass = env->FindClass(OBFUSCATE("android/content/pm/PackageInfo"));
                jfieldID versionNameField = env->GetFieldID(packageInfoClass, OBFUSCATE("versionName"), OBFUSCATE("Ljava/lang/String;"));
                jstring versionName = (jstring) env->GetObjectField(packageInfo, versionNameField);
                if (versionName) {
                    const char *vName = env->GetStringUTFChars(versionName, 0);
                    GameVersion = vName;
                    env->ReleaseStringUTFChars(versionName, vName);
                }
            }
        }
    }

    if (attached) jvm->DetachCurrentThread();
}

std::string Login(const char *user_key) {
    JNIEnv *env;
    bool attached = false;
    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        if (jvm->AttachCurrentThread(&env, NULL) != 0) return "JNI Attach Failed";
        attached = true;
    }
    
    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThreadMethod = env->GetStaticMethodID(activityThreadClass, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->CallStaticObjectMethod(activityThreadClass, currentActivityThreadMethod);

    if (!sCurrentActivityThread) {
        jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
        sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    }
    
    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
    
    std::string hwid_raw = user_key;
    hwid_raw += GetAndroidID(env, mInitialApplication);
    hwid_raw += GetDeviceModel(env);
    hwid_raw += GetDeviceBrand(env);

    std::string UUID = GetDeviceUniqueIdentifier(env, hwid_raw.c_str());
    DeviceBrand = GetDeviceBrand(env);
    DeviceModel = GetDeviceModel(env);
    DeviceID = GetAndroidID(env, mInitialApplication);

    UpdateGameVersion();

    std::string errMsg;
    struct MemoryStruct chunk{};
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    CURL *curl = curl_easy_init();
    if (curl) {
        std::string api_key = oxorany("https://starcool.my.id/connect");
        curl_easy_setopt(curl, CURLOPT_URL, (api_key.c_str()));
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, oxorany("Content-Type: application/x-www-form-urlencoded"));
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        char data[4096];
        sprintf(data, oxorany("game=BS&user_key=%s&serial=%s"), user_key, UUID.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            try {
                json result = json::parse(chunk.memory);
                if (result[std::string(OBFUSCATE("status"))] == true) {
                    std::string token = result[std::string(OBFUSCATE("data"))][std::string(OBFUSCATE("token"))].get<std::string>();
                    time_t rng = result[std::string(OBFUSCATE("data"))][std::string(OBFUSCATE("rng"))].get<time_t>();
                    
                    EXP = result["data"]["EXP"].get<std::string>();
                    mod_status = result["data"]["mod_status"].get<std::string>();
                    
                    if (rng + 30 > time(0)) {
                        std::string auth = "BS";
                        auth += "-";
                        auth += user_key;
                        auth += "-";
                        auth += hwid_raw;
                        auth += "-";        
                        auth += "Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E";

                        g_Token = token;
                        g_Auth = auth;
                        bValid = (g_Token == g_Auth);
                   }
                } else {
                    errMsg = result[std::string(oxorany("reason"))].get<std::string>();
                }
            } catch (json::exception &e) {
                errMsg = "JSON Parse Error";
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
        curl_easy_cleanup(curl);
        if (headers) curl_slist_free_all(headers);
    }

    if (chunk.memory) free(chunk.memory);
    if (attached) jvm->DetachCurrentThread();
    return bValid ? "OK" : (errMsg.empty() ? "Unknown Error" : errMsg);
}





/*
   
if (curl) {
    std::string api_key = oxorany("https://pastebin.com/raw/Gi5BLpAR");
    curl_easy_setopt(curl, CURLOPT_URL, api_key.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        try {
            json result = json::parse(chunk.memory);
            auto keys = result["keys"];
            if (keys.contains(user_key)) {
                std::string exp_date = keys[user_key];
                
                // Check expiry
                time_t now = time(0);
                struct tm tm_exp = {};
                strptime(exp_date.c_str(), "%Y-%m-%d", &tm_exp);
                time_t t_exp = mktime(&tm_exp);
                
                if (t_exp >= now) {
                    bValid = true;
                } else {
                    errMsg = "KEY EXPIRED BUY @EXSTARCOOL";
                }
            } else {
                errMsg = "KEY ERROR BUY @EXSTARCOOL";
            }
        } catch (json::exception &e) {
            errMsg = "{";
            errMsg += e.what();
            errMsg += "}\n{";
            errMsg += chunk.memory;
            errMsg += "}";
        }
    } else {
        errMsg = curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
}
jvm->DetachCurrentThread();
return bValid ? "OK" : errMsg;
}*/
