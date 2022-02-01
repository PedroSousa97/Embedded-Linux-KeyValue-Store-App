#! /bin/bash

##Script to automate and manage the project dependencies.
##It detects and installs all needed depedencies. You just need to aprove them
##At the end it will build the files needed to run the application.

##Author: Pedro Henrique Santos Sousa/henriquesantos293@gmail.com

##Begin Script

##Constants
 ##Variable to change the output text color to red
RED='\033[0;31m'
 ##Variable to change the output text color to green
GREEN='\033[1;32m'
 ##Variable to change the output text color to default
NOCOLOR='\033[0;m'
##Variable to change the output text color to dark grey
GREY='\033[1;30m'

echo
echo "--------------------------------- KVStoreAPP -------------------------------";
echo
echo "This is a Script to automate and manage the project dependencies.";
echo "It detects and installs all needed depedencies. You just need to aprove them";
echo "At the end it will build the files needed to run the application.";
echo
echo
echo  "This app needs the following dependencies installed so it can run properly:";
echo "-> libleveldb-dev;";
echo "-> make;";
echo "-> g++;";
echo
echo
echo -n "If needed, do you wish to run the Script? Yes(y) or No(n)?";
read ANSWER;
echo
if [ "$ANSWER" = "n" ]; then
	exit 0
fi
echo
echo
echo "Cheking for dependecies...";
##Check for dependencies
echo
echo -e "${NOCOLOR}Progress (1/3) - Checking if you have libleveldb-dev installed...${NOCOLOR}";
echo "LevelDB is a fast key-value storage library written at Google that provides an ordered";
echo "mapping from keys to values. It is also light weight, so it is embedded friendly.";
sleep 1s
##Check for libleveldb-dev

if command -v libleveldb-dev > /dev/null 2>&1; then
	echo -e "${GREEN}Good! you already have libleveldb-dev installed on your machine.${NOCOLOR}"
	echo
else
	echo -e "${RED}It seems you don't have libleveldb-dev installed on your machine.${NOCOLOR} "
	echo -n "Would you like to install it? Yes(y) or No(n)?";
	read ANSWER;
	echo
	if [ "$ANSWER" = "y" ]; then
		echo "Proceeding with the installation...";
		sudo apt-get install libleveldb-dev -y;
		echo
	else
	 	echo -e "${RED}Exiting with code 127...${NOCOLOR} ";
		exit 127;
	fi
fi

echo -e "${NOCOLOR}Progress (2/3) - Checking if you have make installed...${NOCOLOR}";
echo
sleep 1s
##Check for make

if command -v make > /dev/null 2>&1; then
	echo -e "${GREEN}Good! you already have make installed on your machine.${NOCOLOR}"
	echo
else
	echo -e "${RED}It seems you don't have make installed on your machine.${NOCOLOR} "
	echo -n "Would you like to install it? Yes(y) or No(n)?";
	read ANSWER;
	echo
	if [ "$ANSWER" = "y" ]; then
		echo "Proceeding with the installation...";
		sudo apt install make -y;
		echo
	else
	 	echo -e "${RED}Exiting with code 127...${NOCOLOR} ";
		exit 127;
	fi
fi

echo -e "${NOCOLOR}Progress (3/3) - Checking if you have g++ installed...${NOCOLOR}";
echo
sleep 1s
##Check for g++

if command -v g++ > /dev/null 2>&1; then
	echo -e "${GREEN}Good! you already have g++ installed on your machine.${NOCOLOR}"
	echo
else
	echo -e "${RED}It seems you don't have g++ installed on your machine.${NOCOLOR} "
	echo -n "Would you like to install it? Yes(y) or No(n)?";
	read ANSWER;
	echo
	if [ "$ANSWER" = "y" ]; then
		echo "Proceeding with the installation...";
		sudo apt install g++ -y;
		echo
	else
	 	echo -e "${RED}Exiting with code 127...${NOCOLOR} ";
		exit 127;
	fi
fi

echo "All dependencies have been successfully installed!";
echo 
echo "Moving to the project build";
echo "Building kvstoreapp..."
make -f makefile || exit
echo
echo "We are finished!. Just type ./kvstoreapp in a terminal inside this folder and you"
echo "are good to go!"
echo
echo
echo "You also have a Client Application example that I built for IPC testing purposes."
echo "If you want to use it, just cd to Client directory, and in a terminal type make."
echo "After that, once your kvstoreapp is in IPC mode, just type ./clientapp inside"
echo "Client directory, in another terminal."

##End script