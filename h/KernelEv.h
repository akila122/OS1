//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    KernelEv.h                              ****
//****             Info:    KernelEv class Header                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================


#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "System.h"
#include "Event.h"

class Eevent;
class PCB;
class EvList;


class KernelEv {

private:

	KernelEv(IVTNo _ivtNo, Event* _myEvent);

	virtual ~KernelEv();

	void wait ();

	void signal();

	IVTNo ivtNo;

	int isSignaled;

	int isBlocked;

	Event* myEvent;

	PCB* myPCB;

	ID myID;

	static EvList* allEvents;


	// Friendships
	friend class EvList;
	friend class IVTEntry;
	friend class KernelThread;
};

#endif
