/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "prng64_xrp32.h"
#include "prng64_xrp32.c"


int 
main()
{

size_t i;
seed_xrp32(0x0A0B0C0D0A0B0C0D);
printf("%s \n","prng64_xrp32() test sample");
for (i=0;i<100;i++) 
	{
		printf("%llu \n",prng64_xrp32());
		
		
	}
	system("pause");
	return 0;
}

