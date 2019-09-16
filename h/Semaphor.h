//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Semaphor.h                              ****
//****             Info:    Semaphore class Header                  ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_
#include "System.h"

class KernSem;

class Semaphore {
public:

 Semaphore (int init=1);

 virtual ~Semaphore ();

 virtual int wait (int toBlock=1);

 virtual void signal();

 int val () const;

protected:

 friend class KernSem;

private:

 ID myID;

 static ID idCntr;

};

#endif
