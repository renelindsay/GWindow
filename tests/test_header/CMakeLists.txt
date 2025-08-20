cmake_minimum_required(VERSION 3.22.1)
project(Test_header VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#============================= SOURCE ============================
include_directories (../..)  # GWindow.h
include_directories (cpp)
aux_source_directory(cpp SRC_LIST)
#=================================================================

#==============================LINUX==============================
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()
#=================================================================
#=============================WINDOWS=============================
if(WIN32)
    add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()
#=================================================================
#=============================ANDROID=============================
if(ANDROID)
    add_link_options(-Wl,-z,max-page-size=16384)  # 16kb page size
    #set(PROJECT_NAME "native")
    #message("PROJECT_NAME = ${PROJECT_NAME}")
    
    configure_file( # Updates Manifest to set lib_name = ${PROJECT_NAME}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/AndroidManifest.xml.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/AndroidManifest.xml"
    @ONLY)
    add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
endif()
#=================================================================
#==============================ALL================================
# Link GWindow's dependencies
include("../../GWindow.cmake")
add_gwindow(${PROJECT_NAME})

# Place exe by assets:
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#=================================================================
