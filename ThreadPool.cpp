/*
 * ThreadPool.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: Mohammad Hashemi
 */

#include "ThreadPool.h"

#include <errno.h>
#include <string.h>

using namespace std;
void *luncher(void *ptr){
	ThreadPool* tp = (ThreadPool*) ptr;
	tp->start_thread();
	return NULL;
}
ThreadPool::ThreadPool(size_t threadCount){
//	pthread_mutex_init(mutex_queue, NULL);
//	sem_init(&sem_queue, 0, 1);
	pthread_mutex_init(&m_lock, NULL);
	pthread_mutex_init(&avail_lock, NULL);
//	sem_init(&sem_avail, 0, 1);
//	sem_init(&sem_empty, 0, 0);
	pthread_cond_init(&m_cond_var, NULL);
	this->threadCount=threadCount;
	cout<<"threadCount is: "<<threadCount<<endl;
	thread_array=new pthread_t[threadCount];
	for(unsigned int i=0;i<threadCount;i++){
		int error = pthread_create(&thread_array[i],NULL,luncher,(void *)this);
		if(error!=0){
			cerr<<"failed to create a new thread "<<error<<endl;
		}

	}

	avail_threads=threadCount;
	cout<<"avail threads = "<<avail_threads<<endl;
}

ThreadPool::~ThreadPool(){
	cout<<"end of threadPool"<<endl;
	pthread_mutex_lock(&m_lock);
	terminate=true;
	pthread_mutex_unlock(&m_lock);
	pthread_cond_broadcast(&m_cond_var);
//	adding start or stop state
	for(int i=0;i<threadCount;i++){
		void* result;
		int ret = pthread_join(thread_array[i], &result);
//		cout << "pthread_join()on "<<i<<" returned " << ret << ": " << strerror(errno) << endl;
		pthread_cond_broadcast(&m_cond_var);
	}

	pthread_cond_destroy(&m_cond_var);
	pthread_mutex_destroy(&m_lock);
	pthread_mutex_destroy(&avail_lock);
//	sem_destroy(&sem_queue);
//	sem_destroy(&sem_empty);
//	sem_destroy(&sem_avail);
	delete[] thread_array;
}

int ThreadPool::start_thread(){
	int sss=0;
	while(true){
//		cout<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS "<<"   "<<sss++<<endl;

//		sem_wait(&sem_queue);
	    pthread_mutex_lock(&m_lock);
		bool empty=0;
//		bool queueIsEmpty=workQueue.empty();
		while( !terminate  && workQueue.empty()){
//		sem_post(&sem_queue);
//			avail_threads++;
//			sem_post(&sem_queue);
			pthread_cond_wait(&m_cond_var, &m_lock);
//			sem_wait(&sem_empty);
			empty=1;
//			int value;
//			sem_getvalue(&sem_empty,&value);
//			cout<<"#%%%%%%%%%%%%%%%%%%%%######   "<<value<<endl;

		}
//		if(empty==0){
//			sem_post(&sem_queue);
//		}

//		cout<<"after queue is empty"<<endl;
//		sem_wait(&sem_queue);

		if (terminate) {
			pthread_mutex_unlock(&m_lock);
		    pthread_exit(NULL);
		}


		Work* running_work=&(workQueue[workQueue.size()-1]);
		workQueue.pop_back();
//		sem_post(&sem_queue);
	    pthread_mutex_unlock(&m_lock);

//		sem_wait(&sem_avail);

		pthread_mutex_lock(&avail_lock);
		avail_threads--;
//		cout<<"avail threads = "<<avail_threads<<endl;
//		sem_post(&sem_avail);
		pthread_mutex_unlock(&avail_lock);

		running_work->run();

//		sem_wait(&sem_avail);
		pthread_mutex_lock(&avail_lock);
		avail_threads++;
//		cout<<"avail threads after work = "<<avail_threads<<endl;
//		sem_post(&sem_avail);
		pthread_mutex_unlock(&avail_lock);

	}
	return 0;
}

int ThreadPool::dispatch_thread(void dispatch_function(void*),void *arg){
	Work new_work(dispatch_function,arg);
//	sem_wait(&sem_queue);
    pthread_mutex_lock(&m_lock);
	workQueue.push_back(new_work);
//	int value;
//	sem_getvalue(&sem_empty,&value);
//	cout<<"######################   "<<value<<endl;
//	sem_post(&sem_empty); //signal threads to run a work
	pthread_cond_signal(&m_cond_var);
//	sem_post(&sem_queue);
    pthread_mutex_unlock(&m_lock);


	return 0;
}

bool ThreadPool::thread_avail(){
	bool result=false;
//	sem_wait(&sem_avail);
	pthread_mutex_lock(&avail_lock);
	if(avail_threads>0){
		result=true;
	}
//	cout<<avail_threads<<endl;
//	sem_post(&sem_avail);
	pthread_mutex_unlock(&avail_lock);
	return result;

}


