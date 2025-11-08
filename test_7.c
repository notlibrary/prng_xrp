/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include "prng32_xrp64.h"
#include "prng32_xrp64.c"


int 
main()
{
	size_t i;
	uint32_t seeda[100];
	uint32_t seedb[100];

	for (i=0;i<100;i++) 
	{
		seeda[i]=0;
		seedb[i]=0;
		
	}

	seed_xrp64(65535);
	printf("%s \n","prng32_xrp64() same seed test sample");
	for (i=0;i<100;i++) 
	{
		seeda[i]=prng32_xrp64();
	}

	seed_xrp64(65000);
	for (i=0;i<50;i++) 
	{
		rng32_xrp64();
	}
	
	seed_xrp64(65535);		
	for (i=0;i<100;i++) 
	{
		seedb[i]=prng32_xrp64();
	}	
	for (i=0;i<100;i++) 
	{
		printf("%lu %lu \n", seeda[i],seedb[i]);
	}
	system("pause");
	return 0;
}