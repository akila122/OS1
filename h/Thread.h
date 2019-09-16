//**********************************************************************
//**********************************************************************
//****             Course:  IR2OS1                                  ****
//****             File:    Thread.h                                ****
//****             Info:    Thread class Header                     ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//**********************************************************************
//**********************************************************************

#ifndef THREAD_H_
#define THREAD_H_

#include "System.h"


const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2;

class PCB;
class Idle;

class Thread {

public:

	void start();

	void waitToComplete();

	virtual ~Thread();

	static void sleep(Time timeToSleep);


protected:

	friend class PCB;

	friend class Idle;

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

	virtual void run() {}


private:

	static void wrapper(Thread* running);

	static ID idCntr;

	ID myID;


};

void dispatch ();

#endif
