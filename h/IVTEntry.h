//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    IVTEntry.h                              ****
//****             Info:    IVTEntry class Header                   ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "System.h"

class KernelEv;

class IVTEntry {

public:

	IVTEntry(IVTNo toSteal, pInterrupt toPlant);

	~IVTEntry();

public:

	void callOld();

	void sig();

	pInterrupt stolenRoutine;

	IVTNo ivtNo;

	KernelEv* myEvent;


	void setEve(KernelEv* toSet){
		myEvent = toSet;
	}

	static IVTEntry* getIVTEnt(char no){
		return IVT[no];
	}

	static IVTEntry* IVT[256];					//Objects with static storage duration are zero-initialized

	//Friendships


};

#endif
