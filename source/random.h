#ifndef RANDOM_H__ /*/////////////////////////////////////////////////////////*/
#define RANDOM_H__

typedef struct RNG
{
   nkU32 a;
   nkU32 b;
}
RNG;

static void  rng_init     (RNG* rng, nkU32 seed);
static nkS32 rng_int      (RNG* rng);
static nkS32 rng_int_range(RNG* rng, nkS32 min, nkS32 max);
static nkF32 rng_num      (RNG* rng);
static nkF32 rng_num_range(RNG* rng, nkF32 min, nkF32 max);

#endif /* RANDOM_H__ *////////////////////////////////////////////////////////*/
