/*=============================================================================
author        :Walter Schreppers
filename      :main.cpp
description   :The original wsbasic main
               Nice and clean. Just type ./wsbasic somescript.b and it executes
               that. Look in examples for some example scripts ....
=============================================================================*/


#include "parser.h"
#include "executer.h"

int main(int argc, char** arg){
  if(argc!=2){
    cerr << "USAGE: "<<arg[0] << " <wash file>" <<endl;
    return 1;
  }
  
  ifstream in(arg[1]);
  if( !in.is_open () ){
    cerr<<"could not open file: "<<arg[1]<<endl;
    return 1;
  }
  
  Parser parser(in);
  
  if( parser.parse() ){

    TreeNode* root=parser.getTree();
    //root->showTree(root); //show parsetree
    
    Executer exe(root); //execute this tree
    exe.run();

  }
  else{
    cerr<<"Parsing failed!"<<endl;
  }
  
  return 0;
}


