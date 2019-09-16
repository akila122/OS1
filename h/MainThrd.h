//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    MainThrd.h                              ****
//****             Info:    MainThrd class Header                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================


#ifndef MAINTHRD_H_
#define MAINTHRD_H_
#include "Thread.h"


class MainThrd : public Thread { 			//Constructed as Singleton so user can't violate main program

private:

	static int shutdownMainThrd();
	static void setUp(int argc,char* argv[]);
	static void execute();


	MainThrd(int argc, char* argv[]);
	~MainThrd();
	void run();

	static int retVal;
	int argc;
	char** argv;
	static MainThrd* mainSingleton;

	//Friendships
	friend int main(int,char**);

};

#endif
