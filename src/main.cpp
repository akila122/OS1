//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    main.cpp       	                        ****
//****             Info:    Main program initialization             ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================


#include "MainThrd.h"
#include "PCB.h"
#include "System.h"
#include "KernThrd.h"
#include "Idle.h"
#include "Semaphor.h"
#include <iostream.h>

int userMain (int argc, char* argv[]);			// All function declarations have an implicit extern applied.


int main(int argc, char* argv[]){

	SET_UP_DUMMY								//Setting up dummyPCB for main to use.

	KernelThread::setUpKernel();				// Systems runningKernelThread must be constructed first,
												// we assume that no other user Thread was statically initialized.

	Idle::setUpIdle();							// Constructing Idle singleton so CPU can be occupied while
												// everyone is sleeping.

	MainThrd::setUp(argc,argv);					// Constructing a Thread that dummyPCB will wait on.

	System::stealTimer();						// Patching Timers routine (55ms).

	MainThrd::execute();						// Waiting for users main to finish.

	System::restoreTimer();						// Restoring Timers routine.

	int ret = MainThrd::shutdownMainThrd();		// Extracting result that users main has returned.

	Idle::shutdownIdle();						// Deallocating heap.

	KernelThread::shutdownKernel();				// Disabling runningKernelThread.

	SHUTDOWN_DUMMY								// Deleting dummyPCB.

	return ret;

}


