#ifndef __CC__
#define __CC__


#include <math.h>
 #include <stdint.h>
#include <iostream>



using namespace std;



struct minor_7bit{
	char counter :7;
};

class CacheBlock
{
public:
	uint64_t major_counter;
	minor_7bit minor_counter[64];

	bool setMajor(uint64_t maor);
	bool setMinor(uint8_t value, uint8_t counter_no);
	uint64_t getMajor();
	uint8_t getMinor(uint8_t counter_no);

};


uint64_t
CacheBlock::getMajor(){
	return this->major_counter;
}

uint8_t 
CacheBlock::getMinor(uint8_t counter_no){
	if(counter_no>=64) return false;
	else{
		return (uint8_t)this->minor_counter[counter_no].counter;
	}
}

bool
CacheBlock::setMinor(uint8_t value, uint8_t counter_no){
	if(value>=128 || counter_no>=64) return false;
	else{
		this->minor_counter[counter_no].counter=value;
		return true;
	}
	
}

bool
CacheBlock::setMajor(uint64_t major){
	this->major_counter=major;
	return true;
}


#endif