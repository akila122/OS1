//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Idle.h                                  ****
//****             Info:    Idle class Header                       ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef IDLE_H_
#define IDLE_H_
#include "Thread.h"

class Idle : public Thread {		//Constructed as Singleton so user cannot violate main program

private:

	Idle();

	virtual void run();

	static void setUpIdle();

	static void shutdownIdle();

	static unsigned hostGuard; 		//Guarding against HostSystems while(1) termination

	static Idle* Singleton;

	PCB* idlePCB;

	//Friendships
	friend class KernelThread;
	friend class System;
	friend class PCB;
	friend int main(int,char**);
};

#endif /* IDLE_H_ */
