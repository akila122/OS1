//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    PcbList.cpp                             ****
//****             Info:    PcbList class Source                    ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "PcbList.h"
#include "PCB.h"
#include "Schedule.h"
#include <stdio.h>

PcbList::PcbList() {
	Head = Tail = 0;
	Len=0;
}

PcbList::~PcbList() {

	while(Head){
		Node* Old = Head;
		Head = Head->Next;
		delete Old;
	}

}


PCB* PcbList::delAtEnd(){

	Node* Ret = 0;
	Node* Temp = Head;
	Node* Prev = 0;

	while(Temp!=Tail){
		Prev = Temp;
		Temp = Temp -> Next;
	}

	if(Temp){

		Ret = Tail;

		if (!Prev){
			Tail = Head = 0;
		}
		else{
			Tail = Prev;
			Tail->Next=0;
		}
	}
	if(Ret) Len--;
	if(Ret) {
		PCB* Temp = Ret->MyPCB;
		delete Ret;
		Ret = Prev = 0;
		return Temp;
	}
	else{
		Ret = Prev = 0;
		return 0;
	}
}

PCB* PcbList::delAtTop(){

	Node* Ret = Head;
	if( Ret ) {
		Head = Head->Next;
		Ret->Next = 0;
	}
	if(!Head) Tail = 0;
	if(Ret){
		Len--;
		PCB* Temp = Ret->MyPCB;
		delete Ret;
		Ret = 0;
		return Temp;
	}
	else{
		Ret = 0;
		return 0;
	}
}

PcbList* PcbList::addAtTop(PCB* NewPCB) {
	if ( !NewPCB ) return this;
			if (!Head ){
				Head = Tail = new Node(NewPCB);
			}
			else Head = new Node(NewPCB, Head);
			Len++;
			return this;
}

PcbList* PcbList::addAtEnd(PCB* NewPCB) {
	if ( !NewPCB ) return this;
	if (!Head ){
		Head = Tail = new Node(NewPCB);
	}
	else Tail = Tail->Next = new Node(NewPCB);
	Len++;
	return this;
}

PcbList* PcbList::addSorted(PCB* NewPCB) {

	if(!NewPCB) return this;
	if(!Head) return addAtEnd(NewPCB); 				//If the storage is empty then take the regular path


	int TimeTest = NewPCB->sleepingTime - Head->MyPCB->sleepingTime;
	if( TimeTest < 0 ){
													//Then NewPCB should become new Master for sleeping
		addAtTop(NewPCB);
		Head->Next->MyPCB->sleepingTime=-TimeTest; 	//Old Master must adapt its SleepingTime

	}
	else{											//Then insert NewPCB as a Slave

		Node* Temp = Head->Next;
		Node* Prev = Head;
													//Finding a right place for Slave to tick

		int TimeSum = Head->MyPCB->sleepingTime;	//Insertion is more complex due to easier "waking"

		while(Temp && TimeSum+Temp->MyPCB->sleepingTime <= NewPCB->sleepingTime ){

			TimeSum+=Temp->MyPCB->sleepingTime;
			Prev = Temp;
			Temp = Temp->Next;

		}
		NewPCB->sleepingTime-=TimeSum;
		Prev->Next = new Node(NewPCB,Prev->Next);
		if(!Temp) Tail = Prev->Next;
	}
	Len++;
	return this;
}

void PcbList::removePCB(PCB* toRemove) {
	Node* temp = Head;
			Node* prev = 0;

			while( temp && temp->MyPCB != toRemove ){
				prev = temp;
				temp = temp->Next;
			}

			if(temp){
				if(prev){
					prev->Next = temp->Next;
					temp->Next = 0;
					if(Tail == temp) Tail = prev;
				}
				else{
					Head = Head->Next;
					temp->Next = 0;
					if(Tail == temp) Tail = Head;
				}

				delete temp;
			}
}

void PcbList::letsTick() {
	tick();
	if(PCB::sleepyList->peekAtTop()){

		PCB::sleepyList->peekAtTop()->sleepingTime--;

		while(PCB::sleepyList->peekAtTop() && PCB::sleepyList->peekAtTop()->sleepingTime<=0){

			PCB* Temp = PCB::sleepyList->delAtTop();
			Temp->myState=READY;
			Scheduler::put(Temp);
			Temp = 0;

		}
	}
}

PCB* PcbList::findByID(ID toFind) {

	Node* temp = Head;

	while(temp && temp->MyPCB->myID!=toFind){
		temp = temp->Next;
	}

	if(!temp) return 0;
	else return temp->MyPCB;
}

