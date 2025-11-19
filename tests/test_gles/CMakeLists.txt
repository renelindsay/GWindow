cmake_minimum_required(VERSION 3.22.1)
project(Test_gles VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#============================= Window ============================
add_subdirectory(../../Window Window)
#=================================================================
#============================= SOURCE ============================
include_directories (cpp)
aux_source_directory(cpp SRC_LIST)
#=================================================================

#==============================LINUX==============================
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Check for required headers (EGL, GLES2)
    find_path(EGL_INCLUDE_DIR NAMES EGL/egl.h)
    find_path(GLES2_INCLUDE_DIR NAMES GLES2/gl2.h)
    if(NOT EGL_INCLUDE_DIR OR NOT GLES2_INCLUDE_DIR)
        message(WARNING "EGL+GLES not found. Run 'requirements.sh' to install dependencies.")
    endif()

    add_executable(${PROJECT_NAME} ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME} Window EGL)
endif()
#=================================================================
#=============================WINDOWS=============================
if(WIN32)
    set(ANGLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/angle-x64)
    if(NOT EXISTS ${ANGLE_DIR})
        message(WARNING "ANGLE not found. Run 'requirements.bat' to set up dependencies.")
    endif()

    add_executable            (${PROJECT_NAME} ${SRC_LIST})
    target_include_directories(${PROJECT_NAME} PRIVATE ${ANGLE_DIR}/include)
    target_link_directories   (${PROJECT_NAME} PRIVATE ${ANGLE_DIR}/lib)
    target_link_libraries     (${PROJECT_NAME} Window libEGL.dll.lib libGLESv2.dll.lib)
endif()
#=================================================================
#=============================ANDROID=============================
if(ANDROID)
    message("Platform = ${ANDROID_PLATFORM}")
    add_link_options(-Wl,-z,max-page-size=16384)  # 16kb page size   
    configure_file( # Updates Manifest to set lib_name = ${PROJECT_NAME}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/AndroidManifest.xml.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/AndroidManifest.xml"
    @ONLY)
    add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME} Window EGL GLESv2)
endif()
#=================================================================
#==============================ALL================================
# Place exe by assets:
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#=================================================================
