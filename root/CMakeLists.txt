cmake_minimum_required(VERSION 2.8)

#if(APPLE)
#    set(CMAKE_C_COMPILER "gcc")
#    set(CMAKE_CXX_COMPILER "g++")
#endif(APPLE)

project(PS4eye)

######################
## PROJECT SETTINGS ##
######################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive") # -fpermissive
#ADD_DEFINITIONS(-Wno-variadic-macros -Wno-long-long -Wall -Wextra -Winit-self -Woverloaded-virtual -Wsign-promo -Wno-unused-parameter -pedantic -Woverloaded-virtual -Wno-unknown-pragmas -Waddress-of-temporary)

## DEFINE MAIN PATHS
set(bin_path ${PROJECT_BINARY_DIR}/bin)
set(source_path ${PROJECT_SOURCE_DIR}/src)
set(test_path ${PROJECT_SOURCE_DIR}/test)
set(sample_path ${PROJECT_SOURCE_DIR}/sample)

## SUB PATHS
## set(configure_path ${source_path}/configure)
set(include_build_path ${PROJECT_BINARY_DIR}/includes)

## DEFINE BINARY DESTINE
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin_path})

#############################
## PROJECT CONTROL VERSION ##
#############################

## set a configure variables
set(main_project_name "\"PS4eye - Portable Stereo Camera\"" )
set(main_version 0)
set(fuction_version 1)
set(patch_release 0)

## include CONFIGURE MAIN PATHS
include_directories(${include_build_path})
## add_subdirectory(${configure_path})

##########################
## SOURCE MAIN PATH ##
##########################
include_directories(${source_path})
add_subdirectory(${source_path})

##########################
## SAMPLE MAIN PATH ##
##########################
include_directories(${sample_path})
add_subdirectory(${sample_path})

####################
## TEST MAIN PATH ##
####################
enable_testing()
add_subdirectory(${test_path})

