/*////////////////////////////////////////////////////////////////////////////*/

#define MIXER_FREQUENCY     MIX_DEFAULT_FREQUENCY
#define MIXER_SAMPLE_FORMAT MIX_DEFAULT_FORMAT
#define MIXER_CHANNELS      2 // Stereo Sound
#define MIXER_SAMPLE_SIZE   2048

#define ALLOCATE_AUDIO_TYPE(name) malloc(sizeof(struct name##__Type))

DEFINE_AUDIO_TYPE(Sound)
{
    Mix_Chunk* chunk;
};

DEFINE_AUDIO_TYPE(Music)
{
    Mix_Music* music;
};

typedef struct AudioContext
{
    nkF32 sound_volume;
    nkF32 music_volume;
}
AudioContext;

static AudioContext g_audio;

static void audio_init(void)
{
    if(!NK_CHECK_FLAGS(Mix_Init(MIX_INIT_OGG), MIX_INIT_OGG))
        fatal_error("Failed to initialize SDL2 Mixer OGG support: %s", Mix_GetError());
    if(Mix_OpenAudio(MIXER_FREQUENCY, MIXER_SAMPLE_FORMAT, MIXER_CHANNELS, MIXER_SAMPLE_SIZE) != 0)
        fatal_error("Failed to open SDL2 Mixer audio device: %s", Mix_GetError());
    Mix_AllocateChannels(64);

    // @Incomplete: Load these from a file!
    set_sound_volume(1.0f);
    set_music_volume(1.0f);
}

static void audio_quit(void)
{
    Mix_CloseAudio();
    Mix_Quit();
}

static void set_sound_volume(nkF32 volume)
{
    g_audio.sound_volume = nk_clamp(volume, 0.0f, 1.0f);
    nkS32 ivolume = NK_CAST(nkS32, NK_CAST(nkF32, MIX_MAX_VOLUME) * g_audio.sound_volume);
    Mix_Volume(-1, ivolume);
}

static void set_music_volume(nkF32 volume)
{
    g_audio.music_volume = nk_clamp(volume, 0.0f, 1.0f);
    nkS32 ivolume = NK_CAST(nkS32, NK_CAST(nkF32, MIX_MAX_VOLUME) * g_audio.music_volume);
    Mix_VolumeMusic(ivolume);
}

static nkF32 get_sound_volume(void)
{
    return g_audio.sound_volume;
}

static nkF32 get_music_volume(void)
{
    return g_audio.music_volume;
}

static nkBool is_sound_on(void)
{
    return (g_audio.sound_volume > 0.0f);
}

static nkBool is_music_on(void)
{
    return (g_audio.music_volume > 0.0f);
}

static nkBool is_music_playing(void)
{
    return Mix_PlayingMusic();
}

static Sound sound_create(const nkChar* file_name)
{
    Sound sound = ALLOCATE_AUDIO_TYPE(Sound);
    if(!sound)
        fatal_error("Failed to allocate sound!");
    sound->chunk = Mix_LoadWAV(file_name);
    if(!sound->chunk)
        fatal_error("Failed to load sound: %s (%s)", file_name, Mix_GetError());
    return sound;
}

static void sound_destroy(Sound sound)
{
    if(!sound) return;
    Mix_FreeChunk(sound->chunk);
    free(sound);
}

static SoundRef sound_play(Sound sound, nkS32 loops)
{
    NK_ASSERT(sound);
    nkS32 channel = Mix_PlayChannel(-1, sound->chunk, loops);
    if(channel == -1)
        printf("Failed to play sound effect: %s\n", Mix_GetError());
    return NK_CAST(SoundRef,channel);
}

static void sound_stop(SoundRef sound_ref)
{
    Mix_HaltChannel(sound_ref);
}

static Music music_create(const nkChar* file_name)
{
    Music music = ALLOCATE_AUDIO_TYPE(Music);
    if(!music)
        fatal_error("Failed to allocate music!");
    music->music = Mix_LoadMUS(file_name);
    if(!music->music)
        fatal_error("Failed to load music: %s (%s)", file_name, Mix_GetError());
    return music;
}

static void music_destroy(Music music)
{
    if(!music) return;
    Mix_FreeMusic(music->music);
    free(music);
}

static void music_play(Music music, nkS32 loops)
{
    NK_ASSERT(music);
    if(Mix_PlayMusic(music->music, loops) == -1)
        printf("Failed to play music: %s\n", Mix_GetError());
}

static void music_resume(void)
{
    Mix_ResumeMusic();
}

static void music_pause(void)
{
    Mix_PauseMusic();
}

static void music_stop(void)
{
    Mix_HaltMusic();
}

/*////////////////////////////////////////////////////////////////////////////*/