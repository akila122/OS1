//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    IVTEntry.cpp                            ****
//****             Info:    IVTEntry class Source                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "IVTEntry.h"
#include "KernelEv.h"
#include <dos.h>
#include <stdio.h>


IVTEntry* IVTEntry::IVT[256] = {0};


IVTEntry::IVTEntry(IVTNo toSteal, pInterrupt toPlant) {

	DISABLE_INTERRUPTS									//Setting up IVT

		ivtNo = toSteal;
		stolenRoutine = getvect(ivtNo);
		setvect(ivtNo,toPlant);
		IVT[ivtNo] = this;
		myEvent = 0;

	ENABLE_INTERRUPTS


}

IVTEntry::~IVTEntry() {

	DISABLE_INTERRUPTS									//Restoring IVT

		IVT[ivtNo] = 0;
		setvect(ivtNo,stolenRoutine);
		stolenRoutine();

	ENABLE_INTERRUPTS
}

void IVTEntry::callOld() {

	if (stolenRoutine) (*stolenRoutine)();				//If stolen routine is demanded
}

void IVTEntry::sig() {

	if(myEvent) myEvent->signal(); 						//If someone is "listening"


}
