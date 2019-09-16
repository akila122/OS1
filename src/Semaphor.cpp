/*
 * Semaphor.cpp
 *
 *  Created on: May 22, 2018
 *      Author: OS1
 */

#include "Semaphor.h"

#include "KernSem.h"
#include "System.h"
#include "KernThrd.h"

void dispatch();

ID Semaphore::idCntr = 0;

Semaphore::Semaphore(int init) {

	DISABLE_INTERRUPTS

		myID = ++idCntr;

		PREPARE_FOR_SEM_CRT

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

Semaphore::~Semaphore() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_SEM_DST

		SYSTEM_CALL

	ENABLE_INTERRUPTS

}

int Semaphore::wait(int toBlock) {

	static unsigned retVal;

	DISABLE_INTERRUPTS

		PREPARE_FOR_SEM_WAIT

		asm push ax;

		SYSTEM_CALL

		asm mov retVal, ax;

		asm pop ax;

	ENABLE_INTERRUPTS

	return (int)retVal;
}

void Semaphore::signal() {

	DISABLE_INTERRUPTS

		PREPARE_FOR_SEM_SIG

		SYSTEM_CALL

	ENABLE_INTERRUPTS
}

int Semaphore::val() const {

	static unsigned retVal;

	DISABLE_INTERRUPTS

		PREPARE_FOR_SEM_VAL

		asm push ax;

		SYSTEM_CALL

		asm mov retVal, ax;
		asm pop ax;

	ENABLE_INTERRUPTS

	return (int)retVal;

}
