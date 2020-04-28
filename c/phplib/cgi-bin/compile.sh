#!/bin/bash
#!@author: Pranav Patil

echo -e "\n"
echo "  Compiling in CURRENT DIR: " $PWD

gcc -Wall -fPIC -c list.c globals.c phplib.c
gcc -Wl -shared -o libphplib.so list.o globals.o phplib.o
gcc -Wall -L$PWD cscript.c -lphplib -o cscript.cgi
export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

echo -e "\n  NOTE: \n"
echo -e "  To run the CGI script you need to login as root and append current dir path as library path to\n"
echo -e "  /etc/ld.so.conf, as tested for openSUSE 11.2. Then run command ldconfig as root to load the libraries.\n"
