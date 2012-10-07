wash
====

wash is bash written like I want it to be. About a decade ago started to write my own little interpreter wsbasic to learn how to write a compiler without lex/yacc. Always had the intention to make it a bash alternative, but never got around to it. This weekend finally started to implement it.

Walters-MacBook-Pro:wash wschrep$ make
g++  -O2 -Wall -c number.cpp
g++  -O2 -Wall -c lexer.cpp
g++  -O2 -Wall -c parser.cpp
g++  -O2 -Wall -c treenode.cpp
g++  -O2 -Wall -c executer.cpp
g++  -O2 -Wall main.cpp -o wsbasic lexer.o parser.o treenode.o executer.o number.o -lncurses
g++  -O2 -Wall -c wash.cpp
g++  -O2 -Wall -o wash wash.o number.o lexer.o parser.o treenode.o executer.o -lreadline

Walters-MacBook-Pro:wash wschrep$ ./wash 

WASH is an awesome bash alternative written by Walter Schreppers on a sunday 7/10/2012 ;)
Typing quit is the only way to exit !

wash$ ls
Makefile     examples     executer.h   lexer.cpp    lexer.o      ntree.cpp    number.h     parser.cpp   parser.o     treenode.cpp treenode.o   wash.cpp     wsbasic
README.md    executer.cpp executer.o   lexer.h      main.cpp     number.cpp   number.o     parser.h     tokens.h     treenode.h   wash         wash.o
wash$ begin println("hello world") println(3*4) end
hello world
12

wash$ exit

Walters-MacBook-Pro:wash wschrep$


