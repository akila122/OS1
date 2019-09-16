//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Event.cpp                               ****
//****             Info:    Event class Source                      ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "Event.h"
#include "System.h"
#include "KernelEv.h"
#include <stdio.h>
#include "KernThrd.h"

void dispatch();

ID Event::idCntr = 0;


Event::Event(IVTNo ivtNo) {

	DISABLE_INTERRUPTS

		myID = ++idCntr;

		PREPARE_FOR_EV_CRT

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

void Event::wait() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_EV_WAIT

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

Event::~Event() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_EV_DST

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

void Event::signal() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_EV_SIG

		SYSTEM_CALL

	ENABLE_INTERRUPTS
}
