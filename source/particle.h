#ifndef PARTICLE_H__ /*///////////////////////////////////////////////////////*/
#define PARTICLE_H__

NK_ENUM(ParticleType, nkS32)
{
    ParticleType_None,
    ParticleType_TOTAL
};

typedef struct Particle
{
    ParticleType type;
    nkVec2       pos;
    nkVec2       vel;
    nkU32        frame;
    nkF32        time;
    nkBool       alive;
}
Particle;

typedef void(*ParticleCreate)(Particle*);
typedef void(*ParticleUpdate)(Particle*, nkF32);
typedef void(*ParticleRender)(Particle*);

typedef struct ParticleHooks
{
    ParticleCreate create;
    ParticleUpdate update;
    ParticleRender render;
}
ParticleHooks;

static const ParticleHooks PARTICLE_HOOKS[] =
{
{ NULL, NULL, NULL }
};

NK_STATIC_ASSERT(ParticleType_TOTAL == NK_ARRAY_SIZE(PARTICLE_HOOKS), particle_hook_array_size_mismatch);

static void particles_spawn (ParticleType type, nkF32 x, nkF32 y, nkF32 w, nkF32 h, nkF32 min, nkF32 max);
static void particles_update(nkF32 dt);
static void particles_render(void);

#endif /* PARTICLE_H__ ///////////////////////////////////////////////////////*/
