//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Event.h                                 ****
//****             Info:    Event class Header                      ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef EVENT_H_
#define EVENT_H_
#include "System.h"
#include "IVTEntry.h"
#include <stdio.h>

class KernelEv;

class Event {

public:

	 Event (IVTNo ivtNo);

	 ~Event ();

	 void wait ();

protected:

	 friend class KernelEv;

	 void signal();

private:

	 ID myID;

	 static ID idCntr;

};


#define PREPAREENTRY(ivtNo,oldflag)\
void interrupt myint##ivtNo(...);\
IVTEntry myentry##ivtNo(ivtNo, myint##ivtNo);\
void interrupt myint##ivtNo(...){\
	if(oldflag) myentry##ivtNo.callOld();\
	myentry##ivtNo.sig();\
	if(System::dispLock) dispatch();\
}

//PITAJ ZA MAKRO I DISPATCH

#endif
