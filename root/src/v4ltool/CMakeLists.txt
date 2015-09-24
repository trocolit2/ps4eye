#######################
##    V4TOOL CMAKE   ##
#######################

################################
## LIBRARYS AND CLASS SESSION ##
################################

add_library(v4ltool SHARED v4l2uvc.c)
set(SOURCE_LIBS_LOCAL v4ltool)
set(SOURCE_LIBS ${SOURCE_LIBS} ${SOURCE_LIBS_LOCAL}  PARENT_SCOPE)

#########################
## EXECUTABLES SESSION ##
#########################

#add_executable(testOpencv test.cpp)
#target_link_libraries(testOpencv ${SOURCE_LIBS_LOCAL} ${OpenCV_LIBS})
