/*
 * message.h
 *
 *  Created on: Nov 7, 2014
 *      Author: Mohammad Hashemi
 */
#include<iostream>
#include <vector>
#include <cstring>
using namespace std;
struct Pair{
	char *content;
	size_t len;
};

class Message
{
public:
   
    Message( );
    Message(char* msg, size_t len);
    ~Message( );
    void msgAddHdr(char *hdr, size_t length);
    char* msgStripHdr(int len);
    int msgSplit(Message& secondMsg, size_t len);
    void msgJoin(Message& secondMsg);
    size_t msgLen( );
    void msgFlat(char *buffer);

private:
//    size_t msglen;
//    char *msg_content;
    size_t totalLen;
    vector<Pair> *headers=new vector<Pair>;
};

    Message::Message()
    {
    	Pair msg;
    	msg.content=NULL;
    	msg.len=0;
    	totalLen=0;
    	headers->push_back(msg);
//	msglen = 0;
//	msg_content = NULL;
    }

    Message::Message(char* msg, size_t len)
    {
    	Pair msg_pair;
    	msg_pair.content=msg;
    	msg_pair.len=len;
    	headers->push_back(msg_pair);
    	totalLen=len;
//	msglen = len;
//	msg_content = new char[len];
//	memcpy(msg_content, msg, len);
//	msg_content=msg;
    }

    Message::~Message( )
    {
//    	while(headers->size()!=0){
//    		Pair pair=headers[headers->size()-1];
//    		headers->pop_back();
//    		delete[] pair.content;
////            delete msg_content;
//    	}
    	//cout<<"message is deleted"<<endl;
    }

void Message::msgAddHdr(char *hdr, size_t length) {

	Pair header;
	header.content=hdr;
	header.len=length;
	headers->push_back(header);
	totalLen+=length;
//
//	char *new_msg_content;
//
//	new_msg_content = new char[msglen + length];
//	memcpy(new_msg_content, hdr, length);
//	memcpy(new_msg_content + length, msg_content, msglen);
//	delete msg_content;
//	msg_content = new_msg_content;
//	msglen += length;
}

    char* Message::msgStripHdr(int len)
    {
//	char *new_msg_content;
	char *stripped_content;
	
        if ((totalLen < len) || (len == 0)) return NULL;
        size_t stripLen=0;
        Pair *header;
		stripped_content = new char[len];
		header=&(headers->at(headers->size()-1));
		if(header->len>=len){
			stripped_content=header->content;
			header->len=header->len-len;
			totalLen-=len;
			header->content=(char *)((long)(header->content)+len);
			if(header->len==0){
				//cout<<"remove a complete header"<<endl;
				headers->pop_back();
			}
			return stripped_content;
		}

        while(stripLen<len){
//        	header=headers[headers->size()-1];
        	header=&(headers->at(headers->size()-1));
        	size_t copyLen=len-stripLen>header->len?header->len:len-stripLen;
        	memcpy(stripped_content+stripLen,headers->at(headers->size()-1).content,copyLen);
        	header->len=header->len-copyLen;
        	totalLen-=copyLen;
        	stripLen+=headers->at(headers->size()-1).len;
        	if(header->len==0){
				//cout<<"remove a complete header"<<endl;
				headers->pop_back();
			}

        }
//	new_msg_content = new char[msglen - len];
//	memcpy(stripped_content, msg_content, len);
//	memcpy(new_msg_content, msg_content + len, msglen - len);
//	msglen -= len;
//	delete msg_content;
//	msg_content = new_msg_content;
		return stripped_content;
    }

    int Message::msgSplit(Message& secondMsg, size_t len)
    {

//	char *content = msg_content;
//	size_t length = msglen;

	if ((totalLen < len) || (len == 0)) return 0;

	int stripLen = 0;
	Pair *header;
//	stripped_content = new char[len];
//	header = &(headers[headers->size() - 1]);
	header=&(headers->at(headers->size()-1));
	if (header->len >= len) {
		//cout<<"here"<<endl;
		vector<Pair> *newHeader=new vector<Pair>;
		Pair newMsg;
		newMsg.content=header->content;
		newMsg.len=len;
		newHeader->push_back(newMsg);
//		size_t temp_header_len=header->len;
//		header->len-=len;
//		cout<<header->content<<endl;
		header->content=(char *)((long)(header->content)+len);
//		header->content++;
//		header->content++;
//		cout<<header->content<<endl;
		//second message:
		secondMsg.totalLen=totalLen-len;
		secondMsg.headers=headers;
//		secondMsg.headers->assign(headers);


//	    		char *buffer2=new char[190];
//		secondMsg.msgFlat(buffer2);
//		cout<<"FFFFFFFFFFFFFFF "<<buffer2<<endl;
		////////////////test::::
//	    		int len=0;
//				for(int i=0;i<headers->size();i++){
//					int index=headers->size()-i-1;
//					char *tmp = headers->at(index).content;
//					char* tempBuffer=buffer2;
//					memcpy(tempBuffer+len,tmp,headers->at(index).len);
//					cout<<"header******************** "<< index<<" = "<<tempBuffer+len<<endl;
//					len+=headers->at(index).len;
//
//				}
				////////////////////////end of test

		header->len = header->len - len;
		if (header->len == 0) {
			//cout << "remove a complete header" << endl;
			secondMsg.headers->pop_back();
		}

		totalLen=len;
		headers=newHeader;

		//if greater than one header:
		return 1;
	}

    	vector<Pair> *newVector=new vector<Pair>;
    while(stripLen<len){
//    	header=headers[headers->size()-1];
    	header=&(headers->at(headers->size()-1));

    	size_t copyLen=(int)len-stripLen>header->len?header->len:len-stripLen;
    	//cout<<"copylen is: "<<(int)len - stripLen<<"     "<< header->len<<endl;
    	stripLen+=header->len;
    	totalLen-=copyLen;
    	header->len=header->len-copyLen;
    	if(header->len==0){
    		header->len=header->len+copyLen;
			//cout<<"split: remove a complete header"<<endl;
//    	newVector.push_back(*header);
    	std::vector<Pair>::iterator it;
    	it = newVector->begin();
    	it = newVector->insert ( it , *header );
    	headers->pop_back();
		}
    	else{ //last time
    		//cout<<"header content is "<<header->content<< " "<<header->len<<endl;
    		size_t temp_header_len=header->len;
    		header->len=copyLen;
//			newVector.push_back(*header);
			std::vector<Pair>::iterator it;
			it = newVector->begin();
			it = newVector->insert ( it , *header );
			header->len=temp_header_len;
			header->content=header->content+copyLen;
			secondMsg.headers=headers;
			secondMsg.totalLen=totalLen;

			totalLen=len;
			headers=newVector;
    		//cout<<"split: !!!!!!!!!!!!!!!!!!!!!!!!!!!!    "<<newVector->size()<<endl;




			return 1;
    	}

//    	memcpy(stripped_content+stripLen,headers->at(headers->size()-1).content,copyLen);
//    	stripLen+=headers->at(headers->size()-1).len;

    }
	return 1;
    }

    void Message::msgJoin(Message& secondMsg)
    {

    	for(int i=0;i<secondMsg.headers->size();i++){
    		int index=secondMsg.headers->size()-i-1;
//    		headers->push_back(secondMsg.headers->at(index));
    		std::vector<Pair>::iterator it;
			it = headers->begin();
			it = headers->insert ( it , secondMsg.headers->at(index));
    		totalLen+=secondMsg.headers->at(index).len;
    	}

//	char *content = msg_content;
//	size_t length = msglen;
//
//	msg_content = new char[msglen + secondMsg.msglen];
//	msglen += secondMsg.msglen;
//	memcpy(msg_content, content, length);
//	memcpy(msg_content + length, secondMsg.msg_content, secondMsg.msglen);
//	delete content;
//	delete secondMsg.msg_content;
//	secondMsg.msg_content = NULL;
//	secondMsg.msglen = 0;
    }

    size_t Message::msgLen( )
    {
	return totalLen;
    }

    void Message::msgFlat(char *buffer)
    {
	//Assume that sufficient memory has been allocated in buffer
    	int len=0;
    	for(int i=0;i<headers->size();i++){
			int index=headers->size()-i-1;
			char *tmp = headers->at(index).content;
			char* tempBuffer=buffer;
			memcpy(tempBuffer+len,tmp,headers->at(index).len);
//			cout<<"header "<< index<<" = "<<buffer<<endl;
			len+=headers->at(index).len;

		}
//	memcpy(buffer, msg_content, msglen);
    }

