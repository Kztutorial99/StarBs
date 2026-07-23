#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stddef.h>
#include "Dobby/dobby.h" 

// === HOOK fgets ===
static char* (*orig_fgets)(char*, int, FILE*);
char* hooked_fgets(char* buf, int size, FILE* fp) {
    char* res = orig_fgets(buf, size, fp);
    if (res && (strstr(res, "lib") || strstr(res, ".so"))) return nullptr;
    return res;
}

// === HOOK access ===
static int (*orig_access)(const char*, int);
int hooked_access(const char* path, int mode) {
    if (strstr(path, "lib") || strstr(path, ".so")) return -1;
    return orig_access(path, mode);
}

// === HOOK send ===
static ssize_t (*orig_send)(int, const void*, size_t, int);
ssize_t hooked_send(int sockfd, const void* buf, size_t len, int flags) {
    if (strstr((const char*)buf, "lib") || strstr((const char*)buf, "imgui") || strstr((const char*)buf, "hook"))
        return 0;
    return orig_send(sockfd, buf, len, flags);
}

// === Auto run on lib load ===
__attribute__((constructor))
void init_bypass() {
    void* libc = dlopen("libc.so", RTLD_LAZY);
    if (libc) {
        void* sym1 = dlsym(libc, "fgets");
        void* sym2 = dlsym(libc, "access");
        void* sym3 = dlsym(libc, "send");

        if (sym1) DobbyHook(sym1, (void*)hooked_fgets, (void**)&orig_fgets);
        if (sym2) DobbyHook(sym2, (void*)hooked_access, (void**)&orig_access);
        if (sym3) DobbyHook(sym3, (void*)hooked_send, (void**)&orig_send);
    }
}
