#include "myShell.h"

void redirection(std::string filename, int flag){
  int fd_in;
  int fd_out;
  int fd_out_err;
  if(flag == 0) {
    fd_in = open(filename.c_str(), O_CREAT|O_RDONLY,0666);
    if(fd_in < 0){
      std::cerr << "fail to open in input redirection" << std::endl;
    }
    close(0);//?need this?
    dup2(fd_in, 0); 
    close(fd_in);
  }
  else if(flag == 1) {
    fd_out = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);//open for read&write,file do not exist create
    if(fd_out < 0){
      std::cerr << "fail to open in output redirection" << std::endl;
    }
    close(1);
    dup2(fd_out, 1);
    close(fd_out);
  }
  else if(flag == 2) {
    fd_out_err = open(filename.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666); //?
    if(fd_out_err < 0){
      std::cerr << "fail to open in output error redirection" << std::endl;
    }
    close(2);
    dup2(fd_out_err, 2);
    close (fd_out_err);
  }
  return;
}

void do_pipe(std::vector<std::string>& command_1,std::vector<std::string>& v,size_t* j, char**& envp){
  // std::cout << "debug:pid:" << getpid() << std::endl;
  int fd[2];
  // static int last_fd0 = -1;
  if(pipe(fd) == -1){ //build a pipe
    std::cerr << "Pipe Error" << std::endl;
    return;
  }
  pid_t pid;
  int status;
  pid = fork(); //fork for the child process
  char** argv_1 = new char*[command_1.size()+1];//vector=>char** for the command before pipe
  for(size_t i = 0;i < command_1.size();i++){
    argv_1[i] = new char[command_1[i].size()+1];
      if(!command_1[i].empty()){
	strcpy(argv_1[i],command_1[i].c_str());
	//	std::cout << "debug:copy into char**(argv_1): " << argv_1[i] << std::endl;
      }
  }
  argv_1[command_1.size()] = NULL;
  if(pid == -1){
    perror("fork");
    return;
  }
  else if(pid == 0){ //child process
    // if(last_fd0 != -1){
    // dup2(last_fd0,STDIN_FILENO);
    // }
    //c.push_back(pid);
    close(fd[0]);
    // close(STDOUT_FILENO);
    dup2(fd[1],1);
    close(fd[1]);
    //close(fd[0]);
    // close(fd[1]);
    execve(argv_1[0],argv_1,envp);
    perror("Child Excute Error"); //report the error when excuting the process
    for(size_t m = 0;m < command_1.size();m++){
      delete[] argv_1[m];
    }
    delete[] argv_1; 
  }
  else{
    //if(last_fd0 != -1){
    //close(last_fd0);
    // }
    close(fd[1]);
    dup2(fd[0],0);
    close(fd[0]);
    //   c.push_back(pid);
    //close(fd[1]);
    //last_fd0 = fd[0];
    for(size_t m = 0;m < command_1.size();m++){
      delete[] argv_1[m];
    }
    delete[] argv_1;
    v.erase(v.begin(),v.begin()+(*j)+1); // after executing, erase the command before pipe and the pipe symbol, search next pipe
    check_slash(v); //check for and complete the command without "/"
    // if(slash == -1){
    // std::cout << "debug:something wrong when check the slash in the first command" << std::endl;
    //  return;
    // }
    //  w = waitpid(pid,&status,0);
    if(waitpid(pid,&status,0) < 0){
    perror("waitpid");
    }
    else{
      *j = 0;
    }
  }
  return;
}
