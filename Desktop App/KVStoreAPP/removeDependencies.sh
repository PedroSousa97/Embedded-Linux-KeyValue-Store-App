#! /bin/bash

##Script to automate and manage the project dependencies removal.
##It deletes the previously installed depedencies for this project.

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
echo "This is a Script to automate and manage the project dependencies removal..";
echo "It deletes the previously installed depedencies for this project.";
echo
echo
echo  "This app needs the following dependencies installed so it can run properly:";
echo "-> libleveldb-dev;";
echo "-> make;";
echo "-> g++;";
echo
echo
echo "I'm going to ask for the removal of each one of this dependencies. You have"
echo "total control over the dependencies that you want to delete. Some may have"
echo "already been installed before this project."
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
echo "Removing dependecies...";
##Check for dependencies
echo
echo "Progress (1/3) - Do you want to remove libleveldb-dev?";
echo "Yes(y) or No(n)?";
echo
##Check for libleveldb-dev
read ANSWER;
if [ "$ANSWER" = "y" ]; then
    echo "Proceeding with the removal...";
    sudo apt-get purge --auto-remove libleveldb-dev
    echo
else
    echo -e "${RED}Ignoring libleveldb-dev removal...${NOCOLOR} ";
fi

echo "Progress (2/3) - Do you want to remove make?$";
echo "Yes(y) or No(n)?";
echo
##Check for make
read ANSWER;
if [ "$ANSWER" = "y" ]; then
    echo "Proceeding with the removal...";
    sudo apt-get purge --auto-remove make
    echo
else
    echo -e "${RED}Ignoring make removal...${NOCOLOR} ";
fi

echo "Progress (3/3) - Do you want to remove g++?";
echo "Yes(y) or No(n)?";
echo
read ANSWER;
##Check for g++
if [ "$ANSWER" = "y" ]; then
    echo "Proceeding with the removal...";
    sudo apt-get purge --auto-remove g++
    echo
else
    echo -e "${RED}Ignoring g++ removal...${NOCOLOR} ";
fi

echo "All dependencies have been successfully uninstalled!";
##End script