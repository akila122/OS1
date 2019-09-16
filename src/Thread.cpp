//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Thread.cpp                              ****
//****             Info:    Thread class Source                     ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "Thread.h"
#include "PCB.h"
#include "KernThrd.h"


ID Thread::idCntr = 0;


void dispatch(){

	DISABLE_INTERRUPTS

		PREPARE_FOR_DISPATCH

		SYSTEM_CALL

	ENABLE_INTERRUPTS
}

void Thread::start() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_THRD_START

		SYSTEM_CALL

	ENABLE_INTERRUPTS


}

void Thread::waitToComplete() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_THRD_WTC

		SYSTEM_CALL

	ENABLE_INTERRUPTS
}

Thread::~Thread() {

	waitToComplete();

	DISABLE_INTERRUPTS

		PREPARE_FOR_THRD_DST

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

void Thread::sleep(Time timeToSleep) {

	DISABLE_INTERRUPTS

		PREPARE_FOR_THRD_SLP

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

Thread::Thread(StackSize stackSize, Time timeSlice) {

	DISABLE_INTERRUPTS

		myID = ++idCntr;

		PREPARE_FOR_THRD_CRT

		SYSTEM_CALL

	ENABLE_INTERRUPTS
}

void Thread::wrapper(Thread* running) {

	running->run();									  //PCB finishIt() callback function called by assembly

}
