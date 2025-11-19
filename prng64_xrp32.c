/* 
The XRP PRNG Xor Rotate Pearson Pseudo Random Number Generator
 * 0-CLAUSE BSD LICENSE.
 */


#include "prng64_xrp32.h"

#define SHIFTED_WORD_WIDTH 64

static xrp_state_t* get_xrp_state()
{
	static xrp_state_t xrp;
	return &xrp; 

}

static uint64_t
rotl64(uint64_t n, size_t shift)
{
    return (n << shift) | (n >> (SHIFTED_WORD_WIDTH - shift));
}

static uint64_t
rotr64(uint64_t n, size_t shift)
{
    return (n >> shift) | (n << (SHIFTED_WORD_WIDTH - shift));
}

#define BYTES_IN_WORD 8
#define TOTAL_PARAMS 4

static uint64_t 
get_word(uint64_t in, xrp_state_t* xrp)
{
	uint64_t  out =0;	
	size_t i=0;  
	uint8_t n =0;
	unsigned char* inb = (unsigned char*)(&in);
	unsigned char* outb = (unsigned char*)(&out);
	
	for (i=0;i<BYTES_IN_WORD;i++)
	{
		n = inb[i];
		n = XRP32_TABLE_ID[n];
		outb[i]=n;
	}
return out;
}

static uint64_t
pearson32(uint64_t in[TOTAL_PARAMS],xrp_state_t* xrp)
{
	size_t i;  
	uint64_t h=0;
	
    for (i = 0; i < TOTAL_PARAMS; ++i) 
	{         
			h^=in[i];
			h=get_word(h,xrp);			
    }
    
	return h;

}

static void
shuffle8bytes(uint64_t a, uint64_t b, xrp_state_t* xrp)
{
    unsigned char swap_buffer = 0;
    size_t i = 0;
    unsigned char* bytesa = (unsigned char*)(&a);
    unsigned char* bytesb = (unsigned char*)(&b);
	
		
for ( i = 0; i < BYTES_IN_WORD; i++) {
      
		swap_buffer = XRP32_TABLE_ID[bytesa[i]];
        XRP32_TABLE_ID[bytesa[i]]=XRP32_TABLE_ID[bytesb[i]];
		XRP32_TABLE_ID[bytesb[i]]=swap_buffer;
}
         
    return;
}
#define WORDS_IN_TABLE 32
uint64_t
prng64_xrp32(void)
{
	xrp_state_t* xrp = get_xrp_state();
	uint64_t const result=rotl64(xrp->x * 5, 7) * 9;
	
	uint64_t const t = xrp->x << 17;

	xrp->y ^= xrp->w;
	xrp->z ^= xrp->x;
	xrp->x ^= xrp->y;
	xrp->w ^= xrp->z;

	xrp->y  ^= t;
	xrp->z  = rotr64(xrp->z,19);

	
    ++(xrp->counter); (xrp->counter >= XRP_MAX) ? xrp->counter = 0 : 0;

	shuffle8bytes(xrp->x, xrp->y,xrp);
	
	uint64_t out[TOTAL_PARAMS];
	out[0]=result;
	out[1]=t;
	out[2]=xrp->y;
	out[3]=xrp->z;
	
	return pearson32(out,xrp);

}

struct splitmix64_state {
	uint64_t s;
};

uint64_t splitmix64(struct splitmix64_state *state) {
	uint64_t result = (state->s += 0x9E3779B97F4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

void
seed_xrp32(uint64_t seed)
{
   const unsigned char xrp32_canonical_table[TABLE_SIZE_BYTES] = {
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
    xrp_state_t* xrp = get_xrp_state();
	
	xrp->counter=0;
	
	struct splitmix64_state smstate = {seed};

	xrp->w = splitmix64(&smstate);
	xrp->x= splitmix64(&smstate);
	xrp->y=splitmix64(&smstate); 
	xrp->z= splitmix64(&smstate); 

	
   
   for (i= 0;i<TABLE_SIZE_BYTES;++i) { XRP32_TABLE_ID[i]=xrp32_canonical_table[i];}
   shuffle8bytes(seed,rotr64(seed,32),xrp);
   shuffle8bytes(seed,rotl64(seed,32),xrp);
   for(i = 0; i < (WORDS_IN_TABLE + seed % WORDS_IN_TABLE); ++i) {
		shuffle8bytes(prng64_xrp32(), prng64_xrp32(),xrp);
   }
   return;
}
#undef TABLE_SIZE_BYTES
#undef SHIFTED_WORD_WIDTH
#undef BYTES_IN_WORD
#undef WORDS_IN_TABLE
#undef TOTAL_PARAMS
