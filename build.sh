#!/bin/sh

case $1 in
        install)
                cd src
                gcc -Wall -Og -g -c -lncursesw mainmenu.c
                gcc -Wall -Og -g -lncursesw test.c mainmenu.o -o ../test
		cd ..
                msgfmt po/ru.po -o /usr/share/locale/ru/LC_MESSAGES/mmtest.mo;;
        pot) xgettext --keyword="_" -f po/POTFILES.in -D src -o po/mmtest.pot;;
        * | help) echo "\
Usage: build.sh [CMD]
Commands
        install   build test program
        pot       generate pot file
        help      show this help
";;
esac
