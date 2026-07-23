#define PACKAGE_NAME "com.netease.newspike"
#include "utils/obfuscate.h"
#include "utils/dlfcn.h"
#include "utils/features.h"
#include "app/includes.h"
#include "And64InlineHook/And64InlineHook.hpp"
#include "xhook/xhook.h"
#include "KittyMemory/MemoryPatch.h"
#include <thread>
#include <pthread.h>
#include <jni.h>
#include <dlfcn.h>
#include <netdb.h>
#include <android/log.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "hooks/firewall.h"
#include "app/block.h"
#include <sys/socket.h>
#include <cstring>
#include <array>


static const char* BLACKLIST[] = {
    "analytics.bloodstrike.netease.com",
    "bamreport.bloodstrike.netease.com",
    "reportban.netease.com",
    "ban.netease.com",
    "matchreport.bloodstrike.netease.com",
    "securitycheck.netease.com",
    "accountsecurity.netease.com",
    "userlog.bloodstrike.netease.com",
    "easebar.com",
    "monitor.easebar.com",
    "data.easebar.com",
    "report.easebar.com",
    "analytics.easebar.com",
    "track.easebar.com",
    "stat.easebar.com",
    "mgbsdknaeast.matrix.easebar.com",
    "android.googleapis.com",
    "prod-lt-playstoregatewayadapter-pa.googleapis.com",
    "anticheat.bloodstrike.net",
    "anticheat-api.bloodstrike.com",
    "qsec.netease.com",
    "tinfo.netease.com",
    "log.qos.netease.com",
    "report.bloodstrike.net",
    "report-api.bloodstrike.io",
    "report-api.bloodstrike.net",
    "appeal-api.bloodstrike.io",
    "log.bloodstrike.net",
    "track.bloodstrike.io",
    "log-collector.bloodstrike.com",
    "stat-api.bloodstrike.net",
    "update-api.bloodstrike.io",
    "config.bloodstrike.io",
    "crashlyticsreports-pa.googleapis.com",
    "protocol.unisdk.easebar.com",
    "translate.mpsdk.easebar.com",
    "g0.gsf.easebar.com",
    "sdkgate.pushv3.easebar.com",
    "who.easebar.com",
    "drpf-g83naxx1ena.proxima.nie.easebar.com",
    "data-detect.nie.easebar.com",
    "s.q.easebar.com",
    "optsdk.gameyw.easebar.com",
    "nos.gameyw.easebar.com",
    "t.appsflyer.com",
    "api.access.umeng.com",
    "crashlytics.com",
    "reports.crashlytics.com",
    "app-measurement.com",
    "events.appsflyer.com",
    "log-api.dragonflygames",
    "log-tracker.dragonflygames.cn",
    "tracking.netease.com",
	"log.network.bloodstrike.com",
	"reqhfg-cdn-settings.appsflyersdk.com",
	"logs.bloodstrike.com",
	"tracking.bloodstrike.com",
	"anti-cheat.bloodstrike.com",
	"h73na-14.gdl.easebar.com",
	"h65na-14.gph.easebar.com",
	"g83-microservice-testing.nie.easebar.com",
	"g29-114.gdl.easebar.com",
	"g138-14.gdl.easebar.com",
	"g112na-pre.gdl.easebar.com",
	"g108naxx2gb-14.update.easebar.com",
	"g108naxx2gb-14.gsf.easebar.com",
	"g108-14.update.easebar.com",
	"g83naxx1ena.appdump.nie.easebar.com",
	"sdkgate.pushv3.easebar.com",
	"auth.unisdk.nie.easebar.com",
	"arghus-es-jp.matrix.easebar.com",
	"apps-aws-l32euro.danlu.easebar.com",
    "aliothjp-gcp.nie.easebar.com",
    "l33sg-gamerecord-sg.l33naobj.leihuo.easebar.com",
    "drpf-g37gb.proxima.nie.easebar.com",
    "cc.res.easebar.com",
    "g92naxx2gb-12.gph.easebar.com",
    "pharos.netease.com",
    "data-detect.nie.easebar.com",
    "anticheat.on.midnight.netease.com",
    "com.netease.newspike",
    "split_config.arm64_v8a.apk",
    "app.metadata",
    "base.digests",
    "base.dm",
};

static constexpr size_t BLACKLIST_SIZE = sizeof(BLACKLIST)/sizeof(BLACKLIST[0]);

// Fungsi asli
static int (*orig_getaddrinfo)(const char *node, const char *service,
                               const struct addrinfo *hints,
                               struct addrinfo **res) = nullptr;

static bool is_blacklisted(const char *node) {
    if (!node) return false;
    for (size_t i = 0; i < BLACKLIST_SIZE; ++i) {
        if (strcasecmp(node, BLACKLIST[i]) == 0)
            return true;
    }
    return false;
}


int hooked_getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res) {
    if (is_blacklisted(node)) {
        return EAI_NONAME; 
    }
    return orig_getaddrinfo(node, service, hints, res);
}

void install_domain_blocker() {
    void *libc = dlopen("libc.so", RTLD_NOW);
    if (!libc) return;
    void *sym = dlsym(libc, "getaddrinfo");
    if (sym) {
        A64HookFunction(sym, (void*)hooked_getaddrinfo, (void**)&orig_getaddrinfo);
    }
}

static bool isTouching = false;
static float touchX = 0.0f, touchY = 0.0f;

#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "FPS_FIX", __VA_ARGS__)

DEFINES(void*, _v07, const char *filename, int flags) { return nullptr; }
DEFINES(void*, _xv07, const char *filename, int flags) { return nullptr; }

static int iLoaded = 0, iLoadAt = 2;
extern const char *srcpy;
DEFINES(void*, iMissMyWife, const char* v0, int v1, void* v2, void* v3, void* v4) {
    static bool loaded = false;
    if (v1 == 257) { iLoaded ++;
        if (iLoaded >= iLoadAt && !loaded) {
            loaded = true;
            _iMissMyWife(srcpy, 257, v2, v2, NULL);
        }
    }
    
    return _iMissMyWife(v0, v1, v2, v3, v4);
}



#include "utils/draw.h"
#include "utils/input.h"
#include <pthread.h>
#include <unistd.h>

void* hack_thread(void*) {
	sleep(3);
    void* libGame_handle = fdlopen(oxorany("libGame.so"), 0);
    if (!libGame_handle) return 0;
    auto libGame = absoluteAddress(oxorany("libGame.so"));
    if (!libGame) return 0;
  
    create_directory_recursive(xorstr_("/data/data/" PACKAGE_NAME "/imgui"));

    HOOKI((ptr) libGame + 0x471A918, iMissMyWife);
    

    __INPUT__();
    start_entity_reader();
    HOOKIT(xorstr_("libEGL.so"), xorstr_("eglSwapBuffers"), Draw);
    return 0;
}

__attribute__((constructor))
void lib_main() {
    
    std::thread(hack_thread, nullptr).detach();
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    jvm = vm;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    return JNI_VERSION_1_6;
}

