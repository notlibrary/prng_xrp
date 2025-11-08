/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "prng64_xrp32.h"
#include "prng64_xrp32.c"


int 
main()
{
	size_t i;
	uint64_t seeda[100];
	uint64_t seedb[100];

	seed_xrp32(65536);
	printf("%s \n","prng64_xrp32() avalanche effect test sample");
	for (i=0;i<100;i++) 
	{
		seeda[i]=prng64_xrp32();
	}

	seed_xrp32(65537);		
	for (i=0;i<100;i++) 
	{
		seedb[i]=prng64_xrp32();
	}	
	for (i=0;i<100;i++) 
	{
		printf("%I64u %I64u \n", seeda[i],seedb[i]);
	}	
	system("pause");
	return 0;
}


