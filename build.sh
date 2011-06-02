#!/bin/bash
echo -e "\033[0m"
clear
build_it ()
{
  echo -e "\033[01;31m"
  g++ -ansi -pedantic -g -c *.cpp
  if [ "$?" != 0 ]; then
    echo -e "\033[0mTheir where build errors. See above."
    echo -e "Exiting...\033[0m"
    exit 1
  fi
  echo -e "\033[0m\033[1;34mLinking Files...\033[01;31m"
  g++ -ansi -pedantic -g -o navn *.o inireader/INIReader.cpp inireader/ini.c -lcares
  if [ "$?" != 0 ]; then
    echo -e "\033[0mTheir where linking errors. See above."
    echo -e "Exiting...\033[0m"
    exit 1
  fi
  rm -f *.o
  if [ "$?" != 0 ]; then
    echo -e "\033[0mTheir was an error removing the object files after linking (rm -f *.o)"
  fi
  echo -e "\033[0m"
  echo "<=====================================>"
  echo "Done building!"
  echo "Build finished at: `date +%T` "
  echo "<=====================================>"
  echo " "
  echo "Bash 'navn' to run the bot (./navn)"
  echo "This bot was created in C++ by Lordofsraam from Flux-Net"
  echo "and further developed by Justasic and Lordofsraam."
  echo " "
}
if [ -f navn ]; then
  echo -n "A previous build was found, would you like to overwrite it (y/n)? "; read answer
  if [ "$answer" = "y" ]; then
    clear
    echo "Removing the old Navn build..."
    rm -f navn
    if [ "$?" != 0 ]; then
      echo -e "\033[0mTheir was an error removing the old executable (rm -f navn)"
    fi
    echo "Rebuilding the Navn bot..."
    echo "Build started at: `date +%T` "
    build_it
  elif [ "$answer" = "n" ]; then
    clear
    echo "Exiting..."
    exit
  else
    clear
    echo "Exiting..."
    exit
  fi
else
  clear
  echo Building the Navn bot...
  build_it
fi
