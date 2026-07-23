cmake_minimum_required(VERSION 3.10.2)

project("star")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Flags - Fixed PAGE_SIZE for NDK 28+
set(COMMON_FLAGS "-Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -fpermissive -DPAGE_SIZE=4096 -DPAGE_MASK=0xfffff000")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -fno-rtti -fexceptions -Wno-error=c++11-narrowing -fms-extensions")

add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# Include Directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/curl/curl-android-${ANDROID_ABI}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/curl/openssl-android-${ANDROID_ABI}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

# Prebuilt Static Libraries
add_library(libdobby STATIC IMPORTED)
set_target_properties(libdobby PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Dobby/libraries/${ANDROID_ABI}/libdobby.a)

add_library(libcurl STATIC IMPORTED)
set_target_properties(libcurl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/curl/curl-android-${ANDROID_ABI}/lib/libcurl.a)

add_library(libssl STATIC IMPORTED)
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/curl/openssl-android-${ANDROID_ABI}/lib/libssl.a)

add_library(libcrypto STATIC IMPORTED)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/curl/openssl-android-${ANDROID_ABI}/lib/libcrypto.a)

# Main Library
add_library(star SHARED
    src/main.cpp
    src/block.cpp
    src/dlfcn.cpp
    src/srcpy.cpp
    src/features.cpp
    vendor/KittyMemory/KittyMemory.cpp
    vendor/KittyMemory/KittyUtils.cpp
    vendor/KittyMemory/MemoryBackup.cpp
    vendor/KittyMemory/MemoryPatch.cpp
    vendor/And64InlineHook/And64InlineHook.cpp
    vendor/ByNameModding/Tools.cpp
    vendor/Oxorany/oxorany.cpp
    vendor/xhook/xhook.c
    vendor/xhook/xh_core.c
    vendor/xhook/xh_elf.c
    vendor/xhook/xh_jni.c
    vendor/xhook/xh_log.c
    vendor/xhook/xh_util.c
    vendor/xhook/xh_version.c
    vendor/xdl/xdl.c
    vendor/xdl/xdl_iterate.c
    vendor/xdl/xdl_linker.c
    vendor/xdl/xdl_lzma.c
    vendor/xdl/xdl_util.c
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/stb_image.cpp
    vendor/imgui/FontData.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_android.cpp
)

# Link Libraries
target_link_libraries(star
    libdobby
    libcurl
    libssl
    libcrypto
    android
    EGL
    GLESv3
    z
    log
)
