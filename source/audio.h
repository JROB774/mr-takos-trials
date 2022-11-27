#ifndef AUDIO_H__ /*//////////////////////////////////////////////////////////*/
#define AUDIO_H__

#define DECLARE_AUDIO_TYPE(name) struct name##__Type; typedef struct name##__Type* name;
#define DEFINE_AUDIO_TYPE(name) struct name##__Type

#define INVALID_SOUND_REF 0xFFFFFFFF

typedef nkU32 SoundRef;

DECLARE_AUDIO_TYPE(Sound);
DECLARE_AUDIO_TYPE(Music);

static void audio_init(void);
static void audio_quit(void);

static void set_sound_volume(nkF32 volume); // [0-1]
static void set_music_volume(nkF32 volume); // [0-1]

static nkF32 get_sound_volume(void);
static nkF32 get_music_volume(void);

static nkBool is_sound_on(void);
static nkBool is_music_on(void);

static nkBool is_music_playing(void);

static Sound    sound_create_from_file(const nkChar* file_name);
static Sound    sound_create_from_data(void* data, nkU64 size);
static void     sound_destroy         (Sound sound);
static SoundRef sound_play            (Sound sound, nkS32 loops);
static SoundRef sound_play_on_channel (Sound sound, nkS32 loop, nkS32 channel);
static void     sound_resume          (SoundRef sound_ref);
static void     sound_pause           (SoundRef sound_ref);
static void     sound_stop            (SoundRef sound_ref);
static void     sound_fade_out        (SoundRef sound_ref, nkF32 seconds);

static Music music_create_from_file(const nkChar* file_name);
static Music music_create_from_data(void* data, nkU64 size);
static void  music_destroy         (Music music);
static void  music_play            (Music music, nkS32 loops);
static void  music_resume          (void);
static void  music_pause           (void);
static void  music_stop            (void);

#endif /* AUDIO_H__ //////////////////////////////////////////////////////////*/
