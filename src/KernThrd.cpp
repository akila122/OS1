//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    KernThrd.cpp                            ****
//****             Info:    KernThrd class Source                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "KernThrd.h"
#include <iostream.h>
#include "PcbList.h"
#include "SemList.h"
#include "EvList.h"
#include "Schedule.h"
#include "Idle.h"

volatile Sys_callData* KernelThread::sys_callParam = 0;
volatile KernelThread* KernelThread::runningKernelThread = 0;
volatile int KernelThread::retVal = 0;
volatile unsigned KernelThread::hostGuard = 1;
volatile int KernelThread::toRet = 0;

pInterrupt KernelThread::stolenInterrupt1 = 0;
pInterrupt KernelThread::stolenInterrupt2 = 0;
PCB* KernelThread::pcbToProcess = 0;
KernelEv* KernelThread::evToProcess = 0;
KernSem* KernelThread::semToProcess = 0;

KernelThread::KernelThread() {

	myPCB = new PCB(4096,0,0);

	myPCB->myState = KERNEL;

	myPCB->myStack[myPCB->stackSize - 1] = 0x200;
	myPCB->myStack[myPCB->stackSize - 2] = FP_SEG(&kernelRun);
	myPCB->myStack[myPCB->stackSize - 3] = FP_OFF(&kernelRun);

	myPCB->SS = FP_SEG(myPCB->myStack + myPCB->stackSize - 12);
	myPCB->SP = FP_OFF(myPCB->myStack + myPCB->stackSize - 12);
	myPCB->BP = myPCB->SP;

}

KernelThread::~KernelThread() {

	delete myPCB;

}

void KernelThread::setUpKernel() {

	if(runningKernelThread == 0){

		DISABLE_INTERRUPTS

			runningKernelThread = new KernelThread();

			stolenInterrupt1 = getvect(0x61);

			stolenInterrupt2 = getvect(0x63);

			setvect(0x61,&userToKernel);

			setvect(0x63,&kernelToUser);


		ENABLE_INTERRUPTS
	}

}

void KernelThread::shutdownKernel() {

	DISABLE_INTERRUPTS

	if(runningKernelThread){

		delete runningKernelThread;

	}
	ENABLE_INTERRUPTS

}

void interrupt KernelThread::userToKernel(...) {

	static unsigned tss,tbp,tsp;
	static unsigned tdx,tcx;

	asm mov tdx,dx
	asm mov tcx,cx

	sys_callParam = (Sys_callData*)(MK_FP(tcx,tdx));

	System::dispLock = 0;

	asm sti

#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}
#endif

	PCB::running->SS = tss;
	PCB::running->SP = tsp;
	PCB::running->BP = tbp;

	tss = runningKernelThread->myPCB->SS;
	tsp = runningKernelThread->myPCB->SP;
	tbp = runningKernelThread->myPCB->BP;

#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
#endif


}

void interrupt KernelThread::kernelToUser(...) {

	static unsigned tss,tbp,tsp;
	static unsigned oldSS,oldBP,oldSP;
	static unsigned retVal2;

	if (toRet){

		toRet = 0;
		retVal2 = retVal;

		tss = PCB::running->SS;
		tsp = PCB::running->SP;
		tbp = PCB::running->BP;

		asm mov oldSS, ss;
		asm mov oldBP, bp;
		asm mov oldSP, sp;

		asm mov ss,tss;
		asm mov sp,tsp;
		asm mov bp,tbp;

		asm push ax;
		asm mov ax, retVal2;
		asm mov bp[16], ax;
		asm pop ax;

		asm mov ss, oldSS;
		asm mov sp, oldSP;
		asm mov bp, oldBP;

	}

	if(System::dispOnDemand){		    //If dispatch was demanded while executing Sys_call

		if(PCB::running->myState==READY && PCB::running->myType!=LAZY) Scheduler::put((PCB*)PCB::running);

		PCB::running = Scheduler::get();

		if(PCB::running == 0) PCB::running = Idle::Singleton->idlePCB;

		System::dispOnDemand = 0;
	}

	delete (void*)sys_callParam;     	// System call was served so we can free heap.


	System::dispLock = 1;				// PSWI is 0, but now we release disLock so timer can
										// execute Context Switch when needed but only after iret.
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}
#endif

	runningKernelThread->myPCB->SS = tss;
	runningKernelThread->myPCB->SP = tsp;
	runningKernelThread->myPCB->BP = tbp;

	tss = PCB::running->SS;
	tsp = PCB::running->SP;
	tbp = PCB::running->BP;

#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
#endif


}

void KernelThread::kernelRun() {

	while(hostGuard){

		switch(sys_callParam->myType){		//Analyzing current system call.

				case DISPATCH:

					System::dispOnDemand = 1;

					break;

				case THRD_CRT:

					new PCB(sys_callParam->mySS,sys_callParam->myTS,sys_callParam->myThrd);

					break;

				case THRD_DST:

					pcbToProcess = PCB::allPCB->findByID(sys_callParam->callerID);

					delete pcbToProcess;

					pcbToProcess = 0;

					break;

				case THRD_START:

					pcbToProcess = PCB::allPCB->findByID(sys_callParam->callerID);

					pcbToProcess->start();

					pcbToProcess = 0;

					break;

				case THRD_SLP:

					PCB::sleep(sys_callParam->myTS);

					break;

				case THRD_WTC:

					pcbToProcess = PCB::allPCB->findByID(sys_callParam->callerID);

					pcbToProcess->waitToComplete();

					pcbToProcess = 0;

					break;

				case SEM_CRT:

					new KernSem(sys_callParam->semInit,sys_callParam->mySem);

					break;

				case SEM_DST:

					semToProcess = KernSem::allSem->findByID(sys_callParam->callerID);

					delete semToProcess;

					semToProcess = 0;

					break;

				case SEM_WAIT:

					semToProcess = KernSem::allSem->findByID(sys_callParam->callerID);

					retVal = semToProcess->wait(sys_callParam->toBlock);

					toRet = 1;

					semToProcess = 0;

					break;

				case SEM_SIG:
//ADD RET

					semToProcess = KernSem::allSem->findByID(sys_callParam->callerID);

					semToProcess->signal();

					semToProcess = 0;

					break;

				case SEM_VAL:
//ADD RET
					semToProcess = KernSem::allSem->findByID(sys_callParam->callerID);

					retVal = semToProcess->val();

					toRet = 1;

					semToProcess = 0;

					break;

				case EV_CRT:

					new KernelEv(sys_callParam->myIVTNo,sys_callParam->myEvent);

					break;

				case EV_DST:

					evToProcess = KernelEv::allEvents->findByID(sys_callParam->callerID);

					delete evToProcess;

					evToProcess = 0;

					break;

				case EV_WAIT:

					evToProcess = KernelEv::allEvents->findByID(sys_callParam->callerID);

					evToProcess->wait();

					evToProcess = 0;

					break;

				case EV_SIG:

					evToProcess = KernelEv::allEvents->findByID(sys_callParam->callerID);

					evToProcess->signal();

					evToProcess = 0;

					break;

				}


		asm int 63h;

	}

}
