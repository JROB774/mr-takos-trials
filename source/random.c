/*////////////////////////////////////////////////////////////////////////////*/

static void rng_init(RNG* rng, nkU32 seed)
{
    NK_ASSERT(rng);
    rng->a = ((seed      ) & 0xFFFF);
    rng->b = ((seed >> 16) & 0xFFFF);
}

// https://www.codeproject.com/Articles/25172/Simple-Random-Number-Generation
static nkS32 rng_int(RNG* rng)
{
    NK_ASSERT(rng);

    rng->a = 36969 * (rng->a & 65535) + (rng->a >> 16);
    rng->b = 18000 * (rng->b & 65535) + (rng->b >> 16);

    return (((rng->a << 16) + rng->b) & NK_S32_MAX);
}

static nkS32 rng_int_range(RNG* rng, nkS32 min, nkS32 max)
{
    return ((rng_int(rng) % ((max+1)-min)) + min);
}

static nkF32 rng_num(RNG* rng)
{
    return (NK_CAST(nkF32,rng_int(rng)) / NK_CAST(nkF32,NK_S32_MAX));
}

static nkF32 rng_num_range(RNG* rng, nkF32 min, nkF32 max)
{
    return (min + NK_CAST(nkF32,rng_int(rng)) / (NK_CAST(nkF32,NK_S32_MAX) / max-min));
}

/*////////////////////////////////////////////////////////////////////////////*/
