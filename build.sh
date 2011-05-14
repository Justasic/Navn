echo '\033[22;37m'
clear
build_it ()
{
  echo '\033[01;31m'
  g++ -ansi -pedantic -g -o navn main.cpp Socket.cpp Sepstream.cpp INIReader.cpp ini.c -lcares
  echo '\033[22;37m'
  echo "<=====================================>"
  echo Done building!
  echo "Build finished at: `date +%T` "
  echo "<=====================================>"
  echo " "
  echo "Bash 'navn' to run the bot (./navn)"
  echo "This bot was made in C++ by Lordofsraam from Flux-Net"
  echo "and further developed by Justasic and Lordofsraam."
  echo " "
}
if [ -f navn ]; then
  echo -n "A previous build was found, would you like to overwrite it (y/n)? "; read answer
  if [ "$answer" = "y" ]; then
    clear
    echo Removing the old Navn build...
    rm -f navn
    echo Rebuilding the Navn bot...
    echo "Build started at: `date +%T` "
    build_it
  elif [ "$answer" = "n" ]; then
    clear
    echo Exiting...
    exit
  else
    clear
    echo Exiting...
    exit
  fi
else
  clear
  echo Building the Navn bot...
  build_it
fi
