WASH
====
wash is bash written like it should be in 2012! 
About a decade ago started to write my own little interpreter wsbasic to learn how to write a compiler without lex/yacc. Always had the intention to make it a bash alternative, but never got around to it. This weekend finally started to implement it.

```
Walters-MacBook-Pro:wash wschrep$ make
...
g++  -O2 -Wall -o wash wash.o number.o lexer.o parser.o treenode.o executer.o -lreadline


Walters-MacBook-Pro:wash wschrep$ ./wash 
  _____                  ____            ______   ____   ____ 
 |\    \   _____    ____|\   \       ___|\     \ |    | |    |
 | |    | /    /|  /    /\    \     |    |\     \|    | |    |
 \/     / |    || |    |  |    |    |    |/____/||    |_|    |
 /     /_  \   \/ |    |__|    | ___|    \|   | ||    .-.    |
|     // \  \   \ |    .--.    ||    \    \___|/ |    | |    |
|    |/   \ |    ||    |  |    ||    |\     \    |    | |    |
|\ ___/\   \|   /||____|  |____||\ ___\|_____|   |____| |____|
| |   | \______/ ||    |  |    || |    |     |   |    | |    |
 \|___|/\ |    | ||____|  |____| \|____|_____|   |____| |____|
    \(   \|____|/   \(      )/      \(    )/       \(     )/
     '      )/       '      '        '    '         '     '  
            ' Bash needs a wash after 2 decades ;)

wash$ 

So that's what you see after running make && ./wash

WASH is an awesome bash alternative written by Walter Schreppers on a sunday 7/10/2012 ;)
Typing quit or exit is the only way to exit !
Auto completion is partly implemented, also command history is done with arrow keys up/down.

Little demo here executing commands etc. is just like bash (you have history and autocompletion thanks to gnu read_line ):
-------------------------------------------------------------------------------------------------------------------------
wash$ ls src/
executer.cpp  executer.o  lexer.h  main.cpp  ntree.cpp   number.h  parser.cpp  parser.o  treenode.cpp  treenode.o  wash.o
executer.h    lexer.cpp   lexer.o  Makefile  number.cpp  number.o  parser.h    tokens.h  treenode.h    wash.cpp    wsbasic

wash$ vi src/executer.cpp


But ontop of that, you get a really nice scripting language: 
------------------------------------------------------------
wash$ println("hello world"); println(3*4)
hello world
12

wash$ a=run("date") println("The date is "+a)
The date is Sun Oct  7 23:15:32 CEST 2012

wash$ for i=1 to 5 print( run("date") )
Mon Oct  8 10:05:48 CEST 2012
Mon Oct  8 10:05:48 CEST 2012
Mon Oct  8 10:05:48 CEST 2012
Mon Oct  8 10:05:48 CEST 2012
Mon Oct  8 10:05:48 CEST 2012

wash$ nr=0 foreach line in run("ls -l") begin print nr println "\t"+line nr=nr+1 end
0 total 960
1 -rw-r--r--@  1 wschrep  staff     929 Oct  7 22:29 Makefile
2 -rw-r--r--   1 wschrep  staff    2295 Oct  8 10:08 README.md
3 drwxr-xr-x@ 17 wschrep  staff     578 Oct  7 22:30 examples
4 -rw-r--r--@  1 wschrep  staff   14497 Oct  7 22:36 executer.cpp
5 -rw-r--r--@  1 wschrep  staff    2458 Oct  7 22:29 executer.h
6 -rw-r--r--   1 wschrep  staff   57296 Oct 12 13:24 executer.o
7 -rw-r--r--@  1 wschrep  staff    5259 Oct 12 14:39 lexer.cpp
8 -rw-r--r--@  1 wschrep  staff    1237 Oct  7 22:29 lexer.h
9 -rw-r--r--   1 wschrep  staff   10976 Oct 12 14:39 lexer.o
10  -rw-r--r--@  1 wschrep  staff     958 Oct  7 22:44 main.cpp
11  -rw-r--r--@  1 wschrep  staff    1941 Oct  7 22:29 ntree.cpp
12  -rw-r--r--@  1 wschrep  staff    4138 Oct  7 22:43 number.cpp
13  -rw-r--r--@  1 wschrep  staff    1670 Oct  7 22:42 number.h
14  -rw-r--r--   1 wschrep  staff    9948 Oct 12 13:24 number.o
15  -rw-r--r--@  1 wschrep  staff   19073 Oct 12 14:27 parser.cpp
16  -rw-r--r--@  1 wschrep  staff    2693 Oct 12 14:12 parser.h
17  -rw-r--r--   1 wschrep  staff   34612 Oct 12 14:27 parser.o
18  -rw-r--r--   1 wschrep  staff     815 Oct  7 22:29 tokens.h
19  -rw-r--r--@  1 wschrep  staff    5289 Oct  7 22:29 treenode.cpp
20  -rw-r--r--@  1 wschrep  staff    3518 Oct  7 22:45 treenode.h
21  -rw-r--r--   1 wschrep  staff   15372 Oct 12 13:24 treenode.o
22  -rwxr-xr-x   1 wschrep  staff  115212 Oct 12 14:39 wash
23  -rw-r--r--   1 wschrep  staff    7122 Oct 12 14:28 wash.cpp
24  -rw-r--r--   1 wschrep  staff   14932 Oct 12 14:28 wash.o
25  -rwxr-xr-x   1 wschrep  staff  110080 Oct 12 14:39 wsbasic

Instead of typing begin/end you can also use the c-style brackets now:
wash$ for i=1 to 5 { println "i=",i print run("date") }
i=1
Fri Oct 12 14:50:29 CEST 2012
i=2
Fri Oct 12 14:50:29 CEST 2012
i=3
Fri Oct 12 14:50:29 CEST 2012
i=4
Fri Oct 12 14:50:29 CEST 2012
i=5
Fri Oct 12 14:50:29 CEST 2012

wash$ exit
Walters-MacBook-Pro:wash wschrep$
```

Also you now have a full calculator right in your shell, just use println to execute some expressions, here I show that operator presedence and braces work like you'd expect:

```
wash$ println ( (2*3)+4000/6 )
672.667
wash$ println ( 2*3+4000/6 )
672.667
wash$ println ( ((2*3)+4000)/6 )
667.667
wash$ println( 4006/6 )
667.667
wash$ println( 4000/6 )
666.667
wash$ println( 4000/6+6 )
672.667

Syntax etc is getting more type friendly every day, here some types of forloops:

wash$ for i=1 to 5 print run("date")
Fri Oct 19 17:28:24 CEST 2012
Fri Oct 19 17:28:24 CEST 2012
Fri Oct 19 17:28:24 CEST 2012
Fri Oct 19 17:28:24 CEST 2012
Fri Oct 19 17:28:24 CEST 2012


wash$ foreach line in run("ls -lh") println(line+"  -> you can execute something else here")
total 272  -> you can execute something else here
-rw-r--r--   1 wschrep  staff   139B Oct 19 20:48 Makefile  -> you can execute something else here
-rw-r--r--   1 wschrep  staff   6.9K Oct 20 00:34 README.md  -> you can execute something else here
drwxr-xr-x@ 18 wschrep  staff   612B Oct 16 16:12 examples  -> you can execute something else here
drwxr-xr-x  24 wschrep  staff   816B Oct 20 00:29 src  -> you can execute something else here
-rw-r--r--   1 wschrep  staff    25B Oct 16 17:22 test.b  -> you can execute something else here
-rwxr-xr-x   1 wschrep  staff   118K Oct 20 00:29 wash  -> you can execute something else here


Foreach can also get a seperated by as here:

wash$ foreach word in run("ls -lh") seperated by " " print(word+",")
total,136K
drwxrwxr-x,2,wschrep,wschrep,4.0K,Oct,19,19:37,examples
-rw-rw-r--,1,wschrep,wschrep,,139,Oct,19,17:09,Makefile
-rw-rw-r--,1,wschrep,wschrep,7.6K,Oct,22,12:05,README.md
drwxrwxr-x,2,wschrep,wschrep,4.0K,Oct,22,11:58,src
-rwxrwxr-x,1,wschrep,wschrep,113K,Oct,22,11:58,wash
,wash$


The hard way if you want to use echo (but print and println are cooler ;) 

wash$ for i=0 to 5 print run("echo 'yeah also works'")
yeah also works
yeah also works
yeah also works
yeah also works
yeah also works
yeah also works

Pressing tab on empty line for now shows you all the keywords of wash programming language (and yeah you can type f<tab> etc:
wash$ 
           !=         <          <=         =          =          ==         >          >=         and      
           begin      by         else       end        exit       for        foreach    if         in         input    
           not        number     or         print      println    quit       run        seperated  step       string   
           substr     to         while      write    
           wash$ 


```



Look in examples dir for some working scripts: it's already a complete programming language with functions and forloops etc.
It's in early alpha stage now. 

Stuff that needs work on is keeping the directory when you use cd and multi line inline scripts.
For now all needs to be a oneliner or you load it by just invoking from a file.

```
wash$ for i=1 to 4 { b=run("date") println(b) }
Wed Sep 11 17:06:04 CEST 2013

Wed Sep 11 17:06:04 CEST 2013

Wed Sep 11 17:06:04 CEST 2013

Wed Sep 11 17:06:04 CEST 2013

wash$ 
```

Ideally this should become:

```
wash$ for i=1 to 4 {
  b=run("date") 
  println(b) 
}
...
```


