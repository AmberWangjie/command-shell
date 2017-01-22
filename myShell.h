#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <cerrno>
#include <iterator>
#include <fcntl.h> 
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> 

//#define MAX_PROMPT 1024
//struct passwd *pwd;


void l_shift(char* &);
int parse_line(std::string, std::vector<std::string>&);
int check_slash(std::vector<std::string>&);
int built_in(std::vector<std::string> &,std::vector<std::string> &);
void prompt();
int do_command(std::vector<std::string>&,std::vector<std::string>&);
//size_t get_pipe_num(std::vector<std::string> &);
int excute(std::vector<std::string> &);
void redirection(std::string,int);
void do_pipe(std::vector<std::string> &,std::vector<std::string> &,size_t*,char**&);

