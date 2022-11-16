#ifndef RANDOM_H__ /*/////////////////////////////////////////////////////////*/
#define RANDOM_H__

static void  rng_init     (nkU32 seed);
static nkS32 rng_int      (void);
static nkS32 rng_int_range(nkS32 min, nkS32 max);
static nkF32 rng_num      (void);
static nkF32 rng_num_range(nkF32 min, nkF32 max);

#endif /* RANDOM_H__ *////////////////////////////////////////////////////////*/
