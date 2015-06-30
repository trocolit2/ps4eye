#!/bin/bash
### Script to automatic install ps4eye driver.
### This code is based on https://github.com/ps4eye/ps4eye 

### This script was tested in:
### Ubuntu 14.*
### Ubuntu 15.04

## Check ubuntu kernel to correcty 
## kernel_version recebe uma linha do filtro do "uname -r | sed 's/\(\-[0-9]* ..." para saber
## se o kernel é compativel com a instalação da PS4eye

## se kernel_version for menor que kernel_reference, mostra uma mensagem de error e sai da instalação

echo "Check Ubuntu Kernel"

kernel_version=$(uname -r | sed 's/\(\-[0-9]*\-[a-zA-Z]*\)//')
kernel_reference="3.16.0"
if [ "$kernel_version" \< "$kernel_reference" ] ; then
	echo "ERROR!! Kernel version less than 3.16. Please install compatible Kernel."
	exit 0
fi

echo "Kernel Version $kernel_version"

echo "Installing PS4eye camera"

#Check if ps4eye is conected
## ps4eye_conected recebe uma linha do filtro do "lsusb | grep 05a9:0580" para saber
## se a camera esta plugada
## se não receber nada, mostra uma mensagem de error e sai da instalação


ps4eye_conected=$(lsusb | grep 05a9:0580)

if ! [ "$ps4eye_conected" ] ; then
	echo "ERROR!! PS4EYE NOT CONECTED."
	exit 0
fi

echo "Detected ps4eye stereo camera -> $check_ps4eye_conected"

#Check python-setuptools for install easy_install and pip -> pyusb
## check_python_setuptools recebe uma linha do filtro do "dpkg -l | grep python-setuptools" para saber
## se o python-setuptoos esta instalado
## se não receber nada, intala o python-setuptoos

check_python_setuptools=$(dpkg -l | grep python-setuptools)

if ! [ "$check_python_setuptools" ] ; then
	sudo apt-get install python-setuptools 
fi
 
echo  -e "\n Python-setuptools installed"

#Check PIP installed
## check_pip recebe uma linha do filtro do "which pip" para saber
## se o pip esta instalado
## se não receber nada, intala o pip
 

check_pip=$(which pip)

if ! [ "$check_pip" ] ; then
	sudo easy_install pip
	sudo pip install --pre pyusb
fi

echo  -e "\n pip and pyusb installed"

#Check GIT installed
## check_git_installed recebe uma linha do filtro do "dpkg -l | grep..." para saber
## se se o git esta instalado
## se não receber nada, intala o git

check_git_installed=$(dpkg -l | grep -E ' git ')

if ! [ "$check_git_installed" ] ; then
	sudo apt-get install git
fi

echo  -e "\n GIT installed"

#Check folder ps4eye
## directory é onde esta o arquivo de instalação
## se a pasta existe só mostra que ela existe
## se não, faz um clone do repositório https://github.com/ps4eye/ps4eye 

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
## Entra na pasta /ps4eye/python
## inicia o arquivo ps4eye_init.py para configurar a ps4eye no sistema

ls
cd ps4eye/
ls
cd python/
ls
sudo ./ps4eye_init.py
echo -e "\n Installing PS4eye camera"


