#!/bin/sh
cd src
gcc -Wall -Og -g -c -lncursesw mainmenu.c
gcc -Wall -Og -g -lncursesw test.c mainmenu.o -o ../test
