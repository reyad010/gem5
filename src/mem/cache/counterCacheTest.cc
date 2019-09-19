#include <iostream>
#include <stdlib.h>
//#include "C_Cache.h"
#include "sgx_Ccache.h"
#include <stdio.h>

using namespace std;

int main()
{
        CounterCache *c = new CounterCache();

        uint64_t addr = 0;
        for (int i=0;i<10000000;i++){
                scanf("%lx",&addr);
                printf("\nreturned---%lx\n",c->PushInCache(addr,'w'));
		//c->hasCounter(addr);
                //c->UpdateLRU(addr);
        }

}

