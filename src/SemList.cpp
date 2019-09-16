//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    Semaphor.cpp                            ****
//****             Info:    Semaphore class Source                  ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "SemList.h"
#include "KernSem.h"

SemList::SemList() {
	head = tail = 0;
	len=0;
}

SemList::~SemList() {

	while(head){
		NodeS* old = head;
		head = head->next;
		delete old;
	}

}


KernSem* SemList::delAtEnd(){

	NodeS* ret = 0;
	NodeS* temp = head;
	NodeS* prev = 0;

	while(temp!=tail){
		prev = temp;
		temp = temp -> next;
	}

	if(temp){

		ret = tail;

		if (!prev){
			tail = head = 0;
		}
		else{
			tail = prev;
			tail->next=0;
		}
	}
	if(ret) {
		len--;
		KernSem* _temp = ret->mySem;
		delete ret;
		return _temp;
	}
	else return 0;
}

KernSem* SemList::delAtTop(){

	NodeS* ret = head;
	if( ret ) {
		head = head->next;
		ret->next = 0;
	}
	if(!head) tail = 0;
	if(ret){
		len--;
		KernSem* temp = ret->mySem;

		delete ret;

		return temp;
	}
	else return 0;
}

SemList* SemList::addAtTop(KernSem* newSem) {
	if ( !newSem ) return this;
			if (!head ){
				head = tail = new NodeS(newSem);
			}
			else head = new NodeS(newSem, head);
			len++;
			return this;
}

SemList* SemList::addAtEnd(KernSem* newSem) {
	if ( !newSem ) return this;
	if (!head ){
		head = tail = new NodeS(newSem);
	}
	else tail = tail->next = new NodeS(newSem);
	len++;
	return this;
}

void SemList::removeSem(KernSem* toRemove) {
			NodeS* temp = head;
			NodeS* prev = 0;

			while( temp && temp->mySem != toRemove ){
				prev = temp;
				temp = temp->next;
			}

			if(temp){
				if(prev){
					prev->next = temp->next;
					temp->next = 0;
					if(tail == temp) tail = prev;
				}
				else{
					head = head->next;
					temp->next = 0;
					if(tail == temp) tail = head;
				}

				delete temp;
			}
}

KernSem* SemList::findByID(ID toFind) {

	NodeS* temp = head;

	while(temp && temp->mySem->myID != toFind){
		temp = temp->next;
	}

	if(temp==0) return 0;
	else return temp->mySem;

}
