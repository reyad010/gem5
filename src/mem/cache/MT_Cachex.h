

#ifndef _MT_Cache_
#define _MT_Cache_


#include <math.h>
#include <stdint.h>
#include <iostream>




using namespace std;


extern const uint64_t MtMainMemorySize = 17179869184; //16GB 
const uint64_t FirstLevelNodes = MtMainMemorySize/(4096*8);


const double MT_Levels=((log(MtMainMemorySize/4096))/(log(8)))+1;

const uint64_t MT_numbers[14]={1,9,73,585,4681,37449,299593,2396745,19173961,153391689,1227133513,9817068105,78536544841,628292358729};
const uint64_t MT_first_level_nodes[14]={1,8,64,512,4096,32768,262144,2097152,16777216,134217728,1073741824,8589934592,68719476736,549755813888};
const uint64_t MT_bytes_per_level[10]={32768,262144,2097152,16777216,134217728,1073741824,8589934592,68719476736,549755813888,4398046511104};

//////////////////////////////////////////
const uint64_t MTCacheSize = 256*1024;

const uint32_t MTAssoc = 8;
const uint64_t MTBlkSiz = 64;
const uint64_t MTNoOfSets = (uint32_t)MTCacheSize/(MTBlkSiz*MTAssoc);
//const uint64_t MTNodesPerSet =MtMainMemorySize/MTNoOfSets;

//static uint64_t total_hit,total_miss,total_evicted;
        static uint32_t MTLRU_Counter[MTNoOfSets][MTAssoc];
        static uint32_t MTDirtyBit[MTNoOfSets][MTAssoc];
        static uint32_t MTCacheUsageBit[MTNoOfSets][MTAssoc];
        static uint64_t MTCached[MTNoOfSets][MTAssoc];
	static uint64_t MTCachedAddr[MTNoOfSets][MTAssoc];
	static uint32_t MTosirisCounter[MTNoOfSets][MTAssoc];
	static uint32_t mtwriteCounter[MTNoOfSets][MTAssoc];
	//static uint64_t osiris=0;
	static uint64_t MTnoOfWrites=0;
	static bool MTshadowPlusC[MTNoOfSets][MTAssoc];
	static uint64_t MTNoOfEviction=1;
	static uint64_t MTsumOfEviction=1;
	static uint64_t MTNodes,MTLevels;
	static uint64_t MTNodesPerSet;
        static uint64_t mtsumLife=1;
        static uint64_t mtreadOnly=0;
        static uint64_t mtmodified=0;
//static uint8_t MTLRU_Counter[MTNoOfSets][MTAssoc];
class MTCache
{

public:

	uint64_t total_hit,total_miss,total_evicted;
	//static uint8_t MTLRU_Counter[MTNoOfSets][MTAssoc];
	//uint8_t MTDirtyBit[MTNoOfSets][MTAssoc];
	//uint8_t MTCacheUsageBit[MTNoOfSets][MTAssoc];
	//uint64_t MTCached[MTNoOfSets][MTAssoc];


	MTCache(){

cout << "initializing cache: no of sets "<<MTNoOfSets << endl;
cout << "MT levels "<<MT_Levels<<endl;
cout << "fl nodes" << FirstLevelNodes << endl;



for(int i=0;i<14;i++)
{
cout << "trying i = " << i << endl;
if((FirstLevelNodes/MT_first_level_nodes[i])==1)
{
int no_of_nodes=MT_numbers[i];
int no_of_levels=i+1;
cout << " nodes " << no_of_nodes << " levels " << no_of_levels << endl;
MTNodes=no_of_nodes;
MTLevels=no_of_levels;
break;
}
else if((FirstLevelNodes/MT_first_level_nodes[i])<1){
int no_of_subtree=(FirstLevelNodes/MT_first_level_nodes[i-1]);
int no_of_nodes=no_of_subtree*MT_numbers[i-1]+1;
int no_of_levels=i+1;
cout << "subtree " << no_of_subtree << " nodes " << no_of_nodes << " levels " << no_of_levels << endl;
MTNodes=no_of_nodes;
MTLevels=no_of_levels;
break;
}

}


MTNodesPerSet=ceil((double)MTNodes/(double)(MTNoOfSets));

cout << "nodes per set " << MTNodesPerSet << endl;
total_hit=0,total_miss=0,total_evicted=0;

for(int set = 0; set < MTNoOfSets; set++){
		for(int i=0;i<MTAssoc;i++){
			MTLRU_Counter[set][i]=i;
			MTDirtyBit[set][i]=0;
			MTCacheUsageBit[set][i]=0;
			MTCached[set][i]=0xffffffffff;
			//for(int k=0;k<64;k++)
			MTosirisCounter[set][i]=0;
			MTshadowPlusC[set][i]=0;
			////cout << MTLRU_Counter[set][i]+30 ;

		}
//cout << " Set : "<<set;
		for(int i=0;i<MTAssoc;i++){
			//cout << MTLRU_Counter[set][i]+0 << " ";
}
//cout << endl;

}


}




	////

	uint64_t PushInCache(uint64_t address, char c,uint64_t level);
	void UpdateLRU(uint64_t set, uint64_t position);
	uint32_t LRU(uint64_t address,uint64_t level);
	uint32_t getPositionInSet(uint64_t address, uint64_t level);
	bool hasMTnode(uint64_t address,uint64_t level);
	bool isCacheHit(uint64_t address,uint64_t level);
	bool isCacheSetFull(uint64_t address,uint64_t level);
	uint32_t evictOne(uint64_t address);
	uint32_t modifiedEvictOne(uint64_t address,uint64_t level);
	void setMTDirtyBit(uint32_t set, uint32_t position);
	void resetMTDirtyBit(uint32_t set, uint32_t position);
	void setUsageBit(uint32_t set, uint32_t position);
	void resetUsageBit(uint32_t set, uint32_t position);
	void printLruCounter(uint32_t set);
	uint32_t getMTosirisCounter(uint64_t address, uint64_t level);
	void incrementMTosirisCounter(uint64_t address, uint64_t level);
	uint32_t mtgetWriteCounter(uint64_t address, uint64_t level);
	void mtincrementWriteCounter(uint64_t address, uint64_t level);
	void resetMTosirisCounter(uint64_t address, uint64_t level);
	uint32_t getShadowCounter(uint64_t address, uint64_t level);
        void setShadowCounter(uint64_t address, uint64_t level);
        void resetShadowCounter(uint64_t address, uint64_t level);
	uint16_t getMaxMTlevel();
	uint64_t getMTNodes();
	uint64_t getNodeNo(uint64_t address, uint64_t level);
	uint32_t mtgetAvgLife();
        uint64_t mtgetReadOnly();
        uint64_t mtgetModified();
	uint32_t getSet(uint64_t address, uint64_t level);

private:

protected:

};


uint64_t
MTCache::PushInCache(uint64_t address, char c, uint64_t level){


uint64_t NodeNo = getNodeNo(address,level);
//uint32_t set = getSet(address,level);
uint32_t set = getSet(address,level);
//cout << "set " << set <<" node no "<<NodeNo<<"node per set " << MTNodesPerSet << endl;

uint64_t mtID = (int)NodeNo;
//cout << "mtid " << mtID << endl;

//mtID = mtID|(level<<32);
//uint64_t id = mtID;
//cout << "level << 32 = " << (level<<32) << endl;
//cout << "final mtid " << mtID << endl;

//cout <<"\n\nadr  "<<address<<"------putting counter in the cache pageID:"<< pageID << " set " << set << " pos " << position <<endl;

//this->printLruCounter(set);
//cout << "--" << endl;
uint64_t evicted=0xffffffffff;

//modifiedEvictOne(address);

if(this->isCacheHit(address,level)){


	mtmodified++;
	//cout << "dirty update " << endl;
	MTnoOfWrites++;
	//MTosirisCounter[set][this->getPositionInSet(address)]++;
	//if(MTosirisCounter[set][this->getPositionInSet(address)]>=4) 
	//{
	//cout << "osiris" << "<<" << MTnoOfWrites << "|" << osiris << ">>" <<endl;
	//osiris++;
	//MTosirisCounter[set][this->getPositionInSet(address)]=0;
	
	//}
//        cout << "osiris counter        " << (int)MTosirisCounter[set][this->getPositionInSet(address)]<<endl;
//	cout << "              S_C                           " << (int)MTshadowPlusC[set][this->getPositionInSet(address)]<<endl;

    	MTCached[set][this->getPositionInSet(address,level)]=mtID;
	MTCachedAddr[set][this->getPositionInSet(address,level)]=address;

    	if(c=='w'){
    		////cout << "**************************incrementing counter <<<<<<<<<" << endl;
    		this->setMTDirtyBit(set,this->getPositionInSet(address,level));
    	}
    	this->setUsageBit(set,this->getPositionInSet(address,level));
		this->UpdateLRU(set, this->getPositionInSet(address,level));
		//this->printLruCounter(set);

}
else if(this->isCacheSetFull(address,level)){
	    //cout << " cache set full " << endl;
		//uint32_t evict_pos=this->evictOne(address);
		 //MTNoOfEviction++;
		uint32_t evict_pos=modifiedEvictOne(address,level);
		if(evict_pos<MTAssoc){
                if(mtwriteCounter[set][evict_pos]==0) mtreadOnly++;
		else MTNoOfEviction++;
                mtsumLife+=mtwriteCounter[set][evict_pos];

		//MTNoOfEviction++;
		MTsumOfEviction+=MTLRU_Counter[set][evict_pos];
		//cout << "ecicting from lru pos     "<<MTLRU_Counter[set][evict_pos]<<"||    avg " <<(double)(MTsumOfEviction/MTNoOfEviction)<<endl; 
		evicted=MTCachedAddr[set][evict_pos];
		MTCached[set][evict_pos]=mtID;
		MTCachedAddr[set][evict_pos]=address;
		//for(int i=0;i<64;i++)
			MTosirisCounter[set][evict_pos]=0;
			MTshadowPlusC[set][evict_pos]=0;
			mtwriteCounter[set][evict_pos]=0;
			//cout<<"clearing shadow plus" << MTshadowPlusC[set][evict_pos] << endl;
		this->resetMTDirtyBit(set,evict_pos);
    	//this->resetUsageBit(set,evict_pos,offset);
    	this->UpdateLRU(set,evict_pos);
    	//this->removeTag(set,evict_position,offset);
		}
		else{
			//cout << "evicting from clean pos" <<(int)(evict_pos-MTAssoc)<< endl;
			if(mtwriteCounter[set][evict_pos-MTAssoc]==0) mtreadOnly++;
			else MTNoOfEviction++;
			MTCached[set][evict_pos-MTAssoc]=mtID;
			MTCachedAddr[set][evict_pos-MTAssoc]=address;
			//for(int i=0;i<64;i++)           
                        	MTosirisCounter[set][evict_pos-MTAssoc]=0;
				MTshadowPlusC[set][evict_pos-MTAssoc]=0;
				mtwriteCounter[set][evict_pos-MTAssoc]=0;
				//cout<<"clearing shadow plus" << MTshadowPlusC[set][this->LRU(address)] << endl;
			this->resetMTDirtyBit(set,evict_pos-MTAssoc);
			this->UpdateLRU(set,evict_pos-MTAssoc);
		}

		//this->printLruCounter(set);


    }
else{

	//cout << "new update " << mtID <<endl;

	MTCached[set][this->LRU(address,level)]=mtID;
	MTCachedAddr[set][this->LRU(address,level)]=address;
	//MTosirisCounter[set][this->LRU(address)]=0;
	//for(int i=0;i<64;i++)
                        MTosirisCounter[set][this->LRU(address,level)]=0;
			MTshadowPlusC[set][this->LRU(address,level)]=0;
			mtwriteCounter[set][this->LRU(address,level)]=0;
			//cout<<"clearing shadow plus" << MTshadowPlusC[set][LRU(address)] << endl;
	if(c=='w'){
		this->resetMTDirtyBit(set,LRU(address,level));
	}
    
    this->setUsageBit(set,this->LRU(address,level));
    //this->printLruCounter(set);
	this->UpdateLRU(set, this->LRU(address,level));
	//this->printLruCounter(set);

}

////cout << "evicting " << evicted << endl;
if(evicted!=0xffffffff){
//cout << "evicting --" << evicted <<endl;
}
else{
////cout << "no eviction" << endl;
}

//cout << "pageid " << pageID << endl;
//this->printLruCounter(set);
//cout << "evicted= " << evicted << endl;
return evicted;


}


uint32_t 
MTCache::getSet(uint64_t address, uint64_t level)
{
uint32_t set = 0;

if(level==1)
{
set = (uint64_t)((double)address/(double)35791394);
if (set==480) set = 479;
}
else if(level==2)
{
set = (uint64_t)((double)address/(double)660764199);
if (set==26) set = 25;
set+=480;
}
else if(level==3)
{
set = (uint64_t)((double)address/(double)8589934592);
if (set==2) set = 1;
set+=506;
}
else if(level==4)
{
set = 508;
}
else if(level==5)
{
set = 509;
}
else if(level==6)
{
set = 510;
}
else if(level==7)
{
set = 511;
}

return set;

}


uint64_t 
MTCache::getNodeNo(uint64_t address, uint64_t level)
{

if(level==1)
{


return (address/MT_bytes_per_level[level-1]);


}
else if(level==2)
{

//cout << FirstLevelNodes << " " << (address/MT_bytes_per_level[level-1]) << endl;
return ((address/MT_bytes_per_level[level-1])+FirstLevelNodes);
}
else if(level==3)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144);
}
else if(level==4)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144)+MtMainMemorySize/(2097152);
}
else if(level==5)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144)+MtMainMemorySize/(2097152)+MtMainMemorySize/(16777216);
}
else if(level==6)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144)+MtMainMemorySize/(2097152)+MtMainMemorySize/(16777216)+MtMainMemorySize/(134217728);
}
else if(level==7)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144)+MtMainMemorySize/(2097152)+MtMainMemorySize/(16777216)+MtMainMemorySize/(134217728)+MtMainMemorySize/(1073741824);
}
else if(level==8)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144)+MtMainMemorySize/(2097152)+MtMainMemorySize/(16777216)+MtMainMemorySize/(134217728)+MtMainMemorySize/(1073741824)+MtMainMemorySize/(8589934592);
}
else if(level==9)
{
return (address/MT_bytes_per_level[level-1])+FirstLevelNodes+MtMainMemorySize/(262144)+MtMainMemorySize/(2097152)+MtMainMemorySize/(16777216)+MtMainMemorySize/(134217728)+MtMainMemorySize/(1073741824)+MtMainMemorySize/(8589934592)+MtMainMemorySize/(68719476736);
}
else
return 0;

}


void
MTCache::mtincrementWriteCounter(uint64_t address, uint64_t level)
{

//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);
mtwriteCounter[set][this->getPositionInSet(address,level)]++;

}

uint32_t
MTCache::mtgetWriteCounter(uint64_t address, uint64_t level)
{

//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);
return mtwriteCounter[set][this->getPositionInSet(address,level)];
}


uint32_t 
MTCache::mtgetAvgLife()
{
return (int)((double)mtsumLife/(double)MTNoOfEviction);
}

uint64_t 
MTCache::mtgetReadOnly()
{
return mtreadOnly;
}

uint64_t 
MTCache::mtgetModified()
{
return MTNoOfEviction;
}



uint16_t
MTCache::getMaxMTlevel()
{
return MTLevels;
}

uint64_t
MTCache::getMTNodes()
{
return MTNodes;
}

uint32_t
MTCache::getShadowCounter(uint64_t address,uint64_t level)
{

//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

return MTshadowPlusC[set][this->getPositionInSet(address,level)];

}

uint32_t 
MTCache::getMTosirisCounter(uint64_t address, uint64_t level)
{
//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

return MTosirisCounter[set][this->getPositionInSet(address,level)];

}

void
MTCache::setShadowCounter(uint64_t address, uint64_t level)
{
//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

MTshadowPlusC[set][this->getPositionInSet(address,level)]=1;

}

void
MTCache::incrementMTosirisCounter(uint64_t address, uint64_t level)
{
//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

MTosirisCounter[set][this->getPositionInSet(address,level)]++;

}


void
MTCache::resetShadowCounter(uint64_t address,uint64_t level)
{
//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

MTshadowPlusC[set][this->getPositionInSet(address,level)]=0;
}

void
MTCache::resetMTosirisCounter(uint64_t address,uint64_t level)
{
//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

MTosirisCounter[set][this->getPositionInSet(address,level)]=0;
}

uint32_t
MTCache::getPositionInSet(uint64_t address, uint64_t level)
{

uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

uint64_t mtID = (int)NodeNo;
//mtID = mtID|(level<<32);

for(int k = 0; k<MTAssoc; k++){
	if(MTCached[set][k]==mtID) {

		return k;
}


}


return 0;

}

uint32_t
MTCache::LRU(uint64_t address, uint64_t level){

	//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

	//cout << "getting LRU in set " << set << endl;

//cout<<endl;
for (int i=0;i<MTAssoc;i++)
{
//cout << (int)MTLRU_Counter[set][i];
}
//cout << endl;


	for(int i=0;i<MTAssoc;i++){
		if(MTLRU_Counter[set][i]==0) {
			//cout << "returning " << i << endl;
			return i;
		}

	}
	
	return 0;
}

void
MTCache::UpdateLRU(uint64_t set, uint64_t position){

	//cout << "LRU set "<<set<<" position " << position<< "in " << MTNoOfSets << endl; 
 
int sum =0;
for(int i=0;i<MTAssoc;i++)
{
sum+=MTLRU_Counter[set][i];
//cout <<"sum" <<  sum << endl;
}
//if(sum!=28) //cout<<"error in LRU"<< sum<<endl;
//else //cout << "LRU OK" << endl;
//for(int i=0;i<MTAssoc;i++)
//{
//cout << MTLRU_Counter[set][i]+0 ;
//}
//cout << endl;

	int prev_position = MTLRU_Counter[set][position];
	MTLRU_Counter[set][position]=MTAssoc-1;
	for(int i=0; i<MTAssoc; i++){
		if(i==position || MTLRU_Counter[set][i]==0) continue;
		if(MTLRU_Counter[set][i]>prev_position) MTLRU_Counter[set][i]--;

	}

	////cout << "LRU set "<<set<<" "; 

	for(int i=0;i<MTAssoc;i++){
		//cout<< (int)MTLRU_Counter[set][i] << " ";
	}

	////cout << endl;
}



bool
MTCache::hasMTnode(uint64_t address, uint64_t level)
{ 

	//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

//cout << "looking for counter : set" <<  set << " addr:"<<address<<"bytes pers "<<MTNodesPerSet<< endl;
if(this->isCacheHit(address,level)){
	//cout << " has counter in cache"<< endl;
	total_hit++;
	UpdateLRU(set, getPositionInSet(address,level));
	return true;
} 
else{
	//cout << " no counter in cache"<< endl;
	total_miss++;
	return false;
} 
}


bool
MTCache::isCacheHit(uint64_t address, uint64_t level)
{
	////cout << "cache hit " << endl;
uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

uint64_t mtID = (unsigned long)NodeNo;
//mtID = mtID|(level<<32);

//cout << "checking cache hit "  << set << " " << mtID << endl;

for(int i = 0; i<MTAssoc; i++){
	//cout << " checking " << (int)MTCached[set][i] << " with " << pageID << endl;
	if(MTCached[set][i]==mtID) {
        //cout << " hit and returning true"<< endl;
		//updateLruCounter(set,i,offset);
		return true;
}
}
////cout << "returning false"<< endl;
return false;
}


bool 
MTCache::isCacheSetFull(uint64_t address, uint64_t level){
	//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

	for(int i = 0; i<MTAssoc; i++){
		if(MTCacheUsageBit[set][i]==0){
			////cout << "cache set is not full" << endl;
			return false;
		}
	}
	////cout << "cache FULL" << endl;
	return true;
}


uint32_t
MTCache::modifiedEvictOne(uint64_t address,uint64_t level){
//uint64_t NodeNo = getNodeNo(address,level);
uint32_t set = getSet(address,level);

        uint32_t evict_position=0;
        uint32_t to_evict=0;
        //uint32_t previous_position=MTAssoc+1;
        bool atLeastOneDirty = false;

        for(int i=0; i<MTAssoc; i++){
                if(MTDirtyBit[set][i]==1) atLeastOneDirty=true;
        }

	if(atLeastOneDirty){
		
	
	}

	evict_position=MTLRU_Counter[set][0];
	for(int i=0;i<MTAssoc;i++){
		if(MTLRU_Counter[set][i]<evict_position){
			evict_position=MTLRU_Counter[set][i];
			to_evict=i;
		}
	}

	if(MTDirtyBit[set][to_evict]){
		//to_evict+=MTAssoc;
	}
	else
	{
		to_evict+=MTAssoc;
	}

	//cout << "evicting " << to_evict << endl;
	return to_evict;
}

uint32_t 
MTCache::evictOne(uint64_t address){
	uint32_t set = (int)(address / MTNodesPerSet);
	uint32_t evict_position=0;
	uint32_t to_evict=0;
	uint32_t previous_position=MTAssoc+1;
	bool atLeastOneDirty = false;

	for(int i=0; i<MTAssoc; i++){
		if(MTDirtyBit[set][i]==1) atLeastOneDirty=true;
	}


	if(atLeastOneDirty){
		for(int i=0; i< MTAssoc; i++ ){
		if(MTDirtyBit[set][i]==0) {
			////cout << "abandoning " << i << endl; 
			continue;
		}
		else{
			if((int)MTLRU_Counter[set][i]<previous_position){
				evict_position = (int)MTLRU_Counter[set][i];
				to_evict = i;
				////cout << "lru count " << i << " is " << (int)MTLRU_Counter[set][i] << " evict position " << evict_position  <<" to evict " << i << endl;
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
MTCache::setMTDirtyBit(uint32_t set, uint32_t position){
	MTDirtyBit[set][position]=1;

}

void 
MTCache::resetMTDirtyBit(uint32_t set, uint32_t position){
	MTDirtyBit[set][position]=0;

}

void 
MTCache::setUsageBit(uint32_t set, uint32_t position){
	MTCacheUsageBit[set][position]=1;

}

void 
MTCache::resetUsageBit(uint32_t set, uint32_t position){
	MTCacheUsageBit[set][position]=0;

}

void MTCache::printLruCounter(uint32_t set)
{
	cout << "new counter array ";
	for(int i=0;i<MTAssoc;i++)
		cout <<" "<< (int)MTLRU_Counter[set][i];
	cout << endl;

	cout << "dirty bits------- ";
	for(int i=0;i<MTAssoc;i++)
		cout<< " "<<(int)MTDirtyBit[set][i];
	cout<<endl;

	cout << "usage bits------- ";
	for(int i=0;i<MTAssoc;i++)
		cout<<" "<<(int)MTCacheUsageBit[set][i];
	cout<<endl;
/*
        cout << "cached mtid ----- ";
        for(int i=0;i<MTAssoc;i++)
                cout<<" "<<(unsigned long)MTCached[set][i];
        cout<<endl;


        cout << "cached addr ----- ";
        for(int i=0;i<MTAssoc;i++)
                cout<<" "<<(unsigned long)MTCachedAddr[set][i];
        cout<<endl;

*/
        cout << "Shadow P    ----- ";
        for(int i=0;i<MTAssoc;i++)
                cout<<" "<<(unsigned long)MTshadowPlusC[set][i];
       
 cout<<endl;
 cout << endl;

	//cout << "stats hit miss " << total_hit << " " << total_miss << " " << total_evicted << endl; 
}





#endif
