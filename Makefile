CXX         = g++ 
FLAGS       = -O2 -Wall #-D_DEBUG_
INSTALL_DIR = /usr/local/bin/

all: wsbasic wash

number.o: number.cpp number.h
	$(CXX) $(FLAGS) -c number.cpp

lexer.o : lexer.cpp lexer.h
	$(CXX) $(FLAGS) -c lexer.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(FLAGS) -c parser.cpp

executer.o: executer.cpp executer.h
	$(CXX) $(FLAGS) -c executer.cpp

treenode.o: treenode.cpp treenode.h
	$(CXX) $(FLAGS) -c treenode.cpp
	
wsbasic: main.cpp number.o lexer.o parser.o treenode.o executer.o
	$(CXX) $(FLAGS) main.cpp -o wsbasic lexer.o parser.o treenode.o executer.o number.o -lncurses

treetest: treenode.o treetest.cpp
	$(CXX) $(FLAGS) -o treetest treetest.cpp treenode.o

wash.o: wash.cpp
	$(CXX) $(FLAGS) -c wash.cpp

wash: wash.o number.o lexer.o parser.o treenode.o executer.o
	$(CXX) $(FLAGS) -o wash $^ -lreadline

install:
	cp wsbasic $(INSTALL_DIR)

clean:
	@rm -vf *~ *.o a.out core wsbasic treetest wash
	@rm -vf scripts/*~

