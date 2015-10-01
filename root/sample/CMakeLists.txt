#######################
##    SAMPLE CMAKE   ##
#######################

#########################
## EXECUTABLES SESSION ##
#########################
find_package(OpenCV REQUIRED)

add_executable(ps4eye_test ps4eye_test.cpp)
target_link_libraries(ps4eye_test ${SOURCE_LIBS} ${OpenCV_LIBS})

add_executable(ps4eye_stereo ps4eye_stereo.cpp)
target_link_libraries(ps4eye_stereo ${SOURCE_LIBS} ${OpenCV_LIBS})

add_executable(ps4eye_calib ps4eye_calib2.cpp)
target_link_libraries(ps4eye_calib ${SOURCE_LIBS} ${OpenCV_LIBS})

