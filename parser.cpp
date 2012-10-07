#include "parser.h"

/*======================= THE GRAMMAR =============================

  BNF for arithmetic expressions (improved unary minus)

    <expression>    ::= <term> [<addop> <term>]*
    <term>          ::= <signed factor> [<mulop> <signed factor>]*
    <signed factor> ::= [<addop>] <factor>
    <factor>        ::= <integer> | <variable> | (<expression>)



  NOT DONE YET !: 
  BNF for boolean algebra

    <b-expression>::= <b-term> [<orop> <b-term>]*
    <b-term>      ::= <not-factor> [AND <not-factor>]*
    <not-factor>  ::= [NOT] <b-factor>
    <b-factor>    ::= <b-literal> | <b-variable> | (<b-expression>)

=================================================================*/

Parser::Parser( ifstream& script ){
  lexer = new Lexer( script );
  tree  = new TreeNode();
}

Parser::Parser( istream& script ){
  lexer = new Lexer( script );
  tree  = new TreeNode();
}

Parser::~Parser(){
  delete lexer;
}


void Parser::Error(const string& s){
  //We silence all errors as with wash if it's not a script it's most likely an executable like 'ls' or 'df -h' or 'ifconfig'
  //cerr<<"Error: "<< s <<" at row "<<row <<" , col "<< col <<"."<<endl;
    //exit(1); // better = throw exception here!
  bNoErrors=false;
}

void Parser::Expected(const string& s){
  Error( "Expected "+s );
}

void Parser::getToken(){
  look=lexer->lex();
  row=lexer->getRow();
  col=lexer->getCol();
}


bool Parser::parse(){
  bNoErrors=true;
  getToken();
  tree=Program();
  return bNoErrors;
}


TreeNode* Parser::getTree(){
  return tree;
}

void Parser::Match(int x){
  if(look.type!=x){
    string tokStr="";
    tokStr+= (char)x;
    
    Error("Syntax error, expected '"+tokStr+"'");
  }
  else{
    getToken(); 
  }  
}


TreeNode* Parser::signedFactor(){
  TreeNode* sfac; //used by '-' and tokNot  
  switch( look.type ){
    case '+':     Match('+');
                  return Factor();
                  break;

    case '-':     Match('-');
                  sfac=Factor();
                  if( sfac->getType() == constantNode ){
                    Number n=sfac->getValue();
                    n.val= - n.val;
                    sfac->setValue( n );
                    return sfac;
                  }
                  else{
                    TreeNode* minus= new TreeNode( minusNode, row, col );
                    minus->appendChild( sfac );
                    return minus;
                  }
                  break;

    case tokNot:  Match(tokNot);
                  sfac=Factor();
                  if( sfac->getType() == constantNode ){
                    Number n=sfac->getValue();
                    n.val= 1 - n.val;
                    sfac->setValue( n );
                    return sfac;
                  }
                  else{
                    TreeNode* n = new TreeNode( notNode, row, col );
                    n->appendChild( sfac );
                    return n;
                  }
                  break;

    default:      return Factor();
                  break;
  }
}



TreeNode* Parser::getId(){
  TreeNode* n=new TreeNode( idNode, row, col );
  n->setName( look.str );
  Match( tokId );
  return n;  
}


TreeNode* Parser::runFunction(){
  TreeNode* n=new TreeNode( runNode, row, col );
  Match(tokRun);
  Match('(');
  
  n->appendChild( Expression() );
  
  Match(')');
  return n;
}

TreeNode* Parser::writeFunction(){
  TreeNode* n=new TreeNode( writeNode, row, col );
  Match(tokWrite);
  Match('(');
  
  n->appendChild( Expression() );
  Match(',');
  n->appendChild( Expression() );
  
  Match(')');
  return n;
}

TreeNode* Parser::substrFunction(){
  TreeNode* n=new TreeNode( substrNode, row, col );
  Match(tokSubstr);
  Match('(');
  
  n->appendChild( getId() );
  Match(',');
  n->appendChild( Expression() );
  Match(',');
  n->appendChild( Expression() );
  
  Match(')');
  return n;
}


/*---------------------------------------------------------------*/
/* Parse and Translate a Math Factor */
TreeNode* Parser::Factor()
{
  TreeNode* n;
  switch( look.type ){
    case '(':       Match('(');
                    n=Expression();
                    Match(')');
                    break;

    case tokId:     n=getId();
                    if( look.type == '(' ){       //is function call
                      string name=n->getName();
                      delete n;              
                      n=FunctionCall( name );
                      n->setType(funcReturnNode); //expect returned value on stack
                    }
                    break;
                  
    case tokString: n=new TreeNode( stringConstantNode, row, col );
                    n->setValue( look.str );
                    Match(tokString);
                    break;

    case tokNumber: n=new TreeNode( constantNode, row, col );
                    n->setValue( look.val );
                    Match(tokNumber);
                    break;
    
    case tokRun:    n=runFunction();     break;
    case tokSubstr: n=substrFunction();  break;
                                    
    default:        Error("Illegal char in expression");
                    n=new TreeNode( Unknown, row, col );
                    getToken();
                    break;
  }
  return n;
}




/*---------------------------------------------------------------*/
/* identify a multiplicative operator */
bool Parser::isMulOp(token t){
  return 
    (t.type == '*') ||
    (t.type == '/') ||
    (t.type == tokAnd)
  ;
}



/*---------------------------------------------------------------*/
/* Parse and Translate a Math Term */
TreeNode* Parser::Term()
{
  TreeNode* termNode=signedFactor();
  TreeNode* pos=termNode;
  TreeNode* left=NULL;
  TreeNode* right=NULL;
  
  while( isMulOp(look) ){
    left=pos;
    pos=new TreeNode( Unknown, row, col );
    pos->appendChild( left );
        
    switch( look.type ){
      case '*':     Match('*');
                    right = signedFactor();
                    pos->setType( mulNode );
                    pos->setName( "mul" );
                    break;
      
      case '/':     Match('/');
                    right = signedFactor();
                    pos->setType( divNode );
                    pos->setName( "div" );
                    break;
      
      case tokAnd:  Match(tokAnd);
                    right = signedFactor();
                    pos->setType( andNode );
                    pos->setName( "and" );
                    break;
                    
      default:      Expected( "* or /" ); 
                    getToken();
                    return pos;
                    break;
    }
    
    if(right!=NULL) pos->appendChild( right );
    termNode=pos;
  }//end while
  
  return termNode;
}


bool Parser::isAddOp(token t){
  return 
    (t.type == '+') ||
    (t.type == '-') ||
    (t.type == tokGt)   ||
    (t.type == tokGe)   ||
    (t.type == tokLt)   ||
    (t.type == tokLe)   ||
    (t.type == tokEq)   ||
    (t.type == tokNe)   ||
    
    (t.type == tokOr )  ||
    (t.type == tokGe)
  ;
}


/*---------------------------------------------------------------*/
/* Parse and Translate an Expression */
TreeNode* Parser::Expression()
{
  TreeNode* retExp=Term();
  TreeNode* pos=retExp;
  TreeNode* left=NULL;
  TreeNode* right=NULL;

  while( isAddOp( look ) ){
    left=pos;
    pos=new TreeNode( Unknown, row, col );
    pos->appendChild( left );
      
    switch( look.type ){
      case '+':   Match('+');
                  right = Term() ;
                  pos->setType( addNode );
                  pos->setName("add");
                  break;

      case '-':   Match('-');
                  right = Term();
                  pos->setType( subNode );
                  pos->setName("sub");
                  break;

      case tokGt: Match(tokGt);
                  right = Term();
                  pos->setType( nodeGT );
                  pos->setName(">");
                  break;

      case tokLt: Match(tokLt);
                  right = Term();
                  pos->setType( nodeLT );
                  pos->setName("<");
                  break;

      case tokGe: Match(tokGe);
                  right = Term();
                  pos->setType( nodeGE );
                  pos->setName(">=");
                  break;

      case tokLe: Match(tokLe);
                  right = Term();
                  pos->setType( nodeLE );
                  pos->setName("<=");
                  break;
      
      case tokEq: Match(tokEq);
                  right = Term();
                  pos->setType( nodeEQ );
                  pos->setName("==");
                  break;

      case tokNe: Match(tokNe);
                  right = Term();
                  pos->setType( nodeNE );
                  pos->setName("!=");
                  break;
      
      case tokOr: Match(tokOr);
                  right = Term();
                  pos->setType( orNode );
                  pos->setName( "or" );
                  break;

      default :   Expected("additive operation" );
                  getToken();
                  return pos;
                  break;
    }
    if(right!=NULL) pos->appendChild( right );
    retExp=pos;
  }
    
  return retExp;
}


TreeNode* Parser::Assignment( const string& name ){
  TreeNode* assign=new TreeNode( assignNode, row, col );
  assign->setName("assignment");
  
  Match(tokAssign);

  //first child of assign is id or lhv of assignment
  TreeNode* left=new TreeNode( idNode, row, col );
  left->setName(name);
  assign->appendChild(left);
  
  //next child is expression or rhv of assignment
  TreeNode* right=Expression();
  assign->appendChild(right);

  return assign;
}



/*
  paramlist is like idlist except it does not only
  except id's it accepts expressions seperated by ','!
*/
TreeNode* Parser::ParamList(){
  TreeNode* ilist=new TreeNode( idListNode, row, col );

  //check for empty idlist -> function with no parameters
  if( look.type == ')' ) return ilist;
  
  //get id's seperated by ','
  ilist->appendChild( Expression() ); //aaah KISS (Keep It Simple Sidney)
  while( look.type == ',' ){
    Match(',');
    ilist->appendChild( Expression() ); 
  }

  return ilist;
}



/*
  <functioncall> := tokId '(' <paramlist> ')' 
*/
TreeNode* Parser::FunctionCall( const string& name ){
  TreeNode* fcall=new TreeNode( functionCallNode, row, col );
  
  //first child contains function name
  TreeNode* funcid= new TreeNode( idNode, row, col );
  funcid->setName( name );
  fcall->appendChild( funcid );
  
  Match( '(' );
  fcall->appendChild( ParamList() );
  Match( ')' );
  
  return fcall;
}


// this is either an assignment or a function call!
TreeNode* Parser::Other(){
  string idname=look.str; 
  Match(tokId);
  
  if( look.type == tokAssign ){
    return Assignment( idname );
  }
  else{
    return FunctionCall( idname );
  }
}




/*
  <while> ::= tokWhile <expression> ( <statement> | <block> )
*/
TreeNode* Parser::While(){
  TreeNode* wNode=new TreeNode( whileNode, row, col );
  wNode->setName("while");
  Match(tokWhile);
  wNode->appendChild( Expression() );
  
  if(look.type == tokBegin){ //while followed by a block
    wNode->appendChild( Block() ); 
  } 
  else{ //while followed by single statement
    wNode->appendChild( Statement() );
  }
  
  return wNode;
}






/*
  <for> ::= tokFor <id>'='<expression> tokTo <expression> (<tokStep> <expression>)? ( <statement> | <block> )
  for loops with step have 5 children
  for loops without step have 4 children
*/
TreeNode* Parser::For(){
  TreeNode* fNode=new TreeNode( forNode, row, col );
  fNode->setName("for loop");
  Match(tokFor);
  fNode->appendChild( getId() ); //loop id
  Match(tokAssign);
  
  fNode->appendChild( Expression() ); //start value expression
  Match(tokTo);
  fNode->appendChild( Expression() ); //stop value expression

  if( look.type == tokStep ){
    Match(tokStep);
    fNode->appendChild( Expression() ); //step expression
  }
    
  if(look.type == tokBegin){ //for followed by a block
    fNode->appendChild( Block() ); 
  } 
  else{ //while followed by single statement
    fNode->appendChild( Statement() );
  }
  
  return fNode;
}


/*
  tokForEach <id> tokIn <expression> (tokSeperated tokBy <expression>)? (<statement>|<block>)
  4 children 
  default seperator is "\n"
*/
TreeNode* Parser::ForEach(){
  TreeNode* fNode=new TreeNode( forEachNode, row, col );
  fNode->setName("foreach loop");

  Match(tokForEach);
  fNode->appendChild( getId() ); //loop id
  Match(tokIn);
  fNode->appendChild( Expression() ); //expression (string with \n's)
  

  if( look.type == tokSeperated ){
    Match(tokSeperated);
    Match(tokBy);
    fNode->appendChild( Expression() ); //seperator expression
  }
  else{ //default seperator "\n"
    fNode->appendChild( NewLineNode() );
  }
    
  if(look.type == tokBegin){ //for followed by a block
    fNode->appendChild( Block() ); 
  } 
  else{ //while followed by single statement
    fNode->appendChild( Statement() );
  }
  
  return fNode;
}









/*
  <if> ::= tokIf <expression> ( <statement> | <block> )
                ( tokElse ( <statement> | <block> ) )?
 
 the expression is in first child
 the first block or statement is in second child
 the else block or statement is in third child
*/
TreeNode* Parser::If(){
  TreeNode* node=new TreeNode( ifNode, row, col );
  node->setName("if");
  
  Match(tokIf);
  node->appendChild( Expression() );
  
  if(look.type == tokBegin){  //if followed by a block
    node->appendChild( Block() ); 
  } 
  else{   //if followed by single statement
    node->appendChild( Statement() );
  }
  
  if(look.type == tokElse){ //else part
    Match(tokElse);
  
    if(look.type == tokBegin){  //else is followed by block
      node->appendChild( Block() );
    }
    else{
      node->appendChild( Statement() );
    }
  
  }
  
  return node;
}


TreeNode* Parser::getString(){
  TreeNode* str = new TreeNode( stringConstantNode, row, col );
  
  str->setStrValue( look.str );
  Match( tokString );
  
  return str;
}

/*
  <print> ::= 'print' ( ( <expression> | <string> ) ',' )+
*/
TreeNode* Parser::Print(){
  TreeNode* node=new TreeNode( printNode, row, col );
  node->setName("print");
  
  getToken(); //is tokPrint or tokPrintLn
  
  //first expression
  node->appendChild( Expression() );
  
  //following strings or expressions
  while( look.type == ',' ){
    getToken(); //the comma
    node->appendChild( Expression() );
  }
  
  return node;
}


TreeNode* Parser::NewLineNode(){
  TreeNode* newline = new TreeNode( stringConstantNode, row, col );
  Number n;
  n=string("\n");
  newline->setValue( n );
  return newline;        
}

//use Print and add extra string element containing "\n"
TreeNode* Parser::PrintLn(){
  TreeNode* node=Print();
  node->setName("println");

  node->appendChild( NewLineNode() );

  return node;
}

TreeNode* Parser::Input(){
  TreeNode* inp=new TreeNode( inputNode, row, col );
  inp->setName("input");
  
  Match( tokInput ); 
  inp->appendChild( getId() ); 
  
  return inp;
}

TreeNode* Parser::Return(){
  TreeNode* ret=new TreeNode( returnNode, row, col );
  
  Match( tokReturn );
  ret->appendChild( Expression() );
  
  return ret;
}

TreeNode* Parser::Break(){
  TreeNode* brk = new TreeNode( breakNode, row, col );
  Match ( tokBreak );
  
  return brk;
}

TreeNode* Parser::Statement(){
  switch(look.type){
    case tokIf      : return If();            break;
    case tokFor     : return For();           break;
    case tokForEach : return ForEach();       break;
    case tokWhile   : return While();         break;
    case tokPrint   : return Print();         break;
    case tokPrintLn : return PrintLn();       break;
    case tokInput   : return Input();         break;
    case tokReturn  : return Return();        break;
    case tokBreak   : return Break();         break;
    case tokId      : return Other();         break; //assignment or function call

    case tokWrite   : return writeFunction(); break;
        
    case tokEnd     : break; //caught by Block
    
    case tokBegin   : Error("Begin without matching end");
                      getToken();
                      return new TreeNode(Unknown, row, col );
                      break;
                                            
    default         : break;    
  }

  Error("Incorrect statement :"+look.str); 
  getToken();
  return new TreeNode( Unknown, row, col );
}


TreeNode* Parser::Block(){
  TreeNode* block=new TreeNode( blockNode, row, col );
  block->setName("block");
  
  Match(tokBegin);
  while( (look.type!=tokEnd) && (look.type!=tokEof) ){
    block->appendChild( Statement() );
  }
  Match(tokEnd);
  
  return block;
}


TreeNode* Parser::IdList(){
  TreeNode* ilist=new TreeNode( idListNode, row, col );

  //check for empty idlist -> function with no parameters
  if( look.type == ')' ) return ilist;
  
  //get id's seperated by ','
  ilist->appendChild( getId() );
  while( look.type == ',' ){
    Match(',');
    ilist->appendChild( getId() );
  }

  return ilist;
}


/*==================================================================
   EBNF for a function 
   <function> := tokId '(' <idlist> ')' <block>

   we can safely use tokId because we require ( .. )
   to be given after the id when it is called, 
   if this were not the case
   we would have to extend the lexer so that it gives a
   tokFunctionId whenever an id has the same name as a function...
===================================================================*/
TreeNode* Parser::Function(){
  TreeNode* func=new TreeNode( functionNode, row, col );
  TreeNode* idn=getId();
  
  func->appendChild( idn );
  Match('(');
  func->appendChild( IdList() );    
  Match(')');
  func->appendChild( Block() );

  return func;
}


TreeNode* Parser::Program(){
  TreeNode* program=new TreeNode( programNode, row, col );
  program->setName("program");

  //get the functions
  while( look.type == tokId ){
    program->appendChild( Function() );
  }

  //the main, or execution starting block
  program->appendChild( Block() );
  
  return program;
}



/*
  //obsolete
  string Parser::toString( number n ){
    ostringstream os;
    os<<n;
    return os.str();
  }
*/
