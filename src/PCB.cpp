//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    PCB.cpp                                 ****
//****             Info:    PCB class Source                        ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "PCB.h"
#include "Schedule.h"
#include <dos.h>
#include "Thread.h"
#include "Idle.h"
#include <stdio.h>

	volatile PCB* PCB::running = 0;
	PcbList* PCB::allPCB = new PcbList();
	PcbList* PCB::sleepyList = new PcbList();

PCB::PCB(StackSize _stackSize, Time _timeSlice, Thread* newThread) {

	if(newThread) myID = newThread->myID;
	myType = (_timeSlice) ? FINITE : INFINITE;
	myState = CREATED;
	myThread = newThread;

	sleepingTime = 0;
	timeSlice = _timeSlice;

	threadsWaiting = new PcbList();

	stackSize = _stackSize;

	if (stackSize > 65530UL) stackSize = 65530UL;

	stackSize/=sizeof(unsigned);


	myStack = new unsigned[stackSize];

	myStack[stackSize - 1] = FP_SEG(myThread);			// void Thread::wrapper(Thread*) parameter setup
	myStack[stackSize - 2] = FP_OFF(myThread);

	myStack[stackSize - 3] = FP_SEG(finishIt);			// Callback function setup
	myStack[stackSize - 4] = FP_OFF(finishIt);

	myStack[stackSize - 5] = 0x200;						// PSW setup for iret

	myStack[stackSize - 6] = FP_SEG(Thread::wrapper);	// PC setup for iret
	myStack[stackSize - 7] = FP_OFF(Thread::wrapper);

	SS = FP_SEG(myStack + stackSize - 16);				// Stack trash setup for popping registers
	SP = FP_OFF(myStack + stackSize - 16);
	BP = SP;											// BP<=SP because literally nothing ever
														// happened on this stack before
	if (myThread) PCB::allPCB->addAtEnd(this);
}

PCB::~PCB() {

	delete myStack;
	delete threadsWaiting;
	if (myThread) allPCB->removePCB(this);
}

void PCB::start() {

	if(myState == CREATED && myType != LAZY){
		myState = READY;
		Scheduler::put(this);
	}

}

void PCB::waitToComplete() {

	if (running == this || this->myState == FINISHED || this == Idle::Singleton->idlePCB ) return;

	this->threadsWaiting->addAtEnd((PCB*)running);

	running->myState = BLOCKED;

	System::dispOnDemand = 1;


}

void PCB::sleep(Time timeToSleep) {

	if(timeToSleep<=0) return;

	else{
		running->sleepingTime = timeToSleep;
		sleepyList->addSorted((PCB*)running);
		running->myState=BLOCKED;
		System::dispOnDemand = 1;
	}
}

void PCB::wrapper() {

	Thread::wrapper(running->myThread);

}

void PCB::finishIt() {

	DISABLE_INTERRUPTS

	while(running->threadsWaiting->peekAtTop()){

		PCB* temp = running->threadsWaiting->delAtTop();

		temp->myState = READY;

		Scheduler::put(temp);

		temp = 0;

	}

	running->myState = FINISHED;

	ENABLE_INTERRUPTS

	dispatch();

}
