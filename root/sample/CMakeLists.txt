#######################
##    SAMPLE CMAKE   ##
#######################

#########################
## EXECUTABLES SESSION ##
#########################
find_package(OpenCV REQUIRED)

add_executable(simple_test simple_test.cpp)
target_link_libraries(simple_test ${SOURCE_LIBS} ${OpenCV_LIBS})


add_executable(stereo_test stereo_test.cpp)
target_link_libraries(stereo_test ${SOURCE_LIBS} ${OpenCV_LIBS})
