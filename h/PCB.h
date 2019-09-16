//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    PCB.h                                   ****
//****             Info:    PCB class Header                        ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef PCB_H_
#define PCB_H_

#include "System.h"
#include "PcbList.h"
#include <dos.h>

enum ThreadState { READY, BLOCKED, CREATED, FINISHED, KERNEL };

enum ThreadType { LAZY, FINITE, INFINITE };

class Thread;

class PCB {

private:

	PCB(StackSize _stackSize = 4096 , Time _timeSlice = 2, Thread* newThread = 0);
	virtual ~PCB();

	void start();

	void waitToComplete();

	static void sleep(Time timeToSleep);

	static void wrapper();

	static void finishIt();


	//Stack info
	unsigned* myStack;
	StackSize stackSize;
	unsigned SS;
	unsigned SP;
	unsigned BP;


	//Utility times
	Time timeSlice;
	int sleepingTime;

	//Thread connections
	Thread* myThread;
	PcbList* threadsWaiting;

	//Thread states
	ThreadType myType;
	ThreadState myState;

	//Static PCB connections
	static volatile PCB* running;
	static PcbList* allPCB;
	static PcbList* sleepyList;

	//Identification
	ID myID;

	//Friendships
	friend class PcbList;
	friend class KernelThread;
	friend class Idle;
	friend class KernelEv;
	friend class KernSem;
	friend class System;
	friend int main(int,char**);

};

#endif /* PCB_H_ */
