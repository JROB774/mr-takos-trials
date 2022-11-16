/*////////////////////////////////////////////////////////////////////////////*/

static void rng_init(nkU32 seed)
{
    srand(seed);
}

static nkS32 rng_int(void)
{
    return rand();
}

static nkS32 rng_int_range(nkS32 min, nkS32 max)
{
    return ((rng_int() % ((max+1)-min)) + min);
}

static nkF32 rng_num(void)
{
    return (NK_CAST(nkF32,rng_int()) / NK_CAST(nkF32,RAND_MAX));
}

static nkF32 rng_num_range(nkF32 min, nkF32 max)
{
    return (min + NK_CAST(nkF32,rng_int()) / (NK_CAST(nkF32,RAND_MAX) / (max-min)));
}

/*////////////////////////////////////////////////////////////////////////////*/
