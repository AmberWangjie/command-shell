miniproject:
Command Shell

Jie Wang (jw505)

*************************************************************************************************************************** 
The program of my project is called myShell, which includes 4 files: myShell.cpp, myShell.h, execute.h, redipipe.h, 
Makefile(also contains the code written for the compilation of 2other test programs). In each file, there are several 
different functions, what they are used for are commented by the right hand side. I also left some of the print statement
which written to help me debug during the process of doing the project commented, from which you might see how I thought 
and constructed the whole structure of this project. 

***************************************************************************************************************************
Some Notes:
1 About the exit status:The program will only report the exit status when it exit abnormally, which means the status is not 0, 
since I found in the real command shell, if the process can be executed well, there will be no exit status report about that.
2 About the slash check: If the command is not started with a slash, it will search the environment variable, if it fails to get 
its path in PATH, the program will report the command not found.
3 Bug:Pipe cannot run correctly when dealing with moderately large files. I think the problem is that it takes a long time for 
the child process to write into the pipe, and the parent process has not enough time to wait for it. I should change the way that 
parent process wait, however, I failed to get it working(bug-free) before the due date.
4 Bug:when I did the redirection part, I did not know that in real command shell, the redirection symbol is able to be identified 
without a space before that,so there should be treated differently when doing the parse line of the command line argument, so my 
shell program would only accept the redirection requirement with at least one space before the redirection symbol.   
***************************************************************************************************************************

The test cases I have used in each step are listed as follows:

step1:
/bin/ls
/bin/cat README
/bin/rm Makefile~

step2:
//since the following 3 lines of command are all “invalid”, so the process was not run by the program, 
I just test these by printing the arguments(which were parsed and separated properly by the parse_line function and then 
put into a vector of string) out see if the white space and escapers were handled correctly
./test    a         b     23
./test  a\ b c\ \ d
./test a\ b c \ d


mkdir a\ b\ \ c
mkdir c \  d

ls

cat README

cp lsall.out lsall2.out
diff lsall.out lsall2.out



step3:

cd ..
cd ./
cd ./85_lls
cd 85_lls

//there are some invalid use of cd, I made the program report an error
cd mm
cd mp_miniproject
cd .
cd x(x is not under the current directory)
  
cd ..
cd ./mp_miniproject


cd ..
cd..
cd jw505
pushd /home/jw505/ece551/mp_miniproject
cd ../   
pushd var //invalid use of pushd, treated as an error and report to the user
dirstack
popd 
popd //keep popd until the stack is empty, the program will report a message




step4:

cat README | wc

ls -l | grep per
ls -l | grep per | wc > pipetest1.txt

ls -1 > pipetest3.txt 
cat < pipetest3.txt / cat pipetest3.txt
 
cat < ls -1 > pipetest4.txt (invalid input, error reported)
cat < ls -1(invalid input, error reported)

ls -1 | grep per > pipetest3.txt
cat pipetest3.txt
ls -1 | grep per | wc > pipetest3.txt
cat < pipetest3.txt > pipetest4.txt
diff pipetest3.txt pipetest4.txt
cat < pipetest3.txt

./perror 2> e.txt  (perror.cpp print one line to std::cerr)
cat e.txt (see if the output was the same line as the one written in perror.cpp)

./sortline sort.txt (the sortline program will read in the lines in sort.txt and sort them, print out on the shell)
./sortline sort.txt > sorttest.txt (see if the output will go to sorttest.txt instead of the screen, and the results are the same)


dd if=/dev/urandom of=temp bs=65536 count=4 
cat < temp > temp2
diff temp temp2

cat temp | wc //failed this case, program was not able to exit





