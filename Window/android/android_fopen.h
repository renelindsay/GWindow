// From http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
//
// This class hijacks android fopen, to read from the APK's assets folder.
// This avoids having to use AAssetManager explicitly, so libs like stb_image.h,
// that uses fopen, will work as is, without modification.
// To prevent breaking normal android fopen behavior, the following precautions were taken:
// If fopen is used with an absolute path (starting with '/') normal fopen is used. (for obb)
// If fopen is used to write, of if the file exists in Internal storage, normal fopen is used.
// Else, if reading from a relative path, fopen is redirected to read from the APK's assets folder.
// Optionally, you can start the path with 'assets/', to better match the desktop build's path.

#ifndef ANDROID_FOPEN_H
#define ANDROID_FOPEN_H

#define _DEFAULT_SOURCE 1  // Enables funopen()

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <android/asset_manager.h>

class Android_fopen {
    static int read(void* cookie, char* buf, int size) {
        return AAsset_read((AAsset*)cookie, buf, size);
    }

    static int write(void* cookie, const char* buf, int size) {
        return EACCES; // can't provide write access to the apk
    }

    static fpos_t seek(void* cookie, fpos_t offset, int whence) {
        return AAsset_seek((AAsset*)cookie, offset, whence);
    }

    static int close(void* cookie) {
        AAsset_close((AAsset*)cookie);
        return 0;
    }

public:
    // must be established by someone else...
    static inline AAssetManager* asset_manager = nullptr;

    static FILE* fopen(const char* fname, const char* mode) {
        // Use regular fopen for absolute paths
        if (fname[0] == '/') return std::fopen(fname, mode);

        // Use regular fopen if file exists OR writing is attempted
        FILE* file = std::fopen(fname, mode);
        if (file || mode[0] != 'r') return file;

        // Finally, read from the APK instead, via AAssetManager
        if (!strncmp(fname, "./",     2)) fname+=2;  // skip the ./ prefix, if present
        if (!strncmp(fname,"assets/", 7)) fname+=7;  // skip the assets/ prefix, if present
        AAsset* asset = AAssetManager_open(asset_manager, fname, 0);
        if(!asset) return nullptr;
        return funopen(asset, read, write, seek, close);
    }
};

static void android_fopen_set_asset_manager(AAssetManager* manager) {
    Android_fopen::asset_manager = manager;
}

/* Hijack fopen and route it through the android asset system,
 * so that we can read from the APK's assets folder */
#define fopen(name, mode) Android_fopen::fopen(name, mode)

#endif

