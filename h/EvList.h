//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    EvList.h                                ****
//****             Info:    EvList class Header                     ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef EVLIST_H_
#define EVLIST_H_

#include "KernelEv.h"



//======================================================================
//				SIMPLE LINKED-LIST IMPLEMENTATION	                  //
//  			  USED FOR TRACKING ALL EVENTS						  //
//======================================================================


//=====================================================================
//                       HELPER STRUCT                               //
//=====================================================================

class KernSem;

struct NodeE{

	KernelEv* myEv;
	NodeE* next;

	NodeE(KernelEv* newEv, NodeE* newNext = 0){
		myEv = newEv;
		next = newNext;
	}

	~NodeE(){
		myEv = 0;
		next = 0;
	}
};

//===================================================================
//         KERNEL EVENT LINKED LIST IMPLEMENTATION                 //
//===================================================================

class EvList {

public:


	EvList();

	virtual ~EvList();

	//Commonly used methods

	EvList* addAtEnd(KernelEv* newEv);
	EvList* addAtTop(KernelEv* newEv);

	KernelEv* delAtTop();
	KernelEv* delAtEnd();

	void removeEv(KernelEv* toRemove);

	KernelEv* findByID(ID toFind);

	KernelEv* peekAtTop() const {
		if(head) return head -> myEv;
		else return 0;
		}
	KernelEv* peekAtEnd() const{
		if(tail) return tail -> myEv;
				else return 0;
		}
	int getLen() const{
		return len;
	}

	friend class KernelEv;

private:
	NodeE  *head, *tail;
	int len;



};

#endif
