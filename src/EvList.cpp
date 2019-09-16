//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    EvList.cpp                              ****
//****             Info:    EvList class Source                     ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#include "EvList.h"

EvList::EvList() {
	head = tail = 0;
	len=0;
}

EvList::~EvList() {

	while(head){
		NodeE* old = head;
		head = head->next;
		delete old;
	}

}


KernelEv* EvList::delAtEnd(){

	NodeE* ret = 0;
	NodeE* temp = head;
	NodeE* prev = 0;

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
		KernelEv* _temp = ret->myEv;
		delete ret;
		return _temp;
	}
	else return 0;
}

KernelEv* EvList::delAtTop(){

	NodeE* ret = head;
	if( ret ) {
		head = head->next;
		ret->next = 0;
	}
	if(!head) tail = 0;
	if(ret){
		len--;
		KernelEv* temp = ret->myEv;

		delete ret;

		return temp;
	}
	else return 0;
}

EvList* EvList::addAtTop(KernelEv* newEv) {
	if ( !newEv ) return this;
			if (!head ){
				head = tail = new NodeE(newEv);
			}
			else head = new NodeE(newEv, head);
			len++;
			return this;
}

EvList* EvList::addAtEnd(KernelEv* newEv) {
	if ( !newEv ) return this;
	if (!head ){
		head = tail = new NodeE(newEv);
	}
	else tail = tail->next = new NodeE(newEv);
	len++;
	return this;
}

void EvList::removeEv(KernelEv* toRemove) {
			NodeE* temp = head;
			NodeE* prev = 0;

			while( temp && temp->myEv != toRemove ){
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

KernelEv* EvList::findByID(ID toFind) {

	NodeE* temp = head;

	while(temp && temp->myEv->myID != toFind){
		temp = temp->next;
	}

	if(temp==0) return 0;
	else return temp->myEv;

}

