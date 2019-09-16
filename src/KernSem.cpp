//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    KernSem.cpp                             ****
//****             Info:    KernSem class Source                    ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "KernSem.h"
#include "SemList.h"
#include "PcbList.h"
#include "PCB.h"
#include "Schedule.h"
#include <stdio.h>
#include "Semaphor.h"
#include <iostream.h>

SemList* KernSem::allSem = new SemList();
ID KernSem::idp = 0;

void dispatch();

KernSem::KernSem(int init, Semaphore* newSem) {

	myVal = init;
	mySem = newSem;
	myID  = newSem->myID;
	threadsWaiting = new PcbList();
	allSem->addAtEnd(this);

}

KernSem::~KernSem() {

	while(myVal<0){
		signal();
	}
	delete threadsWaiting;
	allSem->removeSem(this);

}

int KernSem::wait(int toBlock) {

	int ret;
	if(toBlock == 0 && myVal<=0) {						//Safe-mode
		ret = -1;
	}
	else{												//Blocking-mode

		if(--myVal < 0){
			threadsWaiting->addAtEnd((PCB*)PCB::running);
			PCB::running->myState = BLOCKED;
			ret = 1;
			System::dispOnDemand = 1;					//We only demand dispatch, but due to disabled preemption
		}												//it will occur after the return call.
		else ret = 0;
	}

	return ret;

}

void KernSem::signal() {

	if(myVal++<0){

		PCB* temp = threadsWaiting->delAtTop();
		temp->myState = READY;
		Scheduler::put(temp);
		temp = 0;

	}

}

int KernSem::val() const {

	return myVal;

}
