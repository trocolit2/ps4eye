#!/bin/bash

# EN -> ingles.
# PT -> português. 

# EN -> 
## Script to automatic install ps4eye driver.
## This code is based on https://github.com/ps4eye/ps4eye 
# PT ->
## Script para instalar o driver da ps4eye automaticamente.
## O código é baseado em https://github.com/ps4eye/ps4eye 

# EN -> 
## This script was tested in:
# PT ->
## Este script foi testado em:
### Ubuntu 14.*
### Ubuntu 15.04

# EN -> 
## Colors

# PT ->
## Cores

red="$(tput setaf 1)"
green="$(tput setaf 2)"
standColor="$(tput sgr 0)"


# EN -> 
## if the first argument is "remove", dont install and remove the programs installed in the installation
# PT ->
## se o primeiro argmento for "remove", não instala e remove os programas instalados na instalação

argument1="$1"

if [ "$argument1" != "remove" ];then


# EN -> 
## Check ubuntu kernel to correcty
### kernel_version receives a line of the filter "uname -r | sed 's/\(\-[0-9]* ..." for compare
### if the kernel is compatible with the instalation of PS4eye.
### if kernel_version is less than kernel_reference, show a error menssage and exit install 

# PT ->
## Checar se o kernel do ubuntu esta correto
### kernel_version recebe uma linha do filtro do "uname -r | sed 's/\(\-[0-9]* ..." para comparar
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
### ps4eye_conected receives a line of the filter "lsusb | grep 05a9:0580" for compare
### if the ps4eye is plugged
### if not receive anything, it show a error message and exit install

# PT ->
## Checar se a PS4eye esta conectada
### ps4eye_conected recebe uma linha do filtro do "lsusb | grep 05a9:0580" para comparar
### se a ps4eye esta plugada
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
### check_python_setuptools receives a line of filter "dpkg -l | grep python-setuptools" for compare
### if the python-setuptoos is installed
### if not receive anything, install python-setuptoos
### http://www.hacksparrow.com/ubuntu-how-to-install-easy_install.html

# PT ->
## Checar se o python-setuptools está instalado para instalar o easy_install e o pip -> pyusb
### check_python_setuptools recebe uma linha do filtro do "dpkg -l | grep python-setuptools" para comparar
### se o python-setuptoos esta instalado
### se não receber nada, instala o python-setuptoos
### http://www.hacksparrow.com/ubuntu-how-to-install-easy_install.html

	check_python_setuptools=$(dpkg -l | grep python-setuptools)
	
	if ! [ "$check_python_setuptools" ] ; then
		sudo apt-get install python-setuptools 
	fi
	 
	echo  -e "\n ${green}Python-setuptools installed${standColor}"
	
# EN ->
## Check PIP installed
### check_pip receives a line of the filter "which pip" for compare
### if the pip is installed
### if not receive anything, install pip

# PT ->
## Checar se o PIP esta instalado
### check_pip recebe uma linha do filtro do "which pip" para comprar
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
### check_git_installed receives a line of the filter "dpkg -l | grep..." for compare
### if git is installed
### if not receive anything, install git

# PT ->
## Checar se o GIT esta instalado
### check_git_installed recebe uma linha do filtro do "dpkg -l | grep..." para comparar
### se o git esta instalado
### se não receber nada, intala o git

	check_git=$(dpkg -l | grep -E ' git ')
	
	if ! [ "$check_git" ] ; then
		sudo apt-get install git
	fi
	
	echo  -e "\n ${green}GIT installed${standColor}"
	
# EN ->
## Check folder ps4eye
### directory receives the path for instalation.
### if the folder exists only shows it. 
### if not, does a clone of the repository https://github.com/ps4eye/ps4eye 

# PT ->
## Checar se a pasta ps4eye existe
### directory recebe o caminho para instalação.
### se a pasta existe só mostra que ela existe
### se não, faz um clone do repositório https://github.com/ps4eye/ps4eye 

	directory=$(pwd)
	if [ -d "$directory/ps4eye" ] ; then
		echo -e "\nDirectory /ps4eye exists."
	else
	echo "Cloning the git PS4EYE"
		git clone https://github.com/ps4eye/ps4eye
		echo -e "\n Directory /ps4eye created."
	fi
	
# EN ->
## Initianting the ps4eye camera driver with python
### Go into the folder /ps4eye/python
### Start the ps4eye_init.py for configure the ps4eye in system

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
### if the first argument is equal "test",
### check_luvcview receives a line of the  for compare
### if the luvcview is installed
### if not, install luvcview, after enter the program showing the camera image

# PT ->
## Iniciando a visualização da câmera PS4eye pelo luvcview
### Se o 1º argumento de entrada for igual a "test", 
### check_luvcview recebe uma linha do filtro "dpkg -l | grep luvcview" para comparar
### se o luvcview esta instalado
### se não tiver, intala-o e depois entra no programa mostrando a imagem da câmera

	test="test"
	
	sleep 1
	
	if [ "$argument1" == "$test" ]; then
	
		check_luvcview=$(dpkg -l | grep luvcview)
	
		if ! [ "$check_luvcview" ] ; then
			sudo apt-get install luvcview
		fi
	
		echo -e "\n ${green}PS4eyeluvcview instaled${standColor}"
	
# EN ->
## Filter for find the path of PS4eye
### For ensure that the luvcview will show the image PS4eye makes up a loop in /dev/video*
### check_device receives a filter "udevadm info ..." with the idVendor and idProduct of PS4eye,
### http://wiki.openrobotino.org/index.php?title=USB_cameras

# PT ->
## Filtro para encontrar o caminho da PS4eye
### Para certificar que o luvcview irá mostrar a imagem da PS4eye faz-se um loop no /dev/video* 
### check_device recebe uma linha do filtro do "udevadm info ..." com o idVendor e idProduct da PS4eye, 
### http://wiki.openrobotino.org/index.php?title=USB_cameras

		FILES=/dev/video*
		for f in $FILES; do
	
			check_device=$(udevadm info --query=all --attribute-walk --name=$f | grep -E "05a9|058a")
		
			if [ "$check_device" ] ; then
				way_device=$f
				break
			fi
	
		done
	
		echo -e "\nThe way of PS4eye is: $way_device"

		luvcview -d $way_device -i 60 -s 1748x408


	fi


else

	check_python_setuptools=$(dpkg -l | grep python-setuptools)

	if [ "$check_python_setuptools" ] ; then
		sudo apt-get purge python-setuptools
		check_python_setuptools=$(dpkg -l | grep python-setuptools)
		if ! [ "$check_python_setuptools" ] ; then
			echo  -e "\n ${green}Python-setuptools removed${standColor}"
		fi
	fi
	
	check_pip=$(which pip)
	
	if [ "$check_pip" ] ; then
		read -p "Do you want to remove pip? [Y/n]" pip_remove
		if [ "$pip_remove" == "y" ] || [ "$pip_remove" == "Y" ] ; then
			sudo rm "$check_pip"
		check_pip=$(which pip)
		if ! [ "$check_pip" ] ; then
			echo  -e "\n ${green} pip removed${standColor}"
		fi
		fi
	
	fi
	
	check_git=$(dpkg -l | grep -E ' git ')
	
	if [ "$check_git" ] ; then
		sudo apt-get purge git
		check_git=$(dpkg -l | grep -E ' git ')
		if ! [ "$check_git" ] ; then
			echo  -e "\n ${green}GIT removed${standColor}"
		fi
	fi
		
	check_luvcview=$(dpkg -l | grep luvcview)
	
	if [ "$check_luvcview" ] ; then
		sudo apt-get purge luvcview
		check_luvcview=$(dpkg -l | grep luvcview)
		if ! [ "$check_luvcview" ] ; then
			echo -e "\n ${green}PS4eyeluvcview removed${standColor}"
		fi
	fi
	
	
	fi
