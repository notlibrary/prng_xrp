/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */


#ifndef RNG64_XRP32_H
#define RNG64_XRP32_H
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

uint64_t prng64_xrp32(void);
void seed_xrp32(uint64_t seed);
#define XRP32_TABLE_ID xrp64t

#define TABLE_SIZE_BYTES 256
/* The table must be seeded with DISTINCT 0-255 chars in random order. */
extern unsigned char XRP64_TABLE_ID[TABLE_SIZE_BYTES];
#define SHIFTED_WORD_WIDTH 64
#define XRP_MAX ULONG_MAX
#define BYTES_IN_WORD 8
#define WORDS_IN_TABLE 32
typedef struct 
{
	uint64_t w;
	uint64_t x;
	uint64_t y;
	uint64_t z;
	uint64_t counter;	
}xrp_state_t;
#ifdef __cplusplus
}
#endif //__cpluplus
#endif //RNG64_XRP32_H
