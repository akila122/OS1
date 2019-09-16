//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    KernelEv.cpp                            ****
//****             Info:    KernelEv class Source                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "KernelEv.h"
#include "PCB.h"
#include "Schedule.h"
#include "IVTEntry.h"
#include "Thread.h"
#include <stdio.h>
#include "EvList.h"


EvList* KernelEv::allEvents = new EvList();

KernelEv::KernelEv(IVTNo _ivtNo, Event* _myEvent) {				//Entangling KernelEv and IVTEntry object,
																//but only if no other Event is listening
	myPCB = (PCB*)PCB::running;									//to this IVTEntry
	myID = _myEvent->myID;
	ivtNo = _ivtNo;

	IVTEntry::getIVTEnt(ivtNo)->setEve(this);

	isBlocked = 0;
	isSignaled = 0;
	myEvent = _myEvent;
	allEvents->addAtEnd(this);

}

KernelEv::~KernelEv() {

	if (IVTEntry::IVT[ivtNo]->myEvent == this) IVTEntry::IVT[ivtNo]->myEvent = 0;

	signal();													//Release if someone is blocked

	allEvents->removeEv(this);
}

void KernelEv::wait() {

	if (myPCB != PCB::running) return;


	if( isSignaled ){
		isSignaled = 0;
	}
	else if (!isBlocked){   									//Always true
		myPCB->myState = BLOCKED;
		isBlocked = 1;
		System::dispOnDemand = 1;
	}

}

void KernelEv::signal() {


	if(isBlocked){
		isBlocked = 0;
		isSignaled = 0;
		myPCB->myState = READY;
		Scheduler::put(myPCB);
	}
	else isSignaled = 1;

}
