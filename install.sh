#!/bin/bash
### Script to automatic install ps4eye driver.
### This code is based on https://github.com/ps4eye/ps4eye 

### This script was tested in:
### Ubuntu 14.*
### Ubuntu 15.04


echo "Installing PS4eye camera"

#Check PS4EYE conected

check_ps4eye_conected=$(lsusb | grep 05a9:0580)

if ! [ "$check_ps4eye_conected" ] ; then
	echo "ERROR PS4EYE NOT CONECTED"
	exit 0
fi

echo "Detected ps4eye stereo camera -> $check_ps4eye_conected"

#Check python-setuptools for install easy_install and pip -> pyusb

check_python_setuptools=$(dpkg -l | grep python-setuptools)

if ! [ "$check_python_setuptools" ] ; then
	sudo apt-get install python-setuptools 
fi
 
echo  -e "\n Python-setuptools installed"


check_pip=$(which pip)

if ! [ "$check_pip" ] ; then
	sudo easy_install pip
	sudo pip install --pre pyusb
fi

echo  -e "\n pip and pyusb installed"

#Check GIT installed

check_git_installed=$(dpkg -l | grep -E ' git ')

if ! [ "$check_git_installed" ] ; then
	sudo apt-get install git
fi

echo  -e "\n GIT installed"

#Check folder ps4eye
directory=$(pwd)
if [ -d "$directory/ps4eye" ] ; then
	echo -e "\nDirectory /ps4eye exists."
else
Cloning the git PS4EYE
	git clone https://github.com/ps4eye/ps4eye
	echo -e "\n Directory /ps4eye created."
fi

lsusb -d 05a9:0580

#Initianting the ps4eye camera with python
ls
cd ps4eye/
ls
cd python/
ls
sudo ./ps4eye_init.py
echo -e "\n Installing PS4eye camera"


