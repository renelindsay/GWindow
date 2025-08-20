cmake_minimum_required(VERSION 3.22.1)
project(GWindowTest)

#  Reduce size of MinSizeRel builds
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # smaller exe -flto
endif()

# Test apps
add_subdirectory(test_lib)
add_subdirectory(test_header)
