#include <iostream>
#include <stdlib.h>
//#include "sgx_Mtcache.h"
#include "MT_Cache.h"
#include <stdio.h>

using namespace std;

int main()
{
        MTCache *c = new MTCache();

        uint64_t addr = 0;
	uint64_t level=0;
        for (int i=0;i<10000000;i++){
                scanf("%ld %ld",&addr,&level);
                printf("\nreturned---%ld\n",c->PushInCache(addr,'w',level));
		//c->hasCounter(addr);
                //c->UpdateLRU(addr);
        }

}

