//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    System.cpp                              ****
//****             Info:    System class Source                     ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================


#include <stdio.h>
#include "System.h"
#include "PCB.h"
#include "Schedule.h"
#include <dos.h>
#include "Idle.h"

	volatile unsigned int System::dispLock = 1;
	volatile unsigned int System::dispOnDemand = 0;

	volatile Time System::currentTime = 0;

	pInterrupt System::stolenInterrupt = 0;


void System::stealTimer() {

	DISABLE_INTERRUPTS

		System::stolenInterrupt = getvect(8);
		setvect(8,&myRoutine);
		setvect(0x60,stolenInterrupt);

	ENABLE_INTERRUPTS
}

void System::restoreTimer() {

	DISABLE_INTERRUPTS

		setvect(8,stolenInterrupt);

	ENABLE_INTERRUPTS
}

void interrupt System::myRoutine(...) {

	static unsigned tempSS,tempSP,tempBP; 								// Static modifier so the function
																		// stack stays untouched.

																		// Context switch made slower
		currentTime++;													// but Schedulers queue is enriched
		PCB::sleepyList->letsTick();									// (also shows better results in PublicTest)
		asm int 60h


	if(PCB::running->timeSlice!=0 && currentTime >= PCB::running->timeSlice){

		if(dispLock){
			asm {
					mov tempSP, sp
					mov tempSS, ss
					mov tempBP, bp
				}

			PCB::running->SP = tempSP;
			PCB::running->SS = tempSS;
			PCB::running->BP = tempBP;

			currentTime = 0;
			dispOnDemand = 0;

			if(PCB::running->myState == READY && PCB::running->myType!=LAZY ) Scheduler::put((PCB*)PCB::running);

			PCB::running = (volatile PCB*)Scheduler::get();

			if(PCB::running == 0){
				PCB::running=Idle::Singleton->idlePCB;
			}

			tempSP = PCB::running->SP;
			tempSS = PCB::running->SS;
			tempBP = PCB::running->BP;

			asm {
					mov sp, tempSP
					mov ss, tempSS
					mov bp,tempBP
				}

		}
		else dispOnDemand = 1;										// If Kernel is being busy then we just
	}																// demand dispatch.
}
