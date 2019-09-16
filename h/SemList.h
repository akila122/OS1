//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    SemList.h                               ****
//****             Info:    SemList class Header                    ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef SEMLIST_H_
#define SEMLIST_H_


#include "System.h"

//======================================================================
//					SIMPLE LINKED-LIST IMPLEMENTATION				  //
//  				 USED FOR TRACKING ALL SEMAPHORES				  //
//======================================================================


//======================================================================
//                     		  HELPER STRUCT                           //
//======================================================================

class KernSem;

struct NodeS{

	KernSem* mySem;
	NodeS* next;

	NodeS(KernSem* newSem, NodeS* newNext = 0){
		mySem = newSem;
		next = newNext;
	}

	~NodeS(){
		mySem = 0;
		next = 0;
	}
};


//======================================================================
//         KERNEL SEMAPHORE LINKED LIST IMPLEMENTATION      	      //
//======================================================================


class SemList {

private:


	SemList();

	virtual ~SemList();

	//Commonly used methods

	SemList* addAtEnd(KernSem* newSem);
	SemList* addAtTop(KernSem* newSem);

	KernSem* delAtTop();
	KernSem* delAtEnd();

	void removeSem(KernSem* toRemove);

	KernSem* findByID(ID toFind);

	KernSem* peekAtTop() const {
		if(head) return head -> mySem;
		else return 0;
		}
	KernSem* peekAtEnd() const{
		if(tail) return tail -> mySem;
				else return 0;
		}
	int getLen() const{
		return len;
	}

	NodeS  *head, *tail;
	int len;

	friend class KernSem;
	friend class KernelThread;


};
#endif
