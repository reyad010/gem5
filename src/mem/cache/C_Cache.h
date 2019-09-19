

#ifndef _C_Cache_
#define _C_Cache_


#include <math.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include "CC.h"



using namespace std;


const uint64_t MainMemorySize = 17179869184; //16GB 

const uint64_t CacheSize = 256*1024;
const uint32_t Associativity = 16;
const uint64_t BlockSize = 64;
const uint64_t NoOfSets = (uint32_t)CacheSize/(BlockSize*Associativity);
const uint64_t BytesPerSet = MainMemorySize/(NoOfSets);

//static uint64_t total_hit,total_miss,total_evicted;
static uint32_t LRU_Counter[NoOfSets][Associativity];
static uint32_t DirtyBit[NoOfSets][Associativity];
static uint32_t CacheUsageBit[NoOfSets][Associativity];
static uint64_t Cached[NoOfSets][Associativity];
static uint64_t CachedAddr[NoOfSets][Associativity];
static uint32_t osirisCounter[NoOfSets][Associativity][64];
static uint32_t writeCounter[NoOfSets][Associativity];
//static uint64_t osiris=0;
static uint64_t noOfWrites=0;
static bool shadowPlusC[NoOfSets][Associativity];
static uint64_t noOfEviction=1;
static uint64_t sumOfEviction=1;
static uint64_t sumLife=1;
static uint64_t readOnly=0;
static uint64_t modified=0;
//static uint64_t avgLife=0;
//static uint8_t LRU_Counter[NoOfSets][Associativity];
std::map<uint64_t, uint8_t> ENCRYPT_COUNTERS;

class CounterCache 
{

public:

	uint64_t total_hit,total_miss,total_evicted;
	//static uint8_t LRU_Counter[NoOfSets][Associativity];
	//uint8_t DirtyBit[NoOfSets][Associativity];
	//uint8_t CacheUsageBit[NoOfSets][Associativity];
	//uint64_t Cached[NoOfSets][Associativity];


	CounterCache(){

//cout << "initializing cache: no of sets "<<NoOfSets << endl;
total_hit=0,total_miss=0,total_evicted=0;

cout << "no of set " << NoOfSets << endl;

for(int set = 0; set < NoOfSets; set++){
		for(int i=0;i<Associativity;i++){
			LRU_Counter[set][i]=i;
			DirtyBit[set][i]=0;
			CacheUsageBit[set][i]=0;
			Cached[set][i]=0xffffffffff;
			for(int k=0;k<64;k++)
			osirisCounter[set][i][k]=0;
			writeCounter[set][i]=0;
			shadowPlusC[set][i]=0;
			
			////cout << LRU_Counter[set][i]+30 ;

		}
//cout << " Set : "<<set;
		for(int i=0;i<Associativity;i++){
			//cout << LRU_Counter[set][i]+0 << " ";
}
//cout << endl;

}


}




	////

	uint64_t PushInCache(uint64_t address, char c);
	void UpdateLRU(uint64_t set, uint64_t position);
	uint32_t LRU(uint64_t address);
	uint32_t getPositionInSet(uint64_t address);
	bool hasCounter(uint64_t address);
	bool isCacheHit(uint64_t address);
	bool isCacheSetFull(uint64_t address);
	uint32_t evictOne(uint64_t address);
	uint32_t modifiedEvictOne(uint64_t address);
	void setDirtyBit(uint32_t set, uint32_t position);
	void resetDirtyBit(uint32_t set, uint32_t position);
	void setUsageBit(uint32_t set, uint32_t position);
	void resetUsageBit(uint32_t set, uint32_t position);
	void printLruCounter(uint32_t set);
	uint32_t getOsirisCounter(uint64_t address);
	uint32_t getWriteCounter(uint64_t address);
	void incrementOsirisCounter(uint64_t address);
	void incrementWriteCounter(uint64_t address);
	void resetOsirisCounter(uint64_t address);
	uint32_t getShadowCounter(uint64_t address);
        void setShadowCounter(uint64_t address);
        void resetShadowCounter(uint64_t address);
	uint32_t getAvgLife();
	uint64_t getReadOnly();
	uint64_t getModified();

	uint64_t getWriteCounterData(uint64_t address);
	uint64_t updateWriteCounterData(uint64_t address, uint64_t count);
	bool hasData(uint64_t address);
	uint8_t getData(uint64_t address);
	void setData(uint64_t address, uint8_t data);

private:

protected:

};

bool
CounterCache::hasData(uint64_t cacheLine){
	if(ENCRYPT_COUNTERS.find(cacheLine) != ENCRYPT_COUNTERS.end())
		return true;

	return false;
}

uint8_t
CounterCache::getData(uint64_t cacheLine){
	return ENCRYPT_COUNTERS[cacheLine];
}
void
CounterCache::setData(uint64_t cacheLine, uint8_t data){
	if(ENCRYPT_COUNTERS.find(cacheLine) == ENCRYPT_COUNTERS.end())
		ENCRYPT_COUNTERS[cacheLine] = 0;

	ENCRYPT_COUNTERS[cacheLine] = data;
}

uint64_t
CounterCache::PushInCache(uint64_t address, char c){

	uint32_t set = (int)(address / BytesPerSet);
	uint64_t position = LRU(address);
	position = position;
	uint64_t pageID = (int)((address)/(64*64));


	//cout <<"\n\nadr  "<<address<<"------putting counter in the cache pageID:"<< pageID << " set " << set << " pos " << position <<endl;

	//this->printLruCounter(set);
	//cout << "--" << endl;
	uint64_t evicted=0xffffffffff;

	//modifiedEvictOne(address);

	if(this->isCacheHit(address)){

		modified++;

		//cout << "dirty update " << endl;
		noOfWrites++;
		//osirisCounter[set][this->getPositionInSet(address)]++;
		//if(osirisCounter[set][this->getPositionInSet(address)]>=4)
		//{
		//cout << "osiris" << "<<" << noOfWrites << "|" << osiris << ">>" <<endl;
		//osiris++;
		//osirisCounter[set][this->getPositionInSet(address)]=0;
		//}
	//        cout << "osiris counter        " << (int)osirisCounter[set][this->getPositionInSet(address)]<<endl;
	//	cout << "              S_C                           " << (int)shadowPlusC[set][this->getPositionInSet(address)]<<endl;

	//                for(int k=0;k<64;k++)
	//                        cout << osirisCounter[set][this->getPositionInSet(address)][k]<<"-";
	//			cout << endl;


			Cached[set][this->getPositionInSet(address)]=pageID;
		CachedAddr[set][this->getPositionInSet(address)]=address;

			if(c=='w'){
				////cout << "**************************incrementing counter <<<<<<<<<" << endl;
				this->setDirtyBit(set,this->getPositionInSet(address));
			}
			this->setUsageBit(set,this->getPositionInSet(address));
			this->UpdateLRU(set, this->getPositionInSet(address));
			//this->printLruCounter(set);

	}
	else if(this->isCacheSetFull(address)){
			//cout << " cache set full " << endl;
			//uint32_t evict_pos=this->evictOne(address);
			uint32_t evict_pos=modifiedEvictOne(address);
			//noOfEviction++;

			if(evict_pos<Associativity){
			if(writeCounter[set][evict_pos]==0) readOnly++;
			else noOfEviction++;
			sumLife+=writeCounter[set][evict_pos];
			//cout << (int)sumLife/noOfEviction << endl;
			//noOfEviction++;
			sumOfEviction+=LRU_Counter[set][evict_pos];
			//cout << "ecicting from lru pos     "<<LRU_Counter[set][evict_pos]<<"||    avg " <<(double)(sumOfEviction/noOfEviction)<<endl;
			evicted=CachedAddr[set][evict_pos];
			Cached[set][evict_pos]=pageID;
			CachedAddr[set][evict_pos]=address;
				for(int k=0;k<64;k++)
				osirisCounter[set][evict_pos][k]=0;
				shadowPlusC[set][evict_pos]=0;
				writeCounter[set][evict_pos]=0;
				//cout<<"clearing shadow plus" << shadowPlusC[set][evict_pos] << endl;
			this->resetDirtyBit(set,evict_pos);
			//this->resetUsageBit(set,evict_pos,offset);
			this->UpdateLRU(set,evict_pos);
			//this->removeTag(set,evict_position,offset);
			}
			else{
				if(writeCounter[set][evict_pos-Associativity]==0) readOnly++;
				else noOfEviction++;
				sumLife+=writeCounter[set][evict_pos-Associativity];
						//cout << (int)sumLife/noOfEviction << endl;

				//cout << "evicting from clean pos" <<(int)(evict_pos-Associativity)<< endl;
				Cached[set][evict_pos-Associativity]=pageID;
				CachedAddr[set][evict_pos-Associativity]=address;
				for(int k=0;k<64;k++)
								osirisCounter[set][evict_pos-Associativity][k]=0;
					shadowPlusC[set][evict_pos-Associativity]=0;
					writeCounter[set][evict_pos-Associativity]=0;
					//cout<<"clearing shadow plus" << shadowPlusC[set][this->LRU(address)] << endl;
				this->resetDirtyBit(set,evict_pos-Associativity);
				this->UpdateLRU(set,evict_pos-Associativity);
			}

			//this->printLruCounter(set);


		}
	else{

		//cout << "new update " << endl;

		Cached[set][this->LRU(address)]=pageID;
		CachedAddr[set][this->LRU(address)]=address;
		//osirisCounter[set][this->LRU(address)]=0;
		for(int k=0;k<64;k++)
							osirisCounter[set][this->LRU(address)][k]=0;
				shadowPlusC[set][this->LRU(address)]=0;
				writeCounter[set][this->LRU(address)]=0;
				//cout<<"clearing shadow plus" << shadowPlusC[set][LRU(address)] << endl;
		if(c=='w'){
			this->resetDirtyBit(set,LRU(address));
		}

		this->setUsageBit(set,this->LRU(address));
		//this->printLruCounter(set);
		this->UpdateLRU(set, this->LRU(address));
		//this->printLruCounter(set);

	}

	////cout << "evicting " << evicted << endl;
	if(evicted!=0xffffffff){
	////cout << "evicting" << endl;
	}
	else{
	////cout << "no eviction" << endl;
	}

	//cout << "pageid " << pageID << endl;
	//this->printLruCounter(set);
	return evicted;


}

uint32_t 
CounterCache::getShadowCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
return shadowPlusC[set][this->getPositionInSet(address)];

}

uint32_t 
CounterCache::getOsirisCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
return osirisCounter[set][this->getPositionInSet(address)][(address/64)%64];

}

uint32_t
CounterCache::getWriteCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
return writeCounter[set][this->getPositionInSet(address)];
}

void
CounterCache::setShadowCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
shadowPlusC[set][this->getPositionInSet(address)]=1;

}

uint32_t 
CounterCache::getAvgLife()
{
return (int)((double)sumLife/(double)noOfEviction);
}

uint64_t 
CounterCache::getReadOnly()
{
return readOnly;
}

uint64_t 
CounterCache::getModified()
{
return noOfEviction;
}


void
CounterCache::incrementOsirisCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
osirisCounter[set][this->getPositionInSet(address)][(address/64)%64]++;

}

void
CounterCache::incrementWriteCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
writeCounter[set][this->getPositionInSet(address)]++;

}


void
CounterCache::resetShadowCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
shadowPlusC[set][this->getPositionInSet(address)]=0;
}

void
CounterCache::resetOsirisCounter(uint64_t address)
{
uint32_t set = (int)(address / BytesPerSet);
osirisCounter[set][this->getPositionInSet(address)][(address/64)%64]=0;
}

uint32_t
CounterCache::getPositionInSet(uint64_t address)
{

uint32_t set = (int)(address / BytesPerSet);
uint64_t pageID = (int)((address)/(64*64));

for(int k = 0; k<Associativity; k++){
	if(Cached[set][k]==pageID) {

		return k;
}


}


return 0;

}

/*uint64_t
CounterCache::getWriteCounterData(uint64_t address) {
	if(ENCRYPT_COUNTERS.find(address/PageSize) == ENCRYPT_COUNTERS.end())
	{
		ENCRYPT_COUNTERS[address/PageSize] = 0;
	}
	return ENCRYPT_COUNTERS[address/PageSize];
}

void
CounterCache::updateWriteCounterData(uint64_t address, uint64_t count) {
	if(ENCRYPT_COUNTERS.find(address/PageSize) == ENCRYPT_COUNTERS.end())
	{
		ENCRYPT_COUNTERS[address/PageSize] = 0;
	}
	ENCRYPT_COUNTERS[address/PageSize] = count;
}*/

uint32_t
CounterCache::LRU(uint64_t address){

	uint64_t set = (int)(address / BytesPerSet);
	//cout << "getting LRU in set " << set << endl;

//cout<<endl;
for (int i=0;i<Associativity;i++)
{
//cout << (int)LRU_Counter[set][i];
}
//cout << endl;


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

	//cout << "LRU set "<<set<<" position " << position<< "in " << NoOfSets << endl; 
 
int sum =0;
for(int i=0;i<Associativity;i++)
{
sum+=LRU_Counter[set][i];
//cout <<"sum" <<  sum << endl;
}
//if(sum!=28) //cout<<"error in LRU"<< sum<<endl;
//else //cout << "LRU OK" << endl;
//for(int i=0;i<Associativity;i++)
//{
//cout << LRU_Counter[set][i]+0 ;
//}
//cout << endl;

	int prev_position = LRU_Counter[set][position];
	LRU_Counter[set][position]=Associativity-1;
	for(int i=0; i<Associativity; i++){
		if(i==position || LRU_Counter[set][i]==0) continue;
		if(LRU_Counter[set][i]>prev_position) LRU_Counter[set][i]--;

	}

	////cout << "LRU set "<<set<<" "; 

	for(int i=0;i<Associativity;i++){
		//cout<< (int)LRU_Counter[set][i] << " ";
	}

	////cout << endl;
}



bool
CounterCache::hasCounter(uint64_t address)
{ 

	uint32_t set = (int)(address / BytesPerSet);
//cout << "looking for counter : set" <<  set << " addr:"<<address<<"bytes pers "<<BytesPerSet<< endl;
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
	////cout << "cache hit " << endl;
uint32_t set = (int)(address / BytesPerSet);

uint64_t pageID = (int)((address)/(64*64));

//cout << "checking cache hit "  << set << " " << pageID << endl;

for(int i = 0; i<Associativity; i++){
	//cout << " checking " << (int)Cached[set][i] << " with " << pageID << endl;
	if((int)Cached[set][i]==(int)pageID) {
        //cout << " hit and returning true"<< endl;
		//updateLruCounter(set,i,offset);
		return true;
}
}
////cout << "returning false"<< endl;
return false;
}


bool 
CounterCache::isCacheSetFull(uint64_t address){
	uint32_t set = (int)(address / BytesPerSet);
	for(int i = 0; i<Associativity; i++){
		if(CacheUsageBit[set][i]==0){
			////cout << "cache set is not full" << endl;
			return false;
		}
	}
	////cout << "cache FULL" << endl;
	return true;
}


uint32_t
CounterCache::modifiedEvictOne(uint64_t address){
uint32_t set = (int)(address / BytesPerSet);
        uint32_t evict_position=0;
        uint32_t to_evict=0;
        //uint32_t previous_position=Associativity+1;
        bool atLeastOneDirty = false;

        for(int i=0; i<Associativity; i++){
                if(DirtyBit[set][i]==1) atLeastOneDirty=true;
        }

	if(atLeastOneDirty){
		
	
	}

	evict_position=LRU_Counter[set][0];
	for(int i=0;i<Associativity;i++){
		if(LRU_Counter[set][i]<evict_position){
			evict_position=LRU_Counter[set][i];
			to_evict=i;
		}
	}

	if(DirtyBit[set][to_evict]){
		//to_evict+=Associativity;
	}
	else
	{
		to_evict+=Associativity;
	}

	//cout << "evicting " << to_evict << endl;
	return to_evict;
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
			////cout << "abandoning " << i << endl; 
			continue;
		}
		else{
			if((int)LRU_Counter[set][i]<previous_position){
				evict_position = (int)LRU_Counter[set][i];
				to_evict = i;
				////cout << "lru count " << i << " is " << (int)LRU_Counter[set][i] << " evict position " << evict_position  <<" to evict " << i << endl;
			}
			previous_position=evict_position;
		}
	}
	}
	else{
		////cout << "not evicting" << endl;
		to_evict = 100;
	}
	

    ////cout << "evicting " << evict_position << " of set " << set << endl; 

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
	cout << "new counter array ";
	for(int i=0;i<Associativity;i++)
		cout <<" "<< (int)LRU_Counter[set][i];
	cout << endl;

	cout << "dirty bits------- ";
	for(int i=0;i<Associativity;i++)
		cout<< " "<<(int)DirtyBit[set][i];
	cout<<endl;

	cout << "usage bits------- ";
	for(int i=0;i<Associativity;i++)
		cout<<" "<<(int)CacheUsageBit[set][i];
	cout<<endl;

 


	//cout << "stats hit miss " << total_hit << " " << total_miss << " " << total_evicted << endl; 
}





#endif

