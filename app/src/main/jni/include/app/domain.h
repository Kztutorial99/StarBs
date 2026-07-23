/*
#include <android/log.h>
#define LOG_TAG "BLOCK_NET"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dlfcn.h>
#include "Dobby/dobby.h" 
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dlfcn.h>

int blockcount = 100;
bool block_connections = true;

// Original connect function pointer
int (*original_connect)(int, const struct sockaddr *, socklen_t);

// Custom connect function to hook into libc.so
int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    char ip[INET_ADDRSTRLEN];

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        inet_ntop(AF_INET, &addr_in->sin_addr, ip, sizeof(ip));

        // Check blocked IPs if block_connections is enabled
        if (block_connections &&
            (strcmp(ip, "18.161.49.13") == 0 ||
strcmp(ip, "23.198.104.192") == 0 ||
strcmp(ip, "23.210.96.164") == 0 ||
strcmp(ip, "34.124.164.139") == 0 ||
strcmp(ip, "34.124.204.207") == 0 ||
strcmp(ip, "34.126.73.6") == 0 ||
strcmp(ip, "34.142.221.235") == 0 ||
strcmp(ip, "34.142.239.151") == 0 ||
strcmp(ip, "34.143.130.186") == 0 ||
strcmp(ip, "34.143.137.33") == 0 ||
strcmp(ip, "34.143.185.38") == 0 ||
strcmp(ip, "34.143.187.29") == 0 ||
strcmp(ip, "34.143.192.140") == 0 ||
strcmp(ip, "34.143.221.203") == 0 ||
strcmp(ip, "34.87.159.190") == 0 ||
strcmp(ip, "34.87.186.106") == 0 ||
strcmp(ip, "35.198.219.207") == 0 ||
strcmp(ip, "35.198.226.62") == 0 ||
strcmp(ip, "35.240.133.108") == 0 ||
strcmp(ip, "35.240.152.172") == 0 ||
strcmp(ip, "35.240.201.242") == 0 ||
strcmp(ip, "35.240.238.247") == 0 ||
strcmp(ip, "35.72.104.232") == 0 ||
strcmp(ip, "76.223.88.1") == 0||
    strcmp(ip, "34.124.164.139") == 0 ||
    strcmp(ip, "34.124.204.207") == 0 ||
    strcmp(ip, "34.126.130.128") == 0 ||
    strcmp(ip, "34.126.73.6") == 0 ||
    strcmp(ip, "34.142.204.184") == 0 ||
    strcmp(ip, "34.142.221.235") == 0 ||
    strcmp(ip, "34.142.232.214") == 0 ||
    strcmp(ip, "34.142.238.43") == 0 ||
    strcmp(ip, "34.142.239.151") == 0 ||
    strcmp(ip, "34.143.130.186") == 0 ||
    strcmp(ip, "34.143.137.33") == 0 ||
    strcmp(ip, "34.143.168.146") == 0 ||
    strcmp(ip, "34.143.185.38") == 0 ||
    strcmp(ip, "34.143.187.29") == 0 ||
    strcmp(ip, "34.143.192.140") == 0 ||
    strcmp(ip, "34.143.212.135") == 0 ||
    strcmp(ip, "34.143.221.203") == 0 ||
    strcmp(ip, "34.151.196.112") == 0 ||
    strcmp(ip, "34.151.255.59") == 0 ||
    strcmp(ip, "34.174.143.147") == 0 ||
    strcmp(ip, "34.174.16.59") == 0 ||
    strcmp(ip, "34.174.177.15") == 0 ||
    strcmp(ip, "34.174.186.253") == 0 ||
    strcmp(ip, "34.174.202.220") == 0 ||
    strcmp(ip, "34.174.205.51") == 0 ||
    strcmp(ip, "34.174.212.88") == 0 ||
    strcmp(ip, "34.174.249.110") == 0 ||
    strcmp(ip, "34.174.26.149") == 0 ||
    strcmp(ip, "34.174.36.94") == 0 ||
    strcmp(ip, "34.174.52.228") == 0 ||
    strcmp(ip, "34.174.56.217") == 0 ||
    strcmp(ip, "34.174.73.104") == 0 ||
    strcmp(ip, "34.174.81.33") == 0 ||
    strcmp(ip, "34.87.126.219") == 0 ||
    strcmp(ip, "34.87.159.190") == 0 ||
    strcmp(ip, "34.87.186.106") == 0 ||
    strcmp(ip, "34.87.34.223") == 0 ||
    strcmp(ip, "34.87.39.30") == 0 ||
    strcmp(ip, "34.95.171.126") == 0 ||
    strcmp(ip, "34.95.212.120") == 0 ||
    strcmp(ip, "34.95.228.230") == 0 ||
    strcmp(ip, "35.185.180.183") == 0 ||
    strcmp(ip, "35.187.241.189") == 0 ||
    strcmp(ip, "35.197.140.59") == 0 ||
    strcmp(ip, "35.198.10.86") == 0 ||
    strcmp(ip, "35.198.219.207") == 0 ||
    strcmp(ip, "35.198.226.62") == 0 ||
    strcmp(ip, "35.198.34.68") == 0 ||
    strcmp(ip, "35.199.104.76") == 0 ||
    strcmp(ip, "35.240.133.108") == 0 ||
    strcmp(ip, "35.240.152.172") == 0 ||
    strcmp(ip, "35.240.201.242") == 0 ||
    strcmp(ip, "35.240.238.247") == 0 || 
    strcmp(ip, "35.72.104.232") == 0 ||
    strcmp(ip, "35.194.189.163") == 0 ||
    strcmp(ip, "35.247.218.171") == 0 ||
    strcmp(ip, "35.247.247.182") == 0)) {
            printf("Connection to %s blocked\n", ip);
            blockcount++;
            return -1; // Block connection
        }
    }

    // Check blocked hostnames if block_connections is enabled
    struct hostent *host = gethostbyaddr(&(((struct sockaddr_in*)addr)->sin_addr), sizeof(struct in_addr), AF_INET);
    if (block_connections && host && 
        (

        strcmp(host->h_name, "firebaselogging.googleapis.com") == 0 ||
        strcmp(host->h_name, "httpdns.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "g108na-14.gph.easebar.com") == 0 ||
        strcmp(host->h_name, "g0-web.gsf.easebar.com") == 0 ||
        strcmp(host->h_name, "filecatch.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "feedback-system-dev.webapp.easebar.com") == 0 ||
        strcmp(host->h_name, "feedback-ovs.fp.ps.easebar.com") == 0 ||
        strcmp(host->h_name, "fb.webapp.easebar.com") == 0 ||
        strcmp(host->h_name, "event.shence.easebar.com") == 0 ||
        strcmp(host->h_name, "fb-dev.webapp.easebar.com") == 0 ||
        strcmp(host->h_name, "event.sc.easebar.com") == 0 ||
        strcmp(host->h_name, "applog.unisdk.easebar.com") == 0 ||
        strcmp(host->h_name, "api-office.db.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "api-data-klasifikasi.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "aliothjp-gcp.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "admin.fp.ps.easebar.com") == 0 ||
        strcmp(host->h_name, "l33sg-gamerecord-sg.l33naobj.leihuo.easebar.com") == 0 ||
        strcmp(host->h_name, "acsdk.gameyw.easebar.com") == 0 ||
        strcmp(host->h_name, "app-measurement.com") == 0 ||
        strcmp(host->h_name, "anticheatexpert.com") == 0 ||
        strcmp(host->h_name, "gameguard.net") == 0 ||
        strcmp(host->h_name, "bamreport.bloodstrike.netease.com") == 0 ||
        strcmp(host->h_name, "analytics.bloodstrike.netease.com") == 0 ||
        strcmp(host->h_name, "lamssettings-pa.googleapis.com") == 0 ||
        strcmp(host->h_name, "graph.facebook.com") == 0 ||
        strcmp(host->h_name, "appdump.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "whoami.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "firebaseremoteconfig.googleapis.com") == 0 ||
        strcmp(host->h_name, "audiostatlog.cc.easebar.com") == 0 ||
        strcmp(host->h_name, "audiouser.cc.easebar.com") == 0 ||
        strcmp(host->h_name, "drpf-g83naxx1ena.proxima.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "newspike-avatar.fp.ps.easebar.com") == 0 ||
        strcmp(host->h_name, "b.q.easebar.com") == 0 ||
        strcmp(host->h_name, "c5ipmc-inapps.appsflyersdk.com") == 0 ||
        strcmp(host->h_name, "cc.fp.ps.netease.com") == 0 ||
        strcmp(host->h_name, "cdn-settings.appsflyersdk.com") == 0 ||
        strcmp(host->h_name, "conversions.appsflyer.com") == 0 ||
        strcmp(host->h_name, "data-detect.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "g0.gsf.easebar.com") == 0 ||
        strcmp(host->h_name, "gcdsdk.appsflyer.com") == 0 ||
        strcmp(host->h_name, "inapps.appsflyer.com") == 0 ||
        strcmp(host->h_name, "in.appcenter.ms") == 0 ||
        strcmp(host->h_name, "o33249.ingest.sentry.io") == 0 ||
        strcmp(host->h_name, "launches.appsflyer.com") == 0 ||
        strcmp(host->h_name, "mcount.easebar.com") == 0 ||
        strcmp(host->h_name, "p16-sign-va.tiktokcdn.com") == 0 ||
        strcmp(host->h_name, "pharos.netease.com") == 0 ||
        strcmp(host->h_name, "beacons5.gvt5.com") == 0 ||
        strcmp(host->h_name, "beacons5.gvt4.com") == 0 ||
        strcmp(host->h_name, "beacons5.gvt3.com") == 0 ||
        strcmp(host->h_name, "beacons5.gvt2.com") == 0 ||
        strcmp(host->h_name, "beacons.gcp.gvt2.com") == 0 ||
        strcmp(host->h_name, "ire-dsu.shalltry.com") == 0 ||
        strcmp(host->h_name, "a.nel.cloudflare.com") == 0 ||
        strcmp(host->h_name, "optimizationguide-pa.googleapis.com") == 0 ||
        strcmp(host->h_name, "ire-oneid.shalltry.com") == 0 ||
        strcmp(host->h_name, "protocol.unisdk.easebar.com") == 0 ||
        strcmp(host->h_name, "s.q.easebar.com") == 0 ||
        strcmp(host->h_name, "sdkgate.pushv3.easebar.com") == 0 ||
        strcmp(host->h_name, "translate.mpsdk.easebar.com") == 0 ||
        strcmp(host->h_name, "unisdk.proxima.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "who.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "who.easebar.com") == 0 ||
        strcmp(host->h_name, "ulogs.umengcloud.com") == 0 ||
        strcmp(host->h_name, "g83naxx1ena.appdump.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "p16-oec-va.ibyteimg.com") == 0 ||
        strcmp(host->h_name, "reqhfg-cdn-settings.appsflyersdk.com") == 0 ||
        strcmp(host->h_name, "impression.update.easebar.com") == 0 ||
        strcmp(host->h_name, "logs.bloodstrike.com") == 0 ||
        strcmp(host->h_name, "tracking.bloodstrike.com") == 0 ||
        strcmp(host->h_name, "anti-cheat.bloodstrike.com") == 0 ||
        strcmp(host->h_name, "h73na-14.gdl.easebar.com") == 0 ||
        strcmp(host->h_name, "h65na-14.gph.easebar.com") == 0 ||
        strcmp(host->h_name, "g83-microservice-testing.nie.easebar.com") == 0 ||
        strcmp(host->h_name, "g29-114.gdl.easebar.com") == 0 ||
        strcmp(host->h_name, "g138-14.gdl.easebar.com") == 0 ||
        strcmp(host->h_name, "g112na-pre.gdl.easebar.com") == 0 ||
        strcmp(host->h_name, "g108naxx2gb-14.update.easebar.com") == 0 ||
        strcmp(host->h_name, "g108naxx2gb-14.gph.easebar.com") == 0 ||
        strcmp(host->h_name, "g108naxx2gb-14.gsf.easebar.com") == 0)){
        LOGI("Connection to %s blocked\n", host->h_name);
        blockcount++;
        return 0; // Block connection
    }

    return original_connect(sockfd, addr, addrlen);
}

__attribute__((constructor))
void hook_connect() {
   void *connect_addr = DobbySymbolResolver("/system/lib/libc.so", "connect");
    
   if (connect_addr) {
        DobbyHook(connect_addr, (void *)my_connect, (void **)&original_connect);
    } else {
        fprintf(stderr, "Failed to hook connect function: %s\n", dlerror());
    }
}
*/

