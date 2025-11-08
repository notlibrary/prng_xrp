# The XRP PRNG XOR Rotate Pearson Pseudo Random Number Generator

Select operation mode(32 or 64 bit):

```C
#include "prng32_xrp64.h" 
#include "prng32_xrp64.c"
```
for 32 bit output numbers; 64 words per table; 256 bytes per table

```C
#include "prng64_xrp32.h" 
#include "prng64_xrp32.c"
``` 
for 64 bit output numbers; 32 words per table; 256 bytes per table

To seed use: `seed_xrp64(seed)` or `seed_xrp32(seed)` call

To get next pseudo random number use: `prng32_xrp64()` or `prng64_xrp32()` call

To get number max value use: `XRP_MAX` macro

It needs (512 + 40) bytes of memory to operate(reshuffle hash table each round and store default table plus some internal state) 

Not thread-safe

I wrote it long ago as STEM student now I publish whole algorithm
(because I think it can interest you) and quit. Not going to maintain it anymore. 
However you are welcome to test this perfectly fine number crusher at anytime. 

It's based on XORshift with additional Pearson hashing
and it's randomness passes diehard because of additional hash table reshuffle back link(feedback loop)

It's totally free 0-BSD code. You can include it in your app or library

Do not underestimate it with best possible test circumstances `clang -O2` it outputs ~240 MB/sec
of random numbers from stock desktop computer device
