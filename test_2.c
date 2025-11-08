/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#include <stdlib.h>

#include <stdio.h>

#include "prng32_xrp64.h"
#include "prng32_xrp64.c"


int 
main()
{

size_t i;
seed_xrp64(0x0A0B0C0D);
printf("%s \n","prng32_xrp64() test sample");
for (i=0;i<100;i++) 
	{
		printf("%u \n",prng32_xrp64());
		
		
	}
	system("pause");
	return 0;
	
}

