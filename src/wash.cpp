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
#include <fstream>
#include <string>
#include <vector>

#include "parser.h"
#include "executer.h"
using namespace std;

#include <sys/wait.h>         // to wait for forked process to finish
extern char **environ;        // here we will set things like PWD, PATH etc.

//We need 2 globals (the pid and command auto completion)
//The casts get rid of all warnings, but jeeez going to std::vector instead because this is uuuuugly :(
char* cmd [] ={ 
                (char*)"exit", 
                (char*)"quit", 
                (char*)"if", 
                (char*)"else", 
                (char*)"for", 
                (char*)"to", 
                (char*)"step" ,
                (char*)"foreach", 
                (char*)"in", 
                (char*)"seperated", 
                (char*)"by", 
                (char*)"while", 
                (char*)"number", 
                (char*)"string", 
                (char*)"begin", 
                (char*)"end", 
                (char*)"print", 
                (char*)"println", 
                (char*)"input", 
                (char*)"run", 
                (char*)"write", 
                (char*)"substr", 
                (char*)"or", 
                (char*)"and", 
                (char*)"not", 
                (char*)">=", 
                (char*)">", 
                (char*)"<", 
                (char*)"<=", 
                (char*)"!=", 
                (char*)"=", 
                (char*)"==", 
                (char*)"=", 
                (char*)" ", 0 };

//these will change... will rewrite this stuff
static char** my_completion(const char*, int ,int);
char*         my_generator(const char*,int);
char*         dupstr (const char*);
void*         xmalloc (int);
pid_t         pID=1;            //stores process id of forked process

//one global parser to rule them all!
Parser        wash;

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;




/*****************************************************************************************************************
Function    : rl_gets
Parameters  : 
Description : calls readline and returns a c++ std::string 
*****************************************************************************************************************/
string rl_gets (){
  /* If the buffer has already been allocated, return the memory
     to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline ("wash$ ");

  /* If the line has any text in it, save it on the history. */
  if (line_read && *line_read)
    add_history (line_read);
  
  if( !line_read )
    return "exit"; // someone pressed ctrl-d or end of file is reached here...

  return string(line_read);
}


//splits up the command in seperate strings to pass to execvp
//THIS IS STILL BUGGY but works better than having no command args!
void execute_shell( const string& command ){
  vector<string> words;
  
  //ugh been a while since i used std::string iterators, for now just forloop it with uint ;)
  //we split up our command string in words so we can create our argv array later
  unsigned int pos = 0;
  while( pos < command.size() ){
    string word="";
    while( ( pos<command.size() ) && ( !isspace(command[pos] ) ) && (command[pos]!='|') ){
      word+= command[pos];
      pos++;
    }
    words.push_back( word );
    if( command[pos] == '|' ) {
      cout << "TODO:handle pipe here!!!!" <<endl;
    } 
    pos++;//skip the space or pipe
  }

  char** argv=(char**) malloc( words.size()+1 );
  
  for( unsigned int i=0;i<words.size();i++){
    //argv[i] = dupstr( words[i].c_str() );
    argv[i] = (char*)words[i].c_str();
  }
  argv[words.size()]=(char*)NULL;

#ifdef __APPLE__
  //somehow argv is not yet build up correctly TODO HERE!!!. Also the second param is the path here, we need something like PATH=$PATH:... equivalent for wash!
  const char* path = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/opt/X11/bin:/usr/local/CrossPack-AVR/bin";
  execvP( argv[0], path, argv );
#else
  execvp( argv[0], argv );
#endif
}


/*****************************************************************************************************************
Function    : execute
Parameters  : char* with command to run
Description : This forks our application and runs the executable in foreground (it inherits the tty)
*****************************************************************************************************************/
void execute( const string& command ){

  bool shellCommand=true;
  if (pID == 0){      //child process that executes the wanted command
      #ifdef _DEBUG_
          cout << "before execlp" <<endl;
      #endif
    //shell_execute( command );
    //execlp(command.c_str(), "" ,(char*)0); //this never returns, hence the reason to fork. -> also we can't easily add arguments here we need execvP instead!
    execute_shell( command );

    if( shellCommand ){
      cerr<<"- wash: "<< command <<": command not found"<<endl;  //If it returns the process has failed to start!
    }
    else{
      cerr << wash.getErrors() << endl;
    }
     _exit(0);        // If exec fails then exit forked process.
  }
  else if (pID < 0){  //fork failed
     cerr << "Failed to fork for executing a command!" << endl;
  }
  else{               //main process
    try{
      istringstream script(command);
      //Parser wash( script );
      wash.setScript( script );
      #ifdef _DEBUG_
            cout << "will parse following: "<<string(command)<<endl;
      #endif
      while( wash.parseStatement() ){ //go and execute our expressions
        TreeNode* root=wash.getTree();
        shellCommand=false;
        #ifdef _DEBUG_
                root->showTree(root); //show parsetree
        #endif
        Executer exe(root); //execute this tree
        exe.run();          //executed our script
        //delete( root ); //we are going to handle this differently...
        break; //in future read another line here...
      }

      if(shellCommand) { //parse failed
        //Parsing failed so we run an executable now by forking!
        pID=fork();
      } 
    }
    catch (std::bad_alloc& ba) {
      cerr << "std::bad_alloc caught: " << ba.what() << endl;
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

#ifndef _DEBUG_ //for debugging we allow ctrl-c to kill the app
  signal(sig, SIG_IGN); //ignore the default behaviour
  cout << endl <<"wash$ "<<flush; //just output newline like bash does it ;)
      
  //catch next signal also
  signal( SIGINT, handleSignals );
#endif

}


void executeFile( const string& filename ){
      cout << "executing file: "<<filename<<endl;

      ifstream script(filename);
      //wash.setScript( script );
      Parser washer( script );

      #ifdef _DEBUG_
            cout << "will parse following: "<<string(command)<<endl;
      #endif

      if( washer.parse() ){
        Executer runner( washer.getTree() );
        runner.run();
      }
      
}

int main( int argc, char** argv)
{

    if( argc > 1 ){ //execute given script
      executeFile( argv[1] );
      return 0;
    }


    int exitStatus  = 0;
    string command  = "";
    rl_attempted_completion_function = my_completion;
                                                                        
    cout<<"  _____                  ____            ______   ____   ____ "        <<endl;
    cout<<" |\\    \\   _____    ____|\\   \\       ___|\\     \\ |    | |    |"  <<endl;
    cout<<" | |    | /    /|  /    /\\    \\     |    |\\     \\|    | |    |"    <<endl;
    cout<<" \\/     / |    || |    |  |    |    |    |/____/||    |_|    |"       <<endl;
    cout<<" /     /_  \\   \\/ |    |__|    | ___|    \\|   | ||    .-.    |"     <<endl;
    cout<<"|     // \\  \\   \\ |    .--.    ||    \\    \\___|/ |    | |    |"   <<endl;
    cout<<"|    |/   \\ |    ||    |  |    ||    |\\     \\    |    | |    |"       <<endl;
    cout<<"|\\ ___/\\   \\|   /||____|  |____||\\ ___\\|_____|   |____| |____|"     <<endl;
    cout<<"| |   | \\______/ ||    |  |    || |    |     |   |    | |    |"       <<endl;
    cout<<" \\|___|/\\ |    | ||____|  |____| \\|____|_____|   |____| |____|"      <<endl;
    cout<<"    \\(   \\|____|/   \\(      )/      \\(    )/       \\(     )/"     <<endl;
    cout<<"     '      )/       '      '        '    '         '     '  "         <<endl;
    cout<<"            ' Bash needs a wash after 2 decades ;)"                   <<endl;
    cout<<endl;            

    signal(SIGINT, handleSignals);

    while(true) {
        if( pID != 0 ){ //master process
          ::wait(&exitStatus); //wait for our child to finish first!!!

          //read a new command
          command = rl_gets();

          //enable auto-complete
          rl_bind_key('\t',rl_complete);

          if( command == "quit" ) break; //exit shell
          if( command == "exit" ) break; //exit shell 
          if( command.size()>0){
            add_history( command.c_str() );
            execute( command );
          }
        }
        else if (pID<0){
          cerr <<"Fork failed"<<endl;
        }
        else{ //this execute actually performs the child process!
          execute( command );
        }
    }
 
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

/*=====================================================================================================================================================
  Function    : my_generator
  Description : The generator function is called repeatedly from completion_matches (), returning a string each time. 
                
                The arguments to the generator function are text and state. text is the partial word to be completed. 
                state is zero the first time the function is called, allowing the generator to perform any necessary initialization, 
                and a positive non-zero integer for each subsequent call. 
                
                When the generator function returns (char *)NULL this signals completion_matches () that there are no more possibilities left. 
                Usually the generator function computes the list of possible completions when state is zero, and returns them one at a time on subsequent calls. 
                
                Each string the generator function returns as a match must be allocated with malloc(); 
                Readline frees the strings when it has finished with them. 
======================================================================================================================================================*/
char* my_generator(const char* text, int state){
    static int list_index, len;
    char *name;
 
    if (!state) {
        list_index = 0;
        len = strlen (text);
    }

    //basically all we need is a properly filled vector or even better a has that we can return matches with here
    //-> write this with a proper c++ class here...

    while ((name = cmd[list_index]) && cmd[list_index] != 0 ){ //add a check for end of list to avoid segfaults ;)
        list_index++; //this seems rather inefficient, looping everything in cmd every time!
 
        if (strncmp (name, text, len) == 0)
            return (dupstr(name));
    }

    /* If no names matched, then return NULL. */
    return ((char *)NULL);
 
}


//We leave the dupstr and malloc's here cause that's just how readline lib works with the my_generator it expects malloc'ed char*'s returned for matches...
char* dupstr(const char* s) {
  char *r;
 
  r = (char*) xmalloc ((strlen (s) + 1));
  strcpy (r, s);
  return (r);
}
 
void * xmalloc (int size){
    void *buf;
    buf = malloc (size);
    if (!buf) {
        fprintf (stderr, "Error: Out of memory. Exiting.'n");
        exit (1);
    }
    return buf;
}


