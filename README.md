# The XRP PRNG XOR Rotate Pair Pseudo Random Number Generator

Select operation mode(32 or 64 bit):

```C
#include "prng32_xrp64.h" 
#include "prng32_xrp64.c"
```
for 32 bit output unsigned numbers; xorshift128 algo; 

```C
#include "prng64_xrp32.h" 
#include "prng64_xrp32.c"
``` 
for 64 bit output unsigned numbers; xoshiro256ss algo; 

Explicitly define pairing mode in the beginning of the chosen header (pick one from list below):

- `#define PAIR_NULL_RAW`for crude non-crypto pseudo random 2 times faster than `rand()` (default behavior)

- `#define PAIR_TOY_TEST` non-crypto hashed pseudo random, slow, low quality, large mem footprint testing only

- `#define PAIR_CRYPTO_HASH` quasi-crypto hashed pseudo random, high quality, normal speed

- `#define PAIR_STREAM_CIPHER` stream cipher quasi-crypto pseudo random, high quality, normal speed


The point of pairing(or whitening) is to drop some speed for crypto security reasons because 
fast crypto secure statistically correct PRNG is non-existent thing(ISAAC)

To seed use: `seed_xrp64(seed)` or `seed_xrp32(seed)` call

To get next pseudo random number use: `prng32_xrp64()` or `prng64_xrp32()` call

To get number max value use: `XRP_MAX` macro

Not thread-safe

Again there is no such thing as partially crypto secure PRNG
so you should pay attention to small details and question everything every time to keep it crypto secure

Unfortunately whitening alone is not enough to magically make everything totally secure 
you should also secure initialization routine which use predictable PRNG
by initializing stream cipher(cryptohash) keys from system entropy source(or just XOR generator output against it again) 
which is platform dependable operation we can't do reliably here. 
After this final manual adjustment it's usable as a keystream. Sorry about that.

Good news that our initial intuitive guess about using xoshiro256ss as counter
for ChaCha20 encryption was right so we successfully secured around 300 MB/sec algorithmic keystream
perhaps one of the fastest on the planet(sic! checking it actual statistical quality we leave for experts)

I wrote it long ago as STEM student now I publish whole algorithm
(because I think it can interest you) and quit. Not going to maintain it anymore. 
However you are welcome to test this perfectly fine number crusher at anytime. 

It comes with 8 or more "toy pulse" simple tests allowing user to quickly ensure base generator functionality("vital signs")

It's based on pairing non-crypto xorshift128(or xoshiro256ss) with cryptohash(or symmetric stream cipher or non-crypto hash)

Anyway it demonstrates pairing PRNG with cryptohash or symmetric stream cipher opportunity
in order to slowly obtain slightly more high quality pseudo random numbers

It's totally free 0-BSD code. You can include it in your app or library


Do not underestimate it with best possible test circumstances `clang -O2 -DPAIR_NULL_RAW` it outputs ~2400 MB/sec
of pseudo random numbers from stock desktop computer device

Sure I'm aware about MT and WELL tech and math superiority. The point is not to maintain it but
explain to 5yo what it does and how it works. From this POV generators like `return 4;` or `date | md5`
are actually good one and "shifters" are far ahead than "twisters".
A broken clock is right twice a day.
Also deprecating an old generator in favor of new yields nothing.

P.S. “Anyone who considers arithmetical methods of producing random numbers is, of course, in a state of
sin.” - John von Neumann
