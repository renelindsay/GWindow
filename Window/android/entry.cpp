#include "JClass.h"
#include "android_fopen.h"

android_app* Android_App = nullptr;  // Android native-activity state
int main(int argc, char *argv[]);    // Forward declaration of main function

//--------------------------------------Application Entry Point-------------------------------------
void android_main(struct android_app* state) {
    Android_App = state;                                             // Pass android app state to window_android.cpp
    android_fopen_set_asset_manager(state->activity->assetManager);  // Re-direct fopen to read assets from our APK.
    main(0, nullptr);                                                // call main()
    ANativeActivity_finish(state->activity);                         // exit
}
//--------------------------------------------------------------------------------------------------
