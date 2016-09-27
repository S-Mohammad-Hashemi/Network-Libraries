/*
 * ThreadPool.h
 *
 *  Created on: Nov 7, 2014
 *      Author: Mohammad Hashemi
 */
#include<pthread.h>
#include<iostream>
#include<vector>
#include<semaphore.h>
using namespace std;
#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#define DEFAULT_SIZE 10
class Work{
public:
	Work(void (*func_ptr)(void *) , void* arg){
		this->arg=arg;
		this->func_ptr=func_ptr;
	}
	void run(){
		(*func_ptr)(arg);
	}
private:
	void *arg;
	void (*func_ptr)(void *);
};
class ThreadPool{
public:
	//constructor:
	ThreadPool(size_t threadCount = DEFAULT_SIZE);

	//Destructor
	~ThreadPool();

	int dispatch_thread(void dispatch_function(void*),void *arg);
	bool thread_avail();
	int start_thread();
private:
	size_t threadCount;
	pthread_t *thread_array;
//	sem_t sem_queue;
	pthread_mutex_t m_lock;
	pthread_cond_t m_cond_var;
	vector<Work> workQueue;
//	sem_t sem_empty;
//	sem_t sem_avail;
	pthread_mutex_t avail_lock;
	int avail_threads;
	volatile bool terminate=false;

};



#endif /* THREADPOOL_H_ */
