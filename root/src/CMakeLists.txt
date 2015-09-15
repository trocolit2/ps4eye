#######################
## SOURCE MAIN CMAKE ##
#######################

################################
## LIBRARYS AND CLASS SESSION ##
################################

find_package(OpenCV REQUIRED)
#find_package(Boost COMPONENTS regex REQUIRED)
#include_directories (${Boost_INCLUDE_DIRS})

set(SOURCE_LIBS_LOCAL ${OpenCV_LIBS})
#${Boost_REGEX_LIBRARY}

## ADD SUBDIRECTORIES FROM LIBRARY
#include_directories(library)
#add_subdirectory(library)

add_library(ps4eyeLibrary PS4eye.cpp DeviceUtils.cpp)
set(SOURCE_LIBS_LOCAL ps4eyeLibrary)
set(SOURCE_LIBS ${SOURCE_LIBS} ${SOURCE_LIBS_LOCAL} PARENT_SCOPE)

#########################
## EXECUTABLES SESSION ##
#########################

#add_executable(AS2C main.cxx)
#target_link_libraries(AS2C ${SOURCE_LIBS})
