/*
 * EventScheduler.h
 *
 *  Created on: Nov 7, 2014
 *      Author: Mohammad Hashemi
 */
#include<iostream>
#include<pthread.h>
#include<map>
using namespace std;
#ifndef EVENTSCHEDULER_H_
#define EVENTSCHEDULER_H_

class Event{
public:
	int eid;
	int timeout;
	void *arg;
	void (*func_ptr)(void *);
	pthread_t* thread;
	Event(void (*func_ptr)(void *), void* arg, int eid, int timeout,pthread_t* thread) {
		this->arg = arg;
		this->func_ptr = func_ptr;
		this->eid = eid;
		this->timeout = timeout;
		this->thread=thread;

	}

	void run() {
		(*func_ptr)(arg);
	}

};

class EventScheduler{
public:
	map<int,Event*> myMap;
//	pthread_mutex_t m_lock;
	EventScheduler(size_t maxEvents=10){
		this->maxEvents=maxEvents;
		avail_eid=maxEvents;
		for(int i=0;i<maxEvents;i++){
			myMap[i]=0;
		}
//		pthread_mutex_init(&m_lock, NULL);
	}
	~EventScheduler(){
//		cerr<<"deleted"<<endl;
		for(int i=0;i<maxEvents;i++){
			if(myMap[i]!=0){

				void* result;
				int ret = pthread_join(*(*(myMap[i])).thread, &result);
		//		cout << "pthread_join()on "<<i<<" returned " << ret << ": " << strerror(errno) << endl;
//				pthread_cond_broadcast(&m_cond_var);
			}
		}
	}
	int eventSchedule(void evFunction(void *), void *arg, int timeout);
	void eventCancel(int eventId);

	int avail_eid;
private:
	size_t maxEvents;
};




#endif /* EVENTSCHEDULER_H_ */
