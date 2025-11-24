/* 
The XRP PRNG Xor Rotate Pair Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include "prng64_xrp32.h"
#include "prng64_xrp32.c"

/* So gcc with -O1 -O2 -O3 gives ~20 millions random numbers per second
for rng64_xrp32()
while rand() makes ~92 millions per second 
	not bad result only 4-5 times slower
	with clang and -O2 key it perfoms only 2 times slower than rand()
	even better
	*/

int 
main()
{
	size_t i;
	seed_xrp32(65535);
	printf("%s \n","prng64_xrp32() brute force sample");
	clock_t begin = clock();
	
	for (i=0;i<1000000;i++) 
	{
		prng64_xrp32();	
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("%s %lf \n","1000000 done in:", time_spent	);

	
	begin = clock();
	for (i=0;i<10000000;i++) 
	{
		prng64_xrp32();	
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("%s %lf \n\n","10000000 done in:", time_spent);



	srand(65635);
	printf("%s \n","stdlib.h rand() brute force sample");
	begin = clock();
	
	for (i=0;i<1000000;i++) 
	{
		rand();	
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("%s %lf \n","1000000 done in:", time_spent	);

	begin = clock();
	for (i=0;i<10000000;i++) 
	{
		rand();	
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("%s %lf \n","10000000 done in:", time_spent);

	system("pause");
	return 0;

}
