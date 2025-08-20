cmake_minimum_required(VERSION 3.22.1)
project(Test_lib VERSION 0.1)

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
    add_executable(${PROJECT_NAME} ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME} Window)
endif()
#=================================================================
#=============================WINDOWS=============================
if(WIN32)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    add_executable(${PROJECT_NAME} ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME} Window)
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
    target_link_libraries(${PROJECT_NAME} Window)
endif()
#=================================================================
#==============================ALL================================
# Place exe by assets:
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#=================================================================
