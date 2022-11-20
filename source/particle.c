/*////////////////////////////////////////////////////////////////////////////*/

#define MAX_PARTICLES 1024

static Particle g_particles[MAX_PARTICLES];

static void particles_spawn(ParticleType type, nkF32 x, nkF32 y, nkF32 w, nkF32 h, nkF32 min, nkF32 max)
{
    nkS32 amount = rng_int_range(min,max);
    for(nkS32 i=0; i<amount; ++i)
    {
        Particle* p = &g_particles[i];
        if(!p->alive)
        {
            memset(p, 0, sizeof(*p));

            p->alive = NK_TRUE;
            p->type = type;
            p->pos.x = rng_num_range(x,x+w);
            p->pos.y = rng_num_range(y,y+h);

            PARTICLE_HOOKS[p->type].create(p);
        }
    }
}

static void particles_update(nkF32 dt)
{
    for(nkU32 i=0; i<MAX_PARTICLES; ++i)
    {
        Particle* p = &g_particles[i];
        if(p->alive && p->type != ParticleType_None)
        {
            PARTICLE_HOOKS[p->type].update(p, dt);
        }
    }
}

static void particles_render(void)
{
    for(nkU32 i=0; i<MAX_PARTICLES; ++i)
    {
        Particle* p = &g_particles[i];
        if(p->alive && p->type != ParticleType_None)
        {
            PARTICLE_HOOKS[p->type].render(p);
        }
    }
}

/*////////////////////////////////////////////////////////////////////////////*/
