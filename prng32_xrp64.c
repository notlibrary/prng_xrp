/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */

#include "prng32_xrp64.h"

#define SHIFTED_WORD_WIDTH 32
#ifdef PAIR_STREAM_CIPHER
static uint32_t 
pack4(const uint8_t *a)
{
	uint32_t res = 0;
	res |= (uint32_t)a[0] << 0 * 8;
	res |= (uint32_t)a[1] << 1 * 8;
	res |= (uint32_t)a[2] << 2 * 8;
	res |= (uint32_t)a[3] << 3 * 8;
	return res;
}

static void
chacha20_init_block(chacha20_context_t *ctx, uint8_t key[], uint8_t nonce[])
{
	size_t i=0;
	
	for (i=0;i<sizeof (ctx->key);i++){
		ctx->key[i] = key[i];
	}
	for (i=0;i<sizeof(ctx->nonce);i++) {
		ctx->nonce[i] = nonce[i];
	}

	const uint8_t *magic_constant = (uint8_t*)"expand 32-byte k";
	ctx->state[0] = pack4(magic_constant + 0 * 4);
	ctx->state[1] = pack4(magic_constant + 1 * 4);
	ctx->state[2] = pack4(magic_constant + 2 * 4);
	ctx->state[3] = pack4(magic_constant + 3 * 4);
	ctx->state[4] = pack4(key + 0 * 4);
	ctx->state[5] = pack4(key + 1 * 4);
	ctx->state[6] = pack4(key + 2 * 4);
	ctx->state[7] = pack4(key + 3 * 4);
	ctx->state[8] = pack4(key + 4 * 4);
	ctx->state[9] = pack4(key + 5 * 4);
	ctx->state[10] = pack4(key + 6 * 4);
	ctx->state[11] = pack4(key + 7 * 4);
	ctx->state[12] = 0;
	ctx->state[13] = pack4(nonce + 0 * 4);
	ctx->state[14] = pack4(nonce + 1 * 4);
	ctx->state[15] = pack4(nonce + 2 * 4);
	for (i=0;i<sizeof(ctx->nonce);i++) {
		ctx->nonce[i] = nonce[i];
	}
}

static void
chacha20_block_set_counter(chacha20_context_t *ctx, uint64_t counter)
{
	ctx->state[12] = (uint32_t)counter;
	ctx->state[13] = pack4(ctx->nonce + 0 * 4) + (uint32_t)(counter >> 32);
}

static void 
chacha20_block_next(chacha20_context_t *ctx) {
	for (int i = 0; i < 16; i++) ctx->keystream32[i] = ctx->state[i];

#define CHACHA20_QUARTERROUND(x, a, b, c, d) \
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 16); \
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 12); \
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 8); \
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 7);

	for (int i = 0; i < 10; i++) 
	{
		CHACHA20_QUARTERROUND(ctx->keystream32, 0, 4, 8, 12)
		CHACHA20_QUARTERROUND(ctx->keystream32, 1, 5, 9, 13)
		CHACHA20_QUARTERROUND(ctx->keystream32, 2, 6, 10, 14)
		CHACHA20_QUARTERROUND(ctx->keystream32, 3, 7, 11, 15)
		CHACHA20_QUARTERROUND(ctx->keystream32, 0, 5, 10, 15)
		CHACHA20_QUARTERROUND(ctx->keystream32, 1, 6, 11, 12)
		CHACHA20_QUARTERROUND(ctx->keystream32, 2, 7, 8, 13)
		CHACHA20_QUARTERROUND(ctx->keystream32, 3, 4, 9, 14)
	}

	for (int i = 0; i < 16; i++) ctx->keystream32[i] += ctx->state[i];

	uint32_t *counter = ctx->state + 12;
	counter[0]++;
	if (0 == counter[0]) 
	{
		counter[1]++;
	}
}

static void
chacha20_init_context(chacha20_context_t *ctx, uint8_t key[], uint8_t nonce[], uint64_t counter)
{
	size_t i=0;
	uint8_t* ptr = (uint8_t*)(ctx);
	for (i=0;i<sizeof(chacha20_context_t);i++){
		ptr[i]=0;
	}

	chacha20_init_block(ctx, key, nonce);
	chacha20_block_set_counter(ctx, counter);

	ctx->counter = counter;
	ctx->position = 64;
}

static void
chacha20_xor(chacha20_context_t *ctx, uint8_t *bytes, size_t n_bytes)
{
	uint8_t *keystream8 = (uint8_t*)ctx->keystream32;
	for (size_t i = 0; i < n_bytes; i++) 
	{
		if (ctx->position >= 64) 
		{
			chacha20_block_next(ctx);
			ctx->position = 0;
		}
		bytes[i] ^= keystream8[ctx->position];
		ctx->position++;
	}
}
#endif
static xrp_state_t* get_xrp_state()
{
	static xrp_state_t xrp;
	return &xrp; 

}
static uint32_t
rotl32(uint32_t n, size_t shift)
{
    return (n << shift) | (n >> (SHIFTED_WORD_WIDTH - shift));
}
static uint32_t
rotr32(uint32_t n, size_t shift)
{
    return (n >> shift) | (n << (SHIFTED_WORD_WIDTH - shift));
}

#define BYTES_IN_WORD 4
#define TOTAL_PARAMS 4
#ifdef PAIR_TOY_TEST
static uint32_t
get_word(uint64_t in, xrp_state_t* xrp)
{
	uint32_t  out =0;	
	size_t i=0;  
	unsigned char n =0;
	unsigned char* inb = (unsigned char*)(&in);
	unsigned char* outb = (unsigned char*)(&out);
	for (i=0;i<BYTES_IN_WORD;i++)
	{
		n = inb[i];
		n = XRP64_TABLE_ID[n];
		outb[i]=n;
	}
return out;
}

static uint32_t
pearson64(uint32_t* in,xrp_state_t* xrp)
{
	size_t i;  
	uint32_t h=0;;	
	
    for (i = 0; i < TOTAL_PARAMS; ++i) 
	{       h^=in[i];
			h=get_word(h,xrp);			
    }
    
	return h;

}

static void
shuffle4bytes(uint32_t a, uint32_t b, xrp_state_t* xrp)
{
    unsigned char swap_buffer = 0;
    unsigned char* bytesa = (unsigned char*)(&a);
    unsigned char* bytesb = (unsigned char*)(&b);
	size_t i = 0;
		
for (i = 0; i < BYTES_IN_WORD; i++) {
    
		swap_buffer = XRP64_TABLE_ID[bytesa[i]];
        XRP64_TABLE_ID[bytesa[i]]=XRP64_TABLE_ID[bytesb[i]];
		XRP64_TABLE_ID[bytesb[i]]=swap_buffer;
}
            
return;
}
#endif
#define WORDS_IN_TABLE 64
uint32_t
prng32_xrp64(void)
{
	xrp_state_t* xrp = get_xrp_state();
	uint32_t t = xrp->z;

	uint32_t const s = xrp->w;
	xrp->z = xrp->y;
	xrp->y = xrp->x;
	xrp->x = s;

	t ^= t << 11;
	t ^= t >> 8;
	
	xrp->w= t ^ s ^ (s >> 19);
	
    ++(xrp->counter); (xrp->counter >=XRP_MAX ) ? xrp->counter = 0 : 0;
#ifdef PAIR_TOY_TEST	
	shuffle4bytes(xrp->x, xrp->y,xrp);
	uint32_t out[TOTAL_PARAMS];
	out[0]=xrp->w;
	out[1]=xrp->x;
	out[2]=xrp->y;
	out[3]=xrp->z;	
	return pearson64(out,xrp);	
#endif
#ifdef PAIR_NULL_RAW
	return xrp->w;
#endif

#ifdef PAIR_STREAM_CIPHER
	chacha20_xor(&xrp->ctx, (uint8_t*) (&xrp->w), 4);
	return xrp->w;
#endif
}


static uint64_t 
splitmix64(splitmix64_state_t *state)
 {
	uint64_t result = (state->s += 0x9E3779B97F4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

void
seed_xrp64(uint64_t seed)
{
   xrp_state_t* xrp = get_xrp_state();
   xrp->counter = 0;
   
	splitmix64_state_t smstate = {seed};

	uint64_t tmp = splitmix64(&smstate);
	xrp->w = (uint32_t)tmp;
	xrp->x = (uint32_t)(tmp >> 32);

	tmp = splitmix64(&smstate);
	xrp->y = (uint32_t)tmp;
	xrp->z = (uint32_t)(tmp >> 32);
  #ifdef PAIR_TOY_TEST 
  const unsigned char xrp64_canonical_table[TABLE_SIZE_BYTES] = {
       92,  6, 85,150, 36, 23,112,164,135,207,169,  5, 26, 64,165,219, //  1

       61, 20, 68, 89,130, 63, 52,102, 24,229,132,245, 80,216,195,115, //  2

       90,168,156,203,177,120,  2,190,188,  7,100,185,174,243,162, 10, //  3

      237, 18,253,225,  8,208,172,244,255,126,101, 79,145,235,228,121, //  4

      123,251, 67,250,161,  0,107, 97,241,111,181, 82,249, 33, 69, 55, //  5

       59,153, 29,  9,213,167, 84, 93, 30, 46, 94, 75,151,114, 73,222, //  6

      197, 96,210, 45, 16,227,248,201, 51,152,252,125, 81,206,215,186, //  7

       39,158,178,187,131,136,  1, 49, 50, 17,141, 91, 47,129, 60, 99, //  8

      154, 35, 86,171,105, 34, 38,200,147, 58, 77,118,173,246, 76,254, //  9

      133,232,196,144,198,124, 53,  4,108, 74,223,234,134,230,157,139, // 10

      189,205,199,128,176, 19,221,236,127,192,231, 70,233, 88,146, 44, // 11

      183,201, 22, 83, 13,214,116,109,159, 32, 95,226,140,220, 57, 12, // 12

      221, 31,209,182,143, 92,149,184,148, 62,113, 65, 37, 27,106,166, // 13

        3, 14,204, 72, 21, 41, 56, 66, 28,193, 40,217, 25, 54,179,117, // 14

      238, 87,240,155,180,170,242,212,191,163, 78,218,137,194,175,110, // 15

       43,119,224, 71,122,142, 42,160,104, 48,247,103, 15, 11,138,239  // 16

         };	
   size_t i = 0;
   for (i= 0;i<TABLE_SIZE_BYTES;++i) { XRP64_TABLE_ID[i]=xrp64_canonical_table[i];}
   
   shuffle4bytes(seed,(rotl32(prng32_xrp64(),16)) ,xrp);
   shuffle4bytes(seed,(rotr32(prng32_xrp64(),16)) ,xrp);
   for(i = 0; i < (WORDS_IN_TABLE + seed % WORDS_IN_TABLE); ++i) {
       shuffle4bytes(prng32_xrp64(), prng32_xrp64(),xrp);
   }
#endif
#ifdef PAIR_STREAM_CIPHER
unsigned char* bytesseed = (unsigned char*)(&seed);
uint64_t noncei = splitmix64(&smstate); 
unsigned char* bytesnonce = (unsigned char*)(&noncei);
uint8_t key[32];
uint8_t nonce[12];

size_t i=0;
for (i=0;i<8;++i){
	nonce[i]=bytesnonce[i];
}
for (i=8;i<12;++i){
	nonce[i]=bytesnonce[12-i];
}

for (i=0;i<16;++i){
	key[i]=bytesseed[i];
}
for (i=16;i<32;++i){
	key[i]=bytesseed[32-i];
}
chacha20_init_context(&xrp->ctx,key, nonce,0);

#endif
   return;
}
#undef TABLE_SIZE_BYTES
#undef SHIFTED_WORD_WIDTH
#undef BYTES_IN_WORD
#undef WORDS_IN_TABLE
#undef TOTAL_PARAMS
