cmake_minimum_required (VERSION 2.8)

#include_directories ("${PROJECT_SOURCE_DIR}/src/exemples/access_ps4eye_with_opencv")

project (Exemplo_ps4eye)
find_package( OpenCV REQUIRED )
add_executable(Exemplo_ps4eye src/exemples/access_ps4eye_with_opencv/main.cpp)

target_link_libraries( Exemplo_ps4eye ${OpenCV_LIBS} )
