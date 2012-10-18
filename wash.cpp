/*=============================================================================
author        : Walter Schreppers
filename      : wash.cpp
description   : Shell emulation. You can either enter commands or start typing
                a script.
bugreport(log): This is new written on a sunday and has some bugs.
                Most important todo's:
                - executing commands with arguments (now only things like ls,
                  top, vim work now but only when no extra arguments are given)
                - make our syntax a bit different (don't require begin/end for
                  main block). => Also we want it so that when you type if bla
                  it should start reading further the script until we hit the end.

                - Improve auto completion (just copied from web and made it so
                  it doesn't segfault, but we need to implement further...).
                - Improve forking (we want just like bash to show that wash is
                  the ppid).
=============================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "executer.h"
using namespace std;

#include <sys/wait.h> //to wait for forked process to finish

//these will change... will rewrite this stuff
static char** my_completion(const char*, int ,int);
char* my_generator(const char*,int);
char * dupstr (char*);
void *xmalloc (int);
 
//We need 2 globals (the pid and command auto completion)
//The casts get rid of all warnings, but jeeez going to std::vector instead because this is uuuuugly :(
char* cmd [] ={ (char*)"hello", (char*)"world", (char*)"hell" ,(char*)"word", (char*)"quit", (char*)" ", 0 };
pid_t pID=1; //stores process id of forked process

//try a c++ version here?
//vector<string> commands;
   
//one global parser to rule them all!
Parser wash;


/*****************************************************************************************************************
Function    : execute
Parameters  : char* with command to run
Description : This forks our application and runs the executable in foreground (it inherits the tty)
*****************************************************************************************************************/
void execute( char* cmd ){
  if (pID == 0){      //child process that executes the wanted command
    execlp(cmd, cmd,(char*)0); //this never returns, hence the reason to fork.

    if( wash.getErrors().size()>0 ){
      cerr << wash.getErrors() << endl;
    }
    else{
      cerr<<"- wash: "<< string(cmd)<<": command not found"<<endl;  //If it returns the process has failed to start!
    }
     _exit(0);        // If exec fails then exit forked process.
  }
  else if (pID < 0){  //fork failed
     cerr << "Failed to fork for executing a command!" << endl;
  }
  else{               //main process
      istringstream script(cmd);
      //Parser wash( script );
      wash.setScript( script );
      bool shellCommand=true;
#ifdef _DEBUG_
      cout << "will parse following: "<<string(cmd)<<endl;
#endif
      while( wash.parseStatement() ){ //go and execute our expressions
        shellCommand=false;
        TreeNode* root=wash.getTree();
#ifdef _DEBUG_
        root->showTree(root); //show parsetree
#endif
        Executer exe(root); //execute this tree
        exe.run();          //executed our script

        //delete( root );
        break; //in future read another line here...
      }

      if(shellCommand) { //parse failed
        //Parsing failed so we run an executable now by forking!
        pID=fork();
      } 

  }
}


/*****************************************************************************************************************
Function    : handleSignals 
Parameters  : int sig
Description : This catches things like ctrl-c -> we just override it and reset this custom signal handler
              that way we have bash-like behaviour and our shell only exits when you type 'quit'
*****************************************************************************************************************/
void handleSignals( int sig ){
  signal(sig, SIG_IGN); //ignore the default behaviour
  cout << endl <<"wash$ "<<flush; //just output newline like bash does it ;)
      
  //catch next signal also
  signal( SIGINT, handleSignals );
}

int main()
{
    char *buf       = NULL;
    int exitStatus  = 0;
    string command  = "";
    rl_attempted_completion_function = my_completion;
    
    cout << "WASH is an awesome bash alternative written by Walter Schreppers on a sunday 7/10/2012 ;)" <<endl;
    cout << "Typing quit or exit is the only way to exit !"<<endl;
    cout << "Auto completion is partly implemented, also command history is done with arrow keys up/down."<<endl<<endl;
    signal(SIGINT, handleSignals);

    while(true) {
        if( pID != 0 ){ //master process
          ::wait(&exitStatus); //wait for our child to finish first!!!

          //read a new command
          buf = readline("wash$ ");
          if( buf == NULL ) break;

          //enable auto-complete
          rl_bind_key('\t',rl_complete);
 
          if (strcmp(buf,"quit")==0)
            break; //exit our shell
          if (strcmp(buf,"exit")==0)
            break; //exit our shell

          if (buf[0]!=0){
            add_history(buf);
            execute( buf );
          }
        }
        else if (pID<0){
          cerr <<"Fork failed"<<endl;
        }
        else{ //this execute actually performs the child process!
          if( buf != NULL ) execute( buf );
        }
    }
 
    free(buf);
    return 0;
}
 

//todo: need to rewrite the completion function... 
static char** my_completion( const char * text , int start,  int end)
{
    char **matches;
 
    matches = (char **)NULL;
 
    //we only do this if it's with a start at 0 meaning you tab on empty prompt
    if (start == 0) matches = rl_completion_matches ((char*)text, &my_generator);
    //else //not needed, the else just borks us here
    //  rl_bind_key('\t',rl_insert);
 
    return (matches);
 
}

//todo :need to rewrite this to c++, copied+bugfixed this from the web but it's cryptic and not what we want...
char* my_generator(const char* text, int state)
{
    static int list_index, len;
    char *name;
 
    if (!state) {
        list_index = 0;
        len = strlen (text);
    }

    /*cpp version is broke :(
    string inputText(text);

    for( unsigned int i=0;i<commands.size();i++){
      //if ( commands[i] == inputText ){
        return (char*)commands[i].c_str();
      //}
      //else{
       // cout << "commands[i]="<<commands[i]<<endl;
       // cout << "inputText  ="<<inputText<<endl;
      //}
    }
    */

    //while (name = cmd[list_index]) { //original code from net
    while ((name = cmd[list_index]) && cmd[list_index] != 0 ){ //add a check for end of list to avoid segfaults ;)
        list_index++;
 
        if (strncmp (name, text, len) == 0)
            return (dupstr(name));
    }

    /* If no names matched, then return NULL. */
    return ((char *)NULL);
 
}


//dupstr and xmalloc are not needed if we write this properly...
char * dupstr (char* s) {
  char *r;
 
  r = (char*) xmalloc ((strlen (s) + 1));
  strcpy (r, s);
  return (r);
}
 
void * xmalloc (int size)
{
    void *buf;
 
    buf = malloc (size);
    if (!buf) {
        fprintf (stderr, "Error: Out of memory. Exiting.'n");
        exit (1);
    }
 
    return buf;
}


