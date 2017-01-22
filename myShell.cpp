#include "myShell.h"
#include "redipipe.h"
#include "execute.h"

void l_shift(char* & p){ //a helper function to parse_line, remove the "\" by left shift
  // std::cout << "original string is [" << p << "]" << std::endl;
  size_t i = 0;
  size_t j = i+1;
  while(p[i] != '\0'){
    p[i] = p[j];
    i++;
    j++;
    // std::cout << "  p[i] is  " << p[i] << std::endl;
  }
  // std::cout << "shifted string is [" << p << "]" << std::endl;
}

int parse_line(std::string s, std::vector<std::string>& v){ //split the string by white space, push them into the vector, return val:count number of arguments
  char* l = new char[s.length()+1];
  std::strcpy(l,s.c_str());
  // const char* p_end = l;
  char* ps;
  char* pe;
  //char* command;
  int count = 0;//0=>only command without parameters//-1=>no input
  int finish = 0;
  ps = pe = l;
  //  while(*p_end != '\0'){
  // p_end++;
  // }
  while(finish == 0){
    while((*ps == ' ' && *pe == ' ') || (*ps == '\t' && *pe == '\t') ||(*ps == '\n' && *pe == '\n')){
      ps++;
      pe++;
    }
    while(*pe != ' ' && *pe != '\0' && *pe != '\n'){
      if(*pe == '\\'){
	l_shift(pe);
	pe++;
	continue;
      }
      pe++;
    }
    if(*pe == '\0' || *pe == '\n'){
      if(count == 0){
	if(pe == ps){
	  // std::cout << "debug:no arguments" << std::endl;
	  count = -1;
	  finish = 1;
	}
	else{
	  // std::cout << "debug:only one argument" << std::endl;
	  std::string cmd(ps);
	  v.push_back(cmd);
	  count = 1;
	  finish = 1;
	}
      }
      else if(count > 0){
	//	std::cout << "debug:last argument" << std::endl;
	if(pe == ps){
	  //  std::cout << "debug:both at the end of line, no more argument" << std::endl;
	  finish = 1;
	}
	else{
	  //	  std::cout << "debug:only left last argument" << std::endl;
	  std::string last(ps);
	  //  v.resize(1);
	  v.push_back(ps);
	  count++;
	  finish = 1;
	}
      }
    }
    else{
      *pe = '\0';
      count++;
      std::string arg(ps);
      v.push_back(arg);
      pe++;
      ps = pe;
      continue;
    }
  }
  // std::cout << "debug:parsed arguments number is : " << std::endl;
  if(count != (int)v.size()){
    std::cout << "count: " << count << "   v.size: " << v.size() << std::endl;
  }
  //std::cout << "debug:parsed arguments are: ";
  //for(std::vector<std::string>::iterator it = v.begin(); it!=v.end(); ++it){
  //  std::cout << "" <<*it << " " << std::endl;
  // }
  delete[] l;
  return count;
}





void prompt(){ //print the prompt before the $
  const int max_path_len = 1024;
  // const int max_name_len = 256;
  // struct passwd *pwd;
  char pathname[max_path_len];
  // pwd = getpwuid(getuid());
  if(getcwd(pathname,max_path_len) != NULL){
    printf("myShell:%s$ ", pathname);
  }
  // printf("debug:cwd: %s,length: %zu\n", pathname, strlen(pathname));
  // printf("debug:hd: %s, length: %zu\n", pwd->pw_dir,strlen(pwd->pw_dir));
}



int main(){ 
  std::vector<std::string> stack;  
  while(std::cin.good() && !std::cin.eof()){
    prompt();
    std::string s; 
    std::getline(std::cin,s);
    if((strcmp(s.c_str(), "exit") == 0) || std::cin.eof()){
      // std::cout << "debug: exit or EOF!" << std::endl;
      return EXIT_SUCCESS;
    }
    if(s.empty()){
      continue;
    }
    std::vector<std::string> v; 
    int num =  parse_line(s,v);//split the command, return the num of arguments
    if(num == -1){
      std::cout << "Expecting command input" << std::endl;
      //return -1;
      continue;
    }
    int built = built_in(v,stack);//read the command, do the builtin functions with stack 
    if(built == -1){ 
      //std::cout << "debug:something wrong in builtin" << std::endl;
      continue;
    }
    else if (built == 1){
      //std::cout << "debug:built_in function excuted well" << std::endl;
      continue;
    }
    else if(built == 0){
      int check = do_command(v,stack);//check the first command for slash or redirection or pipe, make the path complete, call the excute to check for redirection and pipe
      if(check == -1){
	//	std::cout << "debug: command doesn't exist, no need to execute it" << std::endl;
	continue;
      }
    }
  }
  return EXIT_SUCCESS;
}
