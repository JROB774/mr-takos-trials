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
    imm_begin_texture_batch(g_asset_particle);
    for(nkU32 i=0; i<MAX_PARTICLES; ++i)
    {
        Particle* p = &g_particles[i];
        if(p->alive && p->type != ParticleType_None)
        {
            PARTICLE_HOOKS[p->type].render(p,i);
        }
    }
    imm_end_texture_batch();
}

//
// None
//

static void particle_none_create(Particle* p)
{
    // Nothing...
}

static void particle_none_update(Particle* p, nkF32 dt)
{
    // Nothing...
}

static void particle_none_render(Particle* p, nkU32 i)
{
    // Nothing...
}

//
// Star
//

#define PARTICLE_STAR_MIN_TIME 0.3f
#define PARTICLE_STAR_MAX_TIME 0.3f

static void particle_star_create(Particle* p)
{
    p->frame = ATLAS_PARTICLE_STAR_0_SHADOW + ((rng_int_range(0,4) * 2) + 1);
    p->time = rng_num_range(PARTICLE_STAR_MIN_TIME,PARTICLE_STAR_MAX_TIME);
}

static void particle_star_update(Particle* p, nkF32 dt)
{
    p->time -= dt;
    if(p->time <= 0.0f)
    {
        p->alive = NK_FALSE;
    }
}

static void particle_star_render(Particle* p, nkU32 i)
{
    render_item_ex(p->pos.x,p->pos.y, 1,1, g_angles_big[i%APP_MAX_ANGLES], ATLAS_PARTICLE, p->frame, 0.5f);
}

/*////////////////////////////////////////////////////////////////////////////*/
