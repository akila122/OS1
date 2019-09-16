//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    MainThrd.cpp                            ****
//****             Info:    MainThrd class Source                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "MainThrd.h"

int userMain(int argc, char* argv[]);

MainThrd* MainThrd::mainSingleton = 0;
int MainThrd::retVal = 0;

void MainThrd::execute() {

	mainSingleton->waitToComplete();

}

void MainThrd::run() {

	retVal = userMain(argc,argv);

}

MainThrd::MainThrd(int _argc, char* _argv[]) {

	argc = _argc;
	argv = _argv;


}

MainThrd::~MainThrd() {

	waitToComplete();

}

int MainThrd::shutdownMainThrd(){

	if(mainSingleton){

		delete mainSingleton;
		mainSingleton = 0;
	}
	return retVal;

}

void MainThrd::setUp(int argc, char* argv[]) {

	if(mainSingleton == 0){
		mainSingleton = new MainThrd(argc,argv);
		mainSingleton->start();
	}
}
