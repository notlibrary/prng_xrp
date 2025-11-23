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
void seed_xrp64(uint64_t seed);
#define XRP64_TABLE_ID xrp->table

#define TABLE_SIZE_BYTES 256
/* The table must be seeded with DISTINCT 0-255 chars in random order. */
#define SHIFTED_WORD_WIDTH 32
#define XRP_MAX UINT_MAX
#define BYTES_IN_WORD 4
#define WORDS_IN_TABLE 64

#ifdef PAIR_STREAM_CIPHER
typedef struct 
{
	uint32_t keystream32[16];
	size_t position;

	uint8_t key[32];
	uint8_t nonce[12];
	uint64_t counter;

	uint32_t state[16];
}chacha20_context_t;
#endif
typedef struct 
{
#ifdef PAIR_TOY_TEST
	unsigned char table[TABLE_SIZE_BYTES];
#endif
	uint32_t w;
	uint32_t x;	
	uint32_t y;
	uint32_t z;
	uint32_t counter;
#ifdef PAIR_STREAM_CIPHER
	chacha20_context_t ctx;
#endif
}xrp_state_t;
typedef struct  
{
	uint64_t s;
}splitmix64_state_t;

#if !defined (PAIR_TOY_TEST) && !defined (PAIR_CRYPTO_HASH) && !defined (PAIR_STREAM_CIPHER)
#define PAIR_NULL_RAW
#endif

#ifdef __cplusplus
}
#endif //__cpluplus
#endif //RNG32_XRP64_H
