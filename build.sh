#!/bin/bash
##Clean the screen when run the build.sh
clear

#properties variables
var_folderName=${PWD##*/}
var_cleanComand="clean"
var_make="make"
var_test="test"
var_cmake="cmake"
var_install="install"




#### INSTALL PROCESSS
if [ "$1" = "$var_install" ] || [ "$2" = "$var_install" ] || [ "$3" = "$var_install" ] || [ "$4" = "$var_install" ]  || [ "$5" = "$var_install" ] 
then
	cd root
	. install.sh
	cd ..
	echo "INSTALL DONE"
fi

echo " ## Start build project $var_folderName ## "

if [ ! -d "build" ];
then
	echo "FOLDER BUILD DO NOT EXIST"
	echo "MAKE DIR BUILD"
	mkdir build
	echo "FOLDER BUILD MADE"
fi


cd build


#### CLEANING PROCESSS
if [ "$1" = "$var_cleanComand" ] || [ "$2" = "$var_cleanComand" ] || [ "$3" = "$var_cleanComand" ] || [ "$4" = "$var_cleanComand" ]  
then
	rm -rf *
	echo "CLEANING BUILD FOLDER"
fi

#### CMAKE PROCESS

if [ "$1" = "$var_cmake" ] || [ "$2" = "$var_cmake" ] || [ "$3" = "$var_cmake" ] || [ "$4" = "$var_cmake" ]  
then
	echo " ## Start CMake process ## "
	cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root
	echo " ## Finished CMake process ## "
fi

#### MAKE PROCESS

if [ "$1" = "$var_make" ] || [ "$2" = "$var_make" ] || [ "$3" = "$var_make" ] || [ "$4" = "$var_make" ]  
then
	echo " ## Start make ## "
	make -j8 
	echo " ## Finished make ## "
fi

#### TEST PROCESSS
if [ "$1" = "$var_test" ] || [ "$2" = "$var_test" ] || [ "$3" = "$var_test" ] || [ "$4" = "$var_test" ]
then 
	echo " ## Start Tests ## "
	make test ARGS="-V"
	#make test 
	echo " ## Finished Tests## "
fi


echo " ## Finished build project $var_folderName ## "

