//======================================================================
//======================================================================
//****             Course:  IR2OS1                                  ****
//****             File:    System.h                                ****
//****             Info:    System class Header                     ****
//****                                                              ****
//****             Student: Aleksa Rajkovic 248|2016      	        ****
//****             Date:    May 2018.                               ****
//======================================================================
//======================================================================

#ifndef SYSTEM_H_
#define SYSTEM_H_


//======================================================================
//		  					 USEFUL TYPES AND						  //
//  				MECHANISM FOR DISABLING PREEMPRION				  //
//     					WITHOUT DISABLING INTERRUPTS,				  //
//	 					INCLUDING MASKABLE INTERRUPTS				  //
// 		 					 CONTROL MECHANISM 						  //
//======================================================================

//======================================================================
//								TYPEDEFS						  	  //
//======================================================================

typedef unsigned long StackSize;

typedef unsigned int Time;

typedef int ID;

typedef unsigned char IVTNo;

typedef void interrupt (*pInterrupt)(...);  //Interrupt pointer


//======================================================================
//							 	 MACROS							  	  //
//======================================================================


#define DISABLE_DISPATCH System::dispLock = 0;
#define ENABLE_DISPATCH System::dispLock= 1; if (System::dispOnDemand) dispatch();

#define DISABLE_INTERRUPTS asm pushf; asm cli;
#define ENABLE_INTERRUPTS asm popf;


void tick();

//======================================================================
//         SYSTEMS GLOBAL DATA AND TIMER ROUTINE ENCAPSULATION    	  //
//======================================================================


class System {

private:

//======================================================================
//            	  USED FOR SETTING UP TIMER ROUTINE          		  //
//======================================================================

	static void stealTimer();
	static void restoreTimer();
	static void interrupt myRoutine(...);

	static pInterrupt stolenInterrupt;

//======================================================================
//                       SHARED VAR.             	                  //
//======================================================================
public:

	static volatile unsigned int dispLock;
	static volatile unsigned int dispOnDemand;

	static volatile Time currentTime;

	//Friendships
	friend class KernelEv;
	friend class KernelThread;
	friend int main(int,char**);


};

#endif
