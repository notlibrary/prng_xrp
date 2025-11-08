/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#ifndef RNG32_XRP64_H
#define RNG32_XRP64_H
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

uint32_t prng32_xrp64(void);
void seed_xrp64(uint32_t seed);
#define XRP64_TABLE_ID xrp64t

#define TABLE_SIZE_BYTES 256
/* The table must be seeded with DISTINCT 0-255 chars in random order. */
extern unsigned char XRP64_TABLE_ID[TABLE_SIZE_BYTES];
#define SHIFTED_WORD_WIDTH 32
#define XRP_MAX UINT_MAX
#define BYTES_IN_WORD 4
#define WORDS_IN_TABLE 64
typedef struct 
{
	uint32_t w;
	uint32_t x;	
	uint32_t y;
	uint32_t z;
	uint32_t counter;	
}xrp_state_t;

#ifdef __cplusplus
}
#endif //__cpluplus
#endif //RNG32_XRP64_H
