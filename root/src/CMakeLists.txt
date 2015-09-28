#######################
## SOURCE MAIN CMAKE ##
#######################

################################
## LIBRARYS AND CLASS SESSION ##
################################

find_package(OpenCV REQUIRED)

## ADD SUBDIRECTORIES FROM LIBRARY
include_directories(v4ltool)
add_subdirectory(v4ltool)

add_library(ps4eyeLibrary V4LTools.cpp DeviceUtils.cpp PS4eye.cpp)
set(SOURCE_LIBS_LOCAL ps4eyeLibrary ${OpenCV_LIBS} "-ludev")
set(SOURCE_LIBS ${SOURCE_LIBS_LOCAL} ${SOURCE_LIBS} PARENT_SCOPE)

#########################
## EXECUTABLES SESSION ##
#########################

#add_executable(ps4eye main.cxx)
#target_link_libraries(ps4eye ${SOURCE_LIBS})
