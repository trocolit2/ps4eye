#!/bin/bash

# EN -> inglês.
# PT -> português. 

# EN -> 
## Script to automatic install ps4eye driver.
## This code is based on https://github.com/ps4eye/ps4eye 

# EN -> 
## This script was tested in:
# PT ->
## Este script foi testado em:

## Ubuntu 14.*
## Ubuntu 15.04

# EN -> 
## Colors

# PT ->
## Cores

red="$(tput setaf 1)"
green="$(tput setaf 2)"
standColor="$(tput sgr 0)"

# EN -> 
## Check ubuntu kernel to correcty
###

# PT ->
## Checar se o kernel do ubuntu esta correto
### kernel_version recebe uma linha do filtro do "uname -r | sed 's/\(\-[0-9]* ..." para saber
### se o kernel é compativel com a instalação da PS4eye.
### se kernel_version for menor que kernel_reference, mostra uma mensagem de error e sai da instalação

echo "Check Ubuntu Kernel"

kernel_version=$(uname -r | sed 's/\(\-[0-9]*\-[a-zA-Z]*\)//')
kernel_reference="3.16.0"
if [ "$kernel_version" \< "$kernel_reference" ] ; then
	echo "${red}ERROR!! Kernel version less than 3.16. Please install compatible Kernel. ${standColor}"
	exit 0
fi

echo "${green}Kernel Version $kernel_version${standColor}"

echo "Installing PS4eye camera"

# EN -> 
## Check if ps4eye is conected
###

# PT ->
## Checar se a PS4eye esta conectada
### ps4eye_conected recebe uma linha do filtro do "lsusb | grep 05a9:0580" para saber
### se a camera esta plugada
### se não receber nada, mostra uma mensagem de error e sai da instalação


ps4eye_conected=$(lsusb | grep 05a9:0580)
ps4eye_conected_modified=$(lsusb | grep 05a9:058a)

if ! [[ "$ps4eye_conected" || "$ps4eye_conected_modified" ]] ; then
	echo "${red}ERROR!! PS4EYE NOT CONECTED.${standColor}"
	exit 0
fi

echo "${green}Detected ps4eye stereo camera${standColor}"

# EN ->
## Check python-setuptools for install easy_install and pip -> pyusb

# PT ->
## Checar se o python-setuptools está instalado para instalar o easy_install e o pip -> pyusb
### check_python_setuptools recebe uma linha do filtro do "dpkg -l | grep python-setuptools" para saber
### se o python-setuptoos esta instalado
### se não receber nada, intala o python-setuptoos

check_python_setuptools=$(dpkg -l | grep python-setuptools)

if ! [ "$check_python_setuptools" ] ; then
	sudo apt-get install python-setuptools 
fi
 
echo  -e "\n ${green}Python-setuptools installed${standColor}"

# EN ->
## Check PIP installed

# PT ->
## Checar se o PIP esta instalado
### check_pip recebe uma linha do filtro do "which pip" para saber
### se o pip esta instalado
### se não receber nada, intala o pip
 

check_pip=$(which pip)

if ! [ "$check_pip" ] ; then
	sudo easy_install pip
	sudo pip install --pre pyusb
fi

echo  -e "\n${green}pip and pyusb installed${standColor}"

# EN ->
## Check GIT installed

# PT ->
## Checar se o GIT esta instalado
### check_git_installed recebe uma linha do filtro do "dpkg -l | grep..." para saber
### se se o git esta instalado
### se não receber nada, intala o git

check_git_installed=$(dpkg -l | grep -E ' git ')

if ! [ "$check_git_installed" ] ; then
	sudo apt-get install git
fi

echo  -e "\n ${green}GIT installed${standColor}"

# EN ->
## Check folder ps4eye

# PT ->
## Checar se a pasta ps4eye existe
### directory é onde esta o arquivo de instalação
### se a pasta existe só mostra que ela existe
### se não, faz um clone do repositório https://github.com/ps4eye/ps4eye 

directory=$(pwd)
if [ -d "$directory/ps4eye" ] ; then
	echo -e "\nDirectory /ps4eye exists."
else
Cloning the git PS4EYE
	git clone https://github.com/ps4eye/ps4eye
	echo -e "\n Directory /ps4eye created."
fi

# EN ->
## Initianting the ps4eye camera driver with python

# PT ->
## Iniciando o driver da camera ps4eye por python
### Entra na pasta /ps4eye/python
### inicia o arquivo ps4eye_init.py para configurar a ps4eye no sistema

cd ps4eye/
cd python/
sudo ./ps4eye_init.py

echo -e "\n ${green}PS4eye camera driver installed ${standColor}"

# EN ->
## Initianting the visualization of the ps4eye camera with luvcview

# PT ->
## Iniciando a visualização da câmera PS4eye pelo luvcview
### Se o 1º argumento de entrada for igual a teste, 
### Testa se está instalado o luvcview
### se não tiver, intala-o e depois entra no programa mostrando 
### a imagem da câmera

teste="teste"

if [ "$1" == "$teste" ]; then

check_luvcview=$(dpkg -l | grep luvcview)

if ! [ "$check_luvcview" ] ; then
	sudo apt-get install luvcview
fi

sleep 1

echo -e "\n ${green}PS4eyeluvcview instaled${standColor}"

# EN ->
##

# PT ->
## Filtro para encontrar o caminho da PS4eye
### Para mostrar a imagem da PS4eye faz-se um for no /dev/video* 
### check_device é o filtro do udevadm info com o idVendor e idProduct da PS4eye, 
### assim acha justamente o endereço da PS4eye para usar no comando da luvcview.
### http://wiki.openrobotino.org/index.php?title=USB_cameras

FILES=/dev/video*
for f in $FILES
do

check_device=$(udevadm info --query=all --attribute-walk --name=$f | grep -E "05a9|058a")

if [ "$check_device" ] ; then
way_device=$f
break
fi

done

echo -e "\nThe way of PS4eye is: $way_device"

luvcview -d $way_device -i 60 -s 1748x408

fi

