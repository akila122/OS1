//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    KernThrd.h                              ****
//****             Info:    KernThrd class Header                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef KERNTHRD_H_
#define KERNTHRD_H_

#include "System.h"
#include "PCB.h"
#include "KernSem.h"
#include "KernelEv.h"
#include <dos.h>


enum Sys_callType{

	DISPATCH,
	THRD_CRT,
	THRD_DST,
	THRD_START,
	THRD_WTC,
	THRD_SLP,
	SEM_CRT,
	SEM_DST,
	SEM_WAIT,
	SEM_SIG,
	SEM_VAL,
	EV_CRT,
	EV_DST,
	EV_WAIT,
	EV_SIG,


};

struct Sys_callData{


	Sys_callType myType;

	ID callerID;

	Thread* myThrd;

	Semaphore* mySem;

	Event* myEvent;

	StackSize mySS;

	Time myTS;

	int semInit;

	int toBlock;

	IVTNo myIVTNo;

	//Constructor made as ugliest as possible
	Sys_callData(Sys_callType newCall, ID caller = 0,
				Thread* newThrd = 0, StackSize newSS = 0, Time newTime = 0,
				Semaphore* newSem = 0, int newSI = 0,int _toBlock = 0,
				Event* newEv = 0,IVTNo newNo = 0){

		myType = newCall;
		callerID = caller;

		myThrd = newThrd;
		mySS = newSS;
		myTS = newTime;

		mySem = newSem;
		semInit = newSI;
		toBlock = _toBlock;

		myEvent = newEv;
		myIVTNo = newNo;


	}

};

class KernelThread {												// Singleton so user cannot violate Kernel
																	// 0x61,0x63 IVTEntry taken.
private:

	KernelThread();
	virtual ~KernelThread();

	static void setUpKernel();
	static void shutdownKernel();

	static void interrupt userToKernel(...);
	static void interrupt kernelToUser(...);

	static void kernelRun();

	static volatile int toRet;
	static volatile Sys_callData* sys_callParam;
	static volatile KernelThread* runningKernelThread;
	static volatile int retVal;
	static volatile unsigned hostGuard; 							// Guarding against HostSystems
																	// while(1) termination.
	static pInterrupt stolenInterrupt1;
	static pInterrupt stolenInterrupt2;
	static PCB* pcbToProcess;										// KernelData static pointers to
	static KernSem* semToProcess;									// process in Kernels run.
	static KernelEv* evToProcess;

	PCB* myPCB;

	//Friendships

	friend int main(int,char**);

};


//======================================================================
//				DEFINITIONS OF MACROS THAT WILL BE USED               //
//  			  FOR SYSTEM CALLS AND THEIR PREPARATION			  //
//======================================================================

#define SYSTEM_CALL\
	asm push dx;\
	asm push cx;\
	unsigned tcx,tdx;\
	tcx = FP_SEG(data);\
	tdx = FP_OFF(data);\
	asm mov cx, tcx;\
	asm mov dx, tdx;\
	asm int 61h;\
	asm pop cx;\
	asm pop dx;\

#define GET_RET\
	asm mov ret, ax;\
	asm pop ax\

#define PREPARE_FOR_DISPATCH Sys_callData* data = new Sys_callData(DISPATCH);

#define PREPARE_FOR_THRD_CRT Sys_callData* data = new Sys_callData(THRD_CRT,myID,this, stackSize, timeSlice);

#define PREPARE_FOR_THRD_DST Sys_callData* data = new Sys_callData(THRD_DST,myID);

#define PREPARE_FOR_THRD_START Sys_callData* data = new Sys_callData(THRD_START,myID);

#define PREPARE_FOR_THRD_WTC Sys_callData* data = new Sys_callData(THRD_WTC,myID);

#define PREPARE_FOR_THRD_SLP Sys_callData* data = new Sys_callData(THRD_SLP,0,0,0,timeToSleep);

#define PREPARE_FOR_SEM_CRT Sys_callData* data = new Sys_callData(SEM_CRT,myID,0,0,0,this,init);

#define PREPARE_FOR_SEM_DST Sys_callData* data = new Sys_callData(SEM_DST,myID);

#define PREPARE_FOR_SEM_WAIT Sys_callData* data = new Sys_callData(SEM_WAIT,myID,0,0,0,0,0,toBlock);

#define PREPARE_FOR_SEM_SIG Sys_callData* data = new Sys_callData(SEM_SIG,myID);

#define PREPARE_FOR_SEM_VAL Sys_callData* data = new Sys_callData(SEM_VAL,myID);

#define PREPARE_FOR_EV_CRT Sys_callData* data = new Sys_callData(EV_CRT,myID,0,0,0,0,0,0,this,ivtNo);

#define PREPARE_FOR_EV_DST Sys_callData* data = new Sys_callData(EV_DST,myID);

#define PREPARE_FOR_EV_WAIT Sys_callData* data = new Sys_callData(EV_WAIT,myID);

#define PREPARE_FOR_EV_SIG Sys_callData* data = new Sys_callData(EV_SIG,myID);

#define SET_UP_DUMMY\
	PCB* dummyPCB = new PCB(4096,2,0);\
	dummyPCB->myState = READY;\
	PCB::running = dummyPCB;\

#define SHUTDOWN_DUMMY delete dummyPCB;

#endif
