#include "myShell.h"

int check_slash( std::vector<std::string>& v){ //check if there is "/" in first command, search and add it if none
  int check = 0;
  std::size_t find_slash = v[0].find('/');
  if(find_slash == std::string::npos){
    char* path = getenv("PATH");
    //    std::cout << "debug:PATH is " << path << std::endl;
    if(path != NULL){
      char* getpath = new char[strlen(path)+1];
      std::strcpy(getpath,path);
      char* curr = getpath;
      const char* dir;
      while((dir = strsep(&getpath,":")) != NULL){
	std::string dir_name(dir);
	//	std::cout << "debug:get a dir: " << dir_name << std::endl;
	DIR* d = opendir(dir);
	if(d == NULL){
	  std::cerr << "error to open the dir( " << errno << " )" << dir_name << std::endl;
	  check = -1;
	  return check;
	}
	struct dirent* ent = NULL;
	do{
	  ent = readdir(d);
	  if(ent != NULL){
	    std::string name(ent->d_name);
	    if(name.compare(v[0]) == 0){
	      check = 1;
	      // std::cout << "debug:debug:program found!" << std::endl;
	      std::string fullname = dir_name + "/" + name;
	      v[0].assign(fullname);
	      //  std::cout << "debug: the first argument(v[0]) now is " << v[0] << std::endl;
	      //
	      closedir(d);
	      delete[] curr;
	      //free(d);
	      return check;
	    }
	    else{
	      continue;
	    }
	  }
	}while(ent != NULL);
	closedir(d); //?
	//free(d);
      }
       if(check != 1){
	std::cout << "Command " << v[0] << " not found" << std::endl;
	check = -1;
	delete[] curr;
	return check;
       }
    }
    else{
      // std::cerr << "debug: failed to get the PATH" << std::endl;
      check = -1;
      return check;
    }
  }
  else{
    // std::cout << "debug:contain slash, no change needed" << std::endl;
    check = 0;
  }
  return check;
}

int built_in(std::vector<std::string> & v, std::vector<std::string> & stack){ //excute the built_in command
  int contain = 0;
  if(v[0].compare("exit") == 0){
    return -1;
  }
  if(v[0].compare("cd") == 0){
    contain = 1;
    std::string cd_path;
    cd_path.assign(v[1]);
    //std::cout << "debug: cd path is " << cd_path << std::endl;
    if(chdir(cd_path.c_str()) != 0){
      std::cerr << "change directory failed, please enter the correct path name" << std::endl;
      return -1;
    }
  }
  if(v[0].compare("pushd") == 0){
    contain = 1;
    const int max_path_len = 1024; //assume there is size limit for path string
    char pathname[max_path_len];
    getcwd(pathname,max_path_len);
    std::string directory(pathname);
    // std::cout << "debug:the curr dir is " << directory << std::endl;
    stack.push_back(directory);
    //  std::cout << "debug:the last in the stack is: " << stack.back() << std::endl;
    if(v[1].at(0) == '/'){
      if(chdir(v[1].c_str()) != 0){
	std::cerr << "cd fail when pushd" << std::endl;
	return -1;
      }
    }
    else{
      std::cerr << "Usage of pushd:absolute path" << std::endl;
      return -1;
    }
  }
  if(v[0].compare("popd") == 0){
    contain = 1;
    //  std::cout << "popd command" << std::endl;
    if(!stack.empty()){
      //  std::cout << "ready to popd" << std::endl;
      if(chdir(stack.back().c_str()) != 0){
	std::cerr << "cd failed when popd" << std::endl;
      }
      stack.pop_back();
    }
    else{
      std::cout << "can't do popd: the stack of directory is empty now!" << std::endl;
    }
  }
  if(v[0].compare("dirstack") == 0){
    contain = 1;
    // std::cout << "dirstack command" << std::endl;
    if(!stack.empty()){
      // std::cout << "ready to dirstack" << std::endl;
      for(std::vector<std::string>::iterator it = stack.begin();it != stack.end(); ++it){
	std::cout << *it << std::endl;
      }
    }
    else{
      std::cout << "can't do dirstack: nothing in the dir stack" << std::endl;
    }
  }
  return contain;
}

/* size_t get_pipe_num(std::vector<std::string>& v){ */
/*   size_t num = 0; */
/*   for(size_t i = 0; i < v.size(); i++){ */
/*     if(v[i].compare("|") == 0){ */
/*       num++; */
/*     } */
/*   } */
/*   return num; */
/* } */

int execute(std::vector<std::string>& v){ //run the child process, check for redirection and pipe, change the command if needed
  // std::vector<pid_t> children;
  //  size_t num = get_pipe_num(v);
  char** envp = NULL;
  pid_t cpid;
  pid_t wpid;
  int status;
  cpid = fork();
  if(cpid == -1){
    perror("fork");
    return -1;
  }
  else if(cpid == 0){ //first check for redirection / pipe command,call its corresponding implementation function
    for(size_t i = 0; i < v.size(); i++){
      if(v[i].compare("<") == 0){
	redirection(v[i+1],0);
	//	std::cout << "debug:indirection:input file is " << v[i] << std::endl;
	v.erase(v.begin()+i,v.begin()+i+2);
	i = 0;
	continue;
      }
      else if(v[i].compare(">") == 0){
	redirection(v[i+1],1);
	//	std::cout << "debug:outdirection:output file is " << v[i] << std::endl;
	v.erase(v.begin()+i,v.begin()+i+2);
	i = 0;
	continue;
      }
      else if(v[i].compare("2>") == 0){
      	redirection(v[i+1],2);
	//	std::cout << "debug:out_error_direction:output file is " << v[i] << std::endl;
	v.erase(v.begin()+i,v.begin()+i+2);
	i = 0;
	continue;
      }
      else if(v[i].compare("|") == 0){ //maybe should put pipe seperately before the fork?                                      //otherwise after it forks in its own do_pipe functi                                      //on,there will be two myShell process
	//	count_pipe++;
	//std::cout << "debug: There is pipe" << std::endl;
	std::vector<std::string> command_1; //command line before the pipe symbol
	for(size_t j = 0; j < i; j++){
	  command_1.push_back(v[j]);
	}
	//std::cout << "debug:The command line before pipe is: ";
	/* for(std::vector<std::string>::iterator it = command_1.begin(); it != command_1.end(); ++it){ */
	/*   std::cout <<  " " << *it << " "; 
	 if(it == command_1.end()-1){
	 std::endl;
	 }*/
	/* } */
	//std::vector<pid_t> children;
	do_pipe(command_1,v,&i,envp);
	/* if(children.size() != num){ */
	/*   continue; */
	/* } */
	/* else{ */
	/*   int status_pipe; */
	/*   for(size_t i = 0; i < children.size(); i++){ */
	/*     if(waitpid(children[i],&status_pipe,0) < 0){ */
	/*       perror("waitpid"); */
	/*     } */
	/*     else{ */
	/*       continue; */
	/*     } */
	/*   } */
	continue;
      }
    }
    char** argv = new char*[v.size()+1];//vector=>char**
    for(size_t m = 0;m < v.size();m++){
      argv[m] = new char[v[m].size()+1];
      if(!v[m].empty()){
      strcpy(argv[m],v[m].c_str());
      // std::cout << "copy into char**(argv): " << argv[m] << std::endl;
      }
    }
    argv[v.size()] = NULL;
    /* std::cout << "The argument list(no pipe) is: "; */
    /* for(size_t m = 0; m < v.size(); m++){ */
    /*   std::cout << argv[m] << ";" ; */
    /*   if(m == v.size()-1){ */
    /* 	std::cout << argv[m] << std::endl; */
    /*   } */
    /* } */
    execve(argv[0],argv,envp);
    perror("excute error");
    for(size_t n = 0;n < v.size();n++){
      delete[] argv[n];
    }
    delete[] argv; 
  }
  else{
    do{
      wpid = waitpid(cpid,&status,WUNTRACED | WCONTINUED);
      if(wpid == -1){
	perror("waitpid");
	return(EXIT_FAILURE);
      }
      else if(WIFEXITED(status)){
	if(status != 0){
	  printf("Program exited with status %d\n", WEXITSTATUS(status));
	}
      }
      else if(WIFSIGNALED(status)){
	printf("Program was killed by signal %d\n", WTERMSIG(status));
      }
    }while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

int do_command(std::vector<std::string>& v,std::vector<std::string>& stack){ //get the input command, call the check_slash function, excute function

  int check_forward_slash = check_slash(v);//check_slash(v);//check"/", if no, search and change  
  if(check_forward_slash == -1){ 
    /*   std::cout << " debug:the command has been found" << std::endl; */
    return -1;//no need to execute this wrong command
  } 
    /* else if(check_forward_slash == 0){ */
    /*   std::cout << "debug:command was began with a slash, no need to found" << std::endl; */
    /* } */
    
    execute(v);//use execve to excute the process, also check for redirection and pipe, call their function if needed

    /* int exc = excute(v);
       if(exc == 1){ */
    /*   std::cout << "debug:process was  excuted" << std::endl; */
    /* } */
    return 0;
}
