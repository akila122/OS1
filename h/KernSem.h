//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    KernSem.h                               ****
//****             Info:    KernSem class Header                    ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "System.h"

class Semaphore;
class SemList;
class PcbList;

class KernSem {

private:

	KernSem(int init, Semaphore* newSem);

	virtual ~KernSem();

	virtual int wait (int toBlock);

	virtual void signal();

	int val () const;

	int myVal;

	Semaphore* mySem;

	static SemList* allSem;

	PcbList* threadsWaiting;

	static ID idp;

	ID myID;

	// Friendships
	friend class KernelThread;
	friend class SemList;

};

#endif /* KERNSEM_H_ */
