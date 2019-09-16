//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    PcbList.h                               ****
//****             Info:    PcbList class Header                    ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef PCBLIST_H_
#define PCBLIST_H_

#include "System.h"

//======================================================================
//				SIMPLE LINKED-LIST IMPLEMENTATION					  //
// 				  USED FOR TRACKING ALL THREADS						  //
//  					AND SLEEPING CYCLES							  //
//======================================================================


//======================================================================
//                       HELPER STRUCT                           	  //
//======================================================================

class PCB;

struct Node{

	PCB* MyPCB;
	Node* Next;

	Node(PCB* newPCB, Node* newNext = 0){
		MyPCB = newPCB;
		Next = newNext;
	}

	~Node(){
		MyPCB = 0;
		Next = 0;
	}
};


//======================================================================
//               PCB LINKED LIST IMPLEMENTATION                		  //
//======================================================================


class PcbList {

private:

	PcbList();

	virtual ~PcbList();

	//Commonly used methods

	PcbList* addAtEnd(PCB* NewPCB);
	PcbList* addAtTop(PCB* NewPCB);

	PCB* delAtTop();
	PCB* delAtEnd();

	void removePCB(PCB* toRemove);

	PCB* findByID(ID toFind);

	PCB* peekAtTop() const {
		if(Head) return Head -> MyPCB;
		else return 0;
		}
	PCB* peekAtEnd() const{
		if(Tail) return Tail -> MyPCB;
				else return 0;
		}
	int getLen() const{
		return Len;
	}

	//SleepyList methods
	PcbList* addSorted(PCB* NewPCB);
	void letsTick();

	Node  *Head, *Tail;
	int Len;

	//Friendships
	friend class Idle;
	friend class PCB;
	friend class KernSem;
	friend class KernelThread;
	friend class System;


};

#endif
