#include <iostream> //std::cout                                               
#include <fstream>  //std::ifsteam                                     
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

int main(int argc, char ** argv){
  if(argc < 1){
    std::cerr << "Usage: sortLines input\n";
    return EXIT_FAILURE;
  }
  if(argc == 1){
    std::vector <std::string> strs;
    std::string s;
    std::getline(std::cin,s);
    while(std::cin.good() && !std::cin.eof()) {
      strs.push_back(s);
      std::getline(std::cin, s);
    }
    std::sort(strs.begin(), strs.end());
    for(std::vector <std::string>::iterator it= strs.begin(); it!=strs.end(); ++it){
      std::cout << *it << std::endl;
    }
    return EXIT_SUCCESS;
  }
  if(argc > 1){
    for(int m = 1; m < argc; m++){
      std::ifstream fin;
      fin.open(argv[m],std::ifstream::in);//
      if (!fin.is_open() || fin.fail()) {
	std::cerr << "Error:" << strerror(errno) << std::endl;
	return EXIT_FAILURE;
      }
      std::vector <std::string> strs;
      std::string s;
      std::getline(fin,s);
      while(fin.good() && !fin.eof()) {
	strs.push_back(s);
	std::getline(fin, s);
      }
      std::sort(strs.begin(), strs.end());
      for(std::vector <std::string>::iterator it= strs.begin(); it!=strs.end(); ++it){
	std::cout << *it << std::endl;
      }
      fin.close();
    }
  }
  return EXIT_SUCCESS;
}
