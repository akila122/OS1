//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Idle.cpp                                ****
//****             Info:    Idle class Source                       ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================


#include "Idle.h"
#include "PCB.h"
#include <iostream.h>
#include <stdio.h>

Idle* Idle::Singleton = 0;
unsigned Idle::hostGuard = 1;

Idle::Idle() : Thread(10000,2) {

	idlePCB = PCB::allPCB->findByID(myID);

	idlePCB->myType = LAZY;
}

void Idle::run() {
	while(hostGuard);

}

void Idle::setUpIdle() {

	if(Singleton == 0){
		Singleton = new Idle();
	}

}

void Idle::shutdownIdle() {

	if(Singleton){

		delete Singleton;
		Singleton = 0;

	}
}
