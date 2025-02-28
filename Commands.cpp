#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <string>
// added parameters

#define COMMAND_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
 
using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string &s) {
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char *cmd_line, char **args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for (std::string s; iss >> s;) {
        args[i] = (char *) malloc(s.length() + 1);
        memset(args[i], 0, s.length() + 1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char *cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char *cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h 
//function done by Ruba
SmallShell::SmallShell() : curr_prompt("smash") {}// initial value for everyline is "smash"


SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/


Command *SmallShell::CreateCommand(const char *cmd_line) {
   //SmallShell& smash_line = SmallShell::getInstance();
   string cmd_str =  string(cmd_line); // casting char to string
   char* args[COMMAND_MAX_ARGS] = { nullptr };
   _parseCommandLine(cmd_line,args);
   if (cmd_str.length() == 0 ) {
	   return nullptr;
   }
  
   if (strcmp( args[0] , "chprompt" ) == 0 ) {
	   return new ChangePromptCommand(cmd_line);
   }
   else if (strcmp( args[0] , "showpid" ) == 0 ) {
	   return new ShowPidCommand(cmd_line);
   }
   else if (strcmp(args[0] , "pwd" ) == 0 ) {
	   return new GetCurrDirCommand(cmd_line);
   }
   return nullptr;
   
   

   
    // For example:
  /*
  string cmd_s = _trim(string(cmd_line));
  string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

  if (firstWord.compare("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line);
  }
  else if (firstWord.compare("showpid") == 0) {
    return new ShowPidCommand(cmd_line);
  }
  else if ...
  .....
  else {
    return new ExternalCommand(cmd_line);
  }
  */
    return nullptr;
}

void ChangePromptCommand::Command::execute() {
	SmallShell& small_shell = SmallShell::getInstance();
	string s = getArgs(1);
	if(s.empty()) {
		small_shell.setCurrentPrompt("smash");
	}
	else {
	    small_shell.setCurrentPrompt(s);
	}
}

void SmallShell::executeCommand(const char *cmd_line) {
	Command* cmd = CreateCommand(cmd_line);
	if ( cmd ) {
     cmd->execute();
   }
   delete cmd;
    // TODO: Add your implementation here
    // for example:
    // Command* cmd = CreateCommand(cmd_line);
    // cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}
