

#ifndef _C_Cache_
#define _C_Cache_


#include <math.h>
#include <stdint.h>
#include <iostream>
#include "CC.h"



using namespace std;


const uint64_t MainMemorySize = 4294967296; //4GB 

const uint64_t CacheSize = 256*1024;
const uint64_t Associativity = 8;
const uint64_t BlockSize = 64;
const uint64_t NoOfSets = (uint32_t)CacheSize/(BlockSize*Associativity);
const uint64_t BytesPerSet = MainMemorySize/NoOfSets;

class CounterCache 
{

public:

	uint64_t total_hit,total_miss,total_evicted;
	uint8_t LRU_Counter[NoOfSets][Associativity];
	uint8_t DirtyBit[NoOfSets][Associativity];
	uint8_t CacheUsageBit[NoOfSets][Associativity];
	uint64_t Cached[NoOfSets][Associativity];


	CounterCache(){

cout << "initializing cache" << endl;
total_hit=0,total_miss=0,total_evicted=0;
for(int set = 0; set < NoOfSets; set++){
		for(int i=0;i<Associativity;i++){
			LRU_Counter[set][i]=i;
			DirtyBit[set][i]=0;
			CacheUsageBit[set][i]=0;
			Cached[set][i]=0;
		}
	
}


}




	////

	uint64_t PushInCache(uint64_t address, char c);
	void UpdateLRU(uint64_t set, uint64_t position);
	uint8_t LRU(uint64_t address);
	uint8_t getPositionInSet(uint64_t address);
	bool hasCounter(uint64_t address);
	bool isCacheHit(uint64_t address);
	bool isCacheSetFull(uint64_t address);
	uint32_t evictOne(uint64_t address);
	void setDirtyBit(uint32_t set, uint32_t position);
	void resetDirtyBit(uint32_t set, uint32_t position);
	void setUsageBit(uint32_t set, uint32_t position);
	void resetUsageBit(uint32_t set, uint32_t position);
	void printLruCounter(uint32_t set);



private:

protected:

};


uint64_t
CounterCache::PushInCache(uint64_t address, char c){


uint32_t set = (int)(address / BytesPerSet);
uint64_t position = LRU(address);

uint64_t pageID = (int)((address)/(64*64));

cout << "\n\n------putting counter in the cache pageID:"<< pageID << " set " << set << " pos " << position <<endl;


uint64_t evicted=0xffffffff;

if(this->isCacheHit(address)){

	cout << "dirty update " << endl;
	


    	this->Cached[set][this->getPositionInSet(address)]=pageID;
    	
    	if(c=='w'){
    		//cout << "**************************incrementing counter <<<<<<<<<" << endl;
    		this->setDirtyBit(set,this->getPositionInSet(address));
    	}
    	this->setUsageBit(set,this->getPositionInSet(address));
		//this->UpdateLRU(set, this->getPositionInSet(address));
		//this->printLruCounter(set);
	
}
else if(this->isCacheSetFull(address)){
	    cout << " cache set full " << endl;
		uint32_t evict_pos=this->evictOne(address);
		evicted=Cached[set][evict_pos];
		this->Cached[set][evict_pos]=pageID;
		this->resetDirtyBit(set,evict_pos);
    	//this->resetUsageBit(set,evict_pos,offset);
    	this->UpdateLRU(set,evict_pos);
    	//this->printLruCounter(set);
    	//this->removeTag(set,evict_position,offset);

    }
else{

	cout << "new update " << endl;

	this->Cached[set][this->LRU(address)]=pageID;
	if(c=='w'){
		this->resetDirtyBit(set,LRU(address));
	}
    
    this->setUsageBit(set,this->LRU(address));
    //this->printLruCounter(set);
	this->UpdateLRU(set, this->LRU(address));
	//this->printLruCounter(set);

}


return evicted;


}


uint8_t
CounterCache::getPositionInSet(uint64_t address)
{

uint32_t set = (int)(address / BytesPerSet);
uint64_t pageID = (int)((address)/(64*64));

for(int k = 0; k<Associativity; k++){
	if(this->Cached[set][k]==pageID) {

		return k;
}


}


return 0;

}

uint8_t
CounterCache::LRU(uint64_t address){

	uint32_t set = (int)(address / BytesPerSet);
	for(int i=0;i<Associativity;i++){
		if(LRU_Counter[set][i]==0) {
			//cout << "returning " << i << endl;
			return i;
		}

	}
	
	return 0;
}

void
CounterCache::UpdateLRU(uint64_t set, uint64_t position){

	//cout << "LRU set "<<set<<" position " << position; 

	int prev_position = this->LRU_Counter[set][position];
	this->LRU_Counter[set][position]=7;
	for(int i=0; i<Associativity; i++){
		if(i==position || this->LRU_Counter[set][i]==0) continue;
		if(this->LRU_Counter[set][i]>prev_position) this->LRU_Counter[set][i]--;

	}

	//cout << "LRU set "<<set<<" "; 

	for(int i=0;i<Associativity;i++){
		cout<< (int)LRU_Counter[set][i] << " ";
	}

	cout << endl;
}



bool
CounterCache::hasCounter(uint64_t address)
{
	uint32_t set = (int)(address / BytesPerSet);
if(this->isCacheHit(address)){
	//cout << " has counter in cache"<< endl;
	total_hit++;
	UpdateLRU(set, getPositionInSet(address));
	return true;
} 
else{
	//cout << " no counter in cache"<< endl;
	total_miss++;
	return false;
} 
}


bool
CounterCache::isCacheHit(uint64_t address)
{
	//cout << "cache hit " << endl;
uint32_t set = (int)(address / BytesPerSet);

uint64_t pageID = (int)((address)/(64*64));

//cout << "checking cache hit "  << set << " " << pageID << endl;

for(int i = 0; i<Associativity; i++){
	//cout << " checking " << (int)Cached[set][i] << " with " << pageID << endl;
	if((int)this->Cached[set][i]==(int)pageID) {
        //cout << " hit and returning true"<< endl;
		//updateLruCounter(set,i,offset);
		return true;
}
}
//cout << "returning false"<< endl;
return false;
}


bool 
CounterCache::isCacheSetFull(uint64_t address){
	uint32_t set = (int)(address / BytesPerSet);
	for(int i = 0; i<Associativity; i++){
		if(CacheUsageBit[set][i]==0){
			//cout << "cache set is not full" << endl;
			return false;
		}
	}
	//cout << "cache FULL" << endl;
	return true;
}


uint32_t 
CounterCache::evictOne(uint64_t address){
	uint32_t set = (int)(address / BytesPerSet);
	uint32_t evict_position=0;
	uint32_t to_evict=0;
	uint32_t previous_position=Associativity+1;
	bool atLeastOneDirty = false;

	for(int i=0; i<Associativity; i++){
		if(DirtyBit[set][i]==1) atLeastOneDirty=true;
	}


	if(atLeastOneDirty){
		for(int i=0; i< Associativity; i++ ){
		if(DirtyBit[set][i]==0) {
			//cout << "abandoning " << i << endl; 
			continue;
		}
		else{
			if((int)LRU_Counter[set][i]<previous_position){
				evict_position = (int)LRU_Counter[set][i];
				to_evict = i;
				//cout << "lru count " << i << " is " << (int)LRU_Counter[set][i] << " evict position " << evict_position  <<" to evict " << i << endl;
			}
			previous_position=evict_position;
		}
	}
	}
	else{
		to_evict = LRU(address);
	}
	

    //cout << "evicting " << evict_position << " of set " << set << endl; 

    total_evicted ++;

    return to_evict;
}


void 
CounterCache::setDirtyBit(uint32_t set, uint32_t position){
	DirtyBit[set][position]=1;

}

void 
CounterCache::resetDirtyBit(uint32_t set, uint32_t position){
	DirtyBit[set][position]=0;

}

void 
CounterCache::setUsageBit(uint32_t set, uint32_t position){
	CacheUsageBit[set][position]=1;

}

void 
CounterCache::resetUsageBit(uint32_t set, uint32_t position){
	CacheUsageBit[set][position]=0;

}

void CounterCache::printLruCounter(uint32_t set)
{
	cout << "new counter array " << (int)LRU_Counter[set][0]
	                             << (int)LRU_Counter[set][1]
	                             << (int)LRU_Counter[set][2]
	                             << (int)LRU_Counter[set][3]
	                             << (int)LRU_Counter[set][4]
	                             << (int)LRU_Counter[set][5]
	                             << (int)LRU_Counter[set][6]
	                             << (int)LRU_Counter[set][7]
	                             << endl;

	cout << "dirty bits------- " << (int)DirtyBit[set][0]
	                             << (int)DirtyBit[set][1]
	                             << (int)DirtyBit[set][2]
	                             << (int)DirtyBit[set][3]
	                             << (int)DirtyBit[set][4]
	                             << (int)DirtyBit[set][5]
	                             << (int)DirtyBit[set][6]
	                             << (int)DirtyBit[set][7]
	                             << endl;

	cout << "usage bits------- " << (int)CacheUsageBit[set][0]
	                             << (int)CacheUsageBit[set][1]
	                             << (int)CacheUsageBit[set][2]
	                             << (int)CacheUsageBit[set][3]
	                             << (int)CacheUsageBit[set][4]
	                             << (int)CacheUsageBit[set][5]
	                             << (int)CacheUsageBit[set][6]
	                             << (int)CacheUsageBit[set][7]
	                             << endl;
	cout << "Cached    ------- " << (int)Cached[set][0]
	                             << (int)Cached[set][1]
	                             << (int)Cached[set][2]
	                             << (int)Cached[set][3]
	                             << (int)Cached[set][4]
	                             << (int)Cached[set][5]
	                             << (int)Cached[set][6]
	                             << (int)Cached[set][7]
	                             << endl;


	cout << "stats hit miss " << total_hit << " " << total_miss << " " << total_evicted << endl; 
}





#endif