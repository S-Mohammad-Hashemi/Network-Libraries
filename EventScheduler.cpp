/*
 * EventScheduler.cpp

 *
 *  Created on: Nov 7, 2014
 *      Author: Mohammad Hashemi
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <semaphore.h>

using namespace std;

#include "EventScheduler.h"

struct Pair{
	Event *e;
	EventScheduler *es;
};


void *luncher(void *ptr){
		Pair* P = (Pair*) ptr;
//		pthread_mutex_t *m_lock=(P->es->m_lock;
		usleep(P->e->timeout*1000);
//		pthread_mutex_lock(&(P->es->m_lock));
//		cout<<">>>>>>>>>>>>>>>>>>>>>.in luncher: "<<P->e->eid<<endl;
		P->e->run();
	//	Event eee();
		P->es->myMap[P->e->eid]=0;
		P->es->avail_eid++;
//		pthread_mutex_unlock(&(P->es->m_lock));
	//	pthread_exit(NULL);
		return NULL;
	}
//	pthread_t testThread;
	map<int,pthread_t> threadMap;
int id=0;
int EventScheduler::eventSchedule(void evFunction(void *), void *arg, int timeout){
		if(avail_eid<=0){
			cout<<"no event is available"<<endl;
			return -1;
		}
		pthread_t thread;

		int eid=id++;
//		while(eid<maxEvents && myMap[eid]!=0){
//			eid++;
//		}
		Event *new_event=new Event(evFunction,arg,eid,timeout,&thread);
//		pthread_mutex_lock(&m_lock);
		myMap[eid]=new_event;
//		pthread_mutex_unlock(&m_lock);
		Pair *myPair=new Pair;
		myPair->e=new_event;
		myPair->es=this;
		pthread_create(&thread,NULL,luncher,(void *)myPair);
		avail_eid--;
		threadMap[eid]=thread;
//		pthread_mutex_lock(&m_lock);
//		pthread_mutex_unlock(&m_lock);
//		cout<<"eid is "<<eid<<" " << *(int *)arg<<" "<<myMap[eid]->thread<<endl;
		return eid;
	}
	void EventScheduler::eventCancel(int eventId){
		int eid=eventId;
//		cout<<"ok  "<<eventId<<"    " << *(myMap[eventId]->thread)<<endl;
		pthread_cancel(threadMap[eventId]);
//		pthread_exit()
//		cout<<"ok"<<endl;
//		pthread_mutex_lock(&m_lock);
		myMap[eventId]=0;
		avail_eid++;
//		pthread_mutex_unlock(&m_lock);
		//////////////////////////TODO  //thread ham bayad bedam be event
	}





