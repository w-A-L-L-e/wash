/*=============================================================================
author        :Walter Schreppers
filename      :lexer.h
description   :Split an input stream up into tokens, eat up white space and
               comments and keep track of row and column
bugreport(log):column will not be incremented enough when numbers are read
=============================================================================*/

#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

#include <string>
#include <fstream>
#include "number.h"
#include "tokens.h"

using namespace std;


class Lexer {
  
  public:
    
    //constructor and destructor
    //==========================
    Lexer( ifstream& );
    Lexer( istream& );
    ~Lexer();
  
    //public members
    //==============
    token lex(); //return token
    int getRow();
    int getCol();
    
    
  private:
    
    //private members
    //===============
    int getChar();
    void ungetChar(int);
    void skipComment();
    void skipWhite();
    void checkKeywords(token&);
    int getNumber(Number&);
    int getName(string&);
    void getStringConstant(token& t);
  
    //private locals
    //==============
    istream* in;
    int row,col,prevCol;
};


#endif


