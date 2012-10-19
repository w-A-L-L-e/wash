/*=============================================================================
author        :Walter Schreppers
filename      :tokens.h
description   :defines all tokens our lexer can return
=============================================================================*/

#ifndef TOKENS_H 
#define TOKENS_H

enum types {
  tokIf=-40,
  tokElse,
  tokWhile,
  tokFor,
  tokTo,
  tokStep,
  tokNumber,
  tokString,
  tokId,
  tokProcId,
  tokBegin,
  tokEnd,
  tokPrint,
  tokPrintLn,
  tokInput,
    
  tokOr,
  tokAnd,
  tokNot,
  
  tokGe,
  tokGt,
  tokLe,
  tokLt,
  tokNe,
  tokEq,
  tokAssign,

  tokReturn,
  tokBreak,

  tokForEach,
  tokIn,
  tokSeperated,
  tokBy,
    
  tokRun,
  tokWrite,
  tokSubstr,
  tokEof,
  tokError

};


struct token {
  Number  val;
  string  str;
  int     type;
};

#endif

