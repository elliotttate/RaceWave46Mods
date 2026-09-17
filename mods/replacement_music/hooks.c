// Standard N64Recomp hooks. Only this small bridge is MIPS code; the recordings
// and mixer live in a native sidecar, without replacing the game executable.
#define IMPORT(module) __attribute__((section(".recomp_import." module), noinline, used))
#define HOOK(name) __attribute__((section(".recomp_hook." name), used))
#define RETURN(name) __attribute__((section(".recomp_hook_return." name), used))
typedef unsigned int u32;
IMPORT(".") void rw_music_load_enter(u32 player, u32 sequence) {}
IMPORT(".") void rw_music_load_return(void) {}
IMPORT(".") void rw_music_disable(u32 player) {}
IMPORT(".") void rw_music_sound_enter(u32 player) {}
IMPORT(".") void rw_music_sound_return(void) {}
IMPORT(".") void rw_music_tick(u32 volume) {}
IMPORT(".") void rw_music_state(void) {}
IMPORT("*") u32 recomp_get_config_u32(const char* key) { return 0; }

HOOK("Audio_LoadSequence") void load_enter(u32 player, u32 sequence) { rw_music_load_enter(player, sequence); }
RETURN("Audio_LoadSequence") void load_return(void) { rw_music_load_return(); }
HOOK("AudioSeq_SequencePlayerDisable") void disable(u32 player) { rw_music_disable(player); }
HOOK("Audio_SequencePlayerProcessSound") void sound_enter(u32 player) { rw_music_sound_enter(player); }
RETURN("Audio_SequencePlayerProcessSound") void sound_return(void) { rw_music_sound_return(); }
HOOK("AudioThread_CreateTask") void tick(void) { rw_music_tick(recomp_get_config_u32("volume")); }
RETURN("AudioThread_CreateTask") void state(void) { rw_music_state(); }
