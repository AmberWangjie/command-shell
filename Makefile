all: myShell perror sortline 

myShell: myShell.h redipipe.h execute.h myShell.cpp 
	g++ -o myShell -ggdb3 -Wall -Werror -pedantic -std=gnu++11 myShell.cpp


perror: perror.cpp 
	g++ -o perror -ggdb3 -Wall -Werror -pedantic -std=gnu++11 perror.cpp


sortline: sortline.cpp
	g++ -o sortline -ggdb3 -Wall -Werror -pedantic -std=gnu++11 sortline.cpp

.PONEY:clean
clean:
	rm -f myShell *.o *.cpp~ *.h~
	rm -f perror *.o *.cpp~ *.h~
	rm -f sortline *.o *.cpp~ *.h~
