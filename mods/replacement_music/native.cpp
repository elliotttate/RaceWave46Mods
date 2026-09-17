#include "recomp.h"
#include "wr64_music.hpp"
#include <Windows.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <mutex>
#include <vector>

#define EXPORT extern "C" __declspec(dllexport)
EXPORT uint32_t recomp_api_version = 1;
namespace music = wr64::music;
namespace {
constexpr uint32_t players = 0x8003FCC8, stride = 0x140;
int32_t address(unsigned p) { return int32_t(players + p * stride); }
unsigned index(uint32_t ptr) {
    const auto offset = ptr - players;
    return offset < 4 * stride && offset % stride == 0 ? offset / stride : 4;
}
bool musical(unsigned p) { return p > 0 && p < 4; }
float real(uint8_t* rdram, int32_t ptr, int offset) {
    uint32_t bits = MEM_W(offset, ptr);
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
void initialize() {
    static std::once_flag once;
    std::call_once(once, [] {
        HMODULE module = nullptr;
        wchar_t path[32768];
        if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&initialize), &module)) return;
        const DWORD length = GetModuleFileNameW(module, path, DWORD(std::size(path)));
        if (!length || length >= std::size(path)) return;
        const auto directory = std::filesystem::path(path).parent_path() / "racewave_music";
        music::initialize(directory);
        std::fprintf(stderr, "[music-mod] Native mod loaded; recordings resolved beside DLL.\n");
    });
}
thread_local std::array<unsigned, 4> loading{};
thread_local std::array<bool, 4> active{}, suppressed{};
struct Load { unsigned player; uint32_t sequence; bool valid; };
thread_local std::vector<Load> loads;
struct Sound { unsigned player; int32_t scale; bool suppress; };
thread_local std::vector<Sound> sounds;
void stop(unsigned p) {
    if (active[p]) music::sequence_stopped(p);
    active[p] = suppressed[p] = false;
}
void publish(uint8_t* rdram) {
    for (unsigned p = 1; p < 4; ++p) {
        int32_t ptr = address(p);
        uint8_t flags = MEM_BU(0, ptr), behavior = MEM_BU(3, ptr);
        if (!(flags & 0x80)) { stop(p); continue; }
        float gain = real(rdram, ptr, 0x18) * real(rdram, ptr, 0x28);
        if ((flags & 0x20) && (behavior & 0x20)) gain *= real(rdram, ptr, 0x24);
        if ((flags & 0x20) && (behavior & 0x40)) gain = 0;
        if (!std::isfinite(gain) || gain < 0) gain = 0;
        music::sequence_state(p, MEM_BU(4, ptr), gain,
            (flags & 0x18) || ((flags & 0x20) && (behavior & 0x80)));
        active[p] = true;
    }
}
}

EXPORT void rw_music_load_enter(uint8_t* rdram, recomp_context* ctx) {
    initialize();
    const unsigned p = uint32_t(ctx->r4), sequence = uint32_t(ctx->r5);
    loads.push_back({p, sequence, sequence < MEM_HU(0, int32_t(0x80045514))});
    if (musical(p)) ++loading[p];
}
EXPORT void rw_music_load_return(uint8_t* rdram, recomp_context*) {
    if (loads.empty()) return;
    const auto load = loads.back(); loads.pop_back();
    const auto p = load.player;
    if (!musical(p)) return;
    --loading[p];
    const auto ptr = address(p);
    if (load.valid && (MEM_BU(0, ptr) & 0x80) && MEM_BU(4, ptr) == load.sequence) {
        music::sequence_started(p, uint8_t(load.sequence)); active[p] = true;
    } else if (!(MEM_BU(0, ptr) & 0x80)) stop(p);
}
EXPORT void rw_music_disable(uint8_t*, recomp_context* ctx) {
    unsigned p = index(uint32_t(ctx->r4));
    if (musical(p) && !loading[p]) stop(p);
}
EXPORT void rw_music_sound_enter(uint8_t* rdram, recomp_context* ctx) {
    const auto ptr = int32_t(ctx->r4);
    const auto p = index(uint32_t(ptr));
    bool suppress = musical(p) && (MEM_BU(0, ptr) & 0x80) && music::replacement_available(MEM_BU(4, ptr));
    sounds.push_back({p, musical(p) ? MEM_W(0x28, ptr) : 0, suppress});
    if (!musical(p)) return;
    if (suppress || suppressed[p]) MEM_B(0, ptr) = MEM_BU(0, ptr) | 4;
    if (suppress) MEM_W(0x28, ptr) = 0;
}
EXPORT void rw_music_sound_return(uint8_t* rdram, recomp_context*) {
    if (sounds.empty()) return;
    const auto sound = sounds.back(); sounds.pop_back();
    if (!musical(sound.player)) return;
    const auto ptr = address(sound.player);
    if (sound.suppress) MEM_W(0x28, ptr) = sound.scale;
    suppressed[sound.player] = sound.suppress && (MEM_BU(0, ptr) & 0x80);
}
EXPORT void rw_music_tick(uint8_t* rdram, recomp_context* ctx) {
    initialize();
    music::set_replacement_volume(std::min(uint32_t(ctx->r4), 100u));
    // AudioThread_CreateTask queues this completed triple buffer before doing
    // synthesis. Mirror its early-return condition and next queue index exactly.
    // No ROM instructions or host callbacks are replaced by this mod.
    const auto divisor = MEM_H(0, int32_t(0x80045520));
    const uint32_t tick = uint32_t(MEM_W(0, int32_t(0x80045550))) + 1;
    if (divisor <= 0 || tick % uint32_t(divisor)) return;
    const auto old_index = MEM_W(0, int32_t(0x8004555C));
    if (old_index < 0 || old_index > 2) return;
    const auto slot = (old_index + 2) % 3;
    const auto frames = MEM_H(slot * 2, int32_t(0x80045624));
    const uint32_t ptr = MEM_W(slot * 4, int32_t(0x80045618));
    const uint32_t rate = MEM_HU(0, int32_t(0x80045524));
    const uint32_t offset = ptr - 0x80000000u;
    if (frames <= 0 || frames > 4096 || rate < 8000 || offset > 0x800000u - frames * 4u || (offset & 3)) return;
    static thread_local std::vector<int16_t> pcm;
    pcm.resize(size_t(frames) * 2);
    // Convert RDRAM's word-swapped stereo to interleaved L/R, then restore it
    // before the original host resampler and master-volume stage consume it.
    for (int i = 0; i < frames * 2; ++i) pcm[i] = MEM_H(i * 2, int32_t(ptr));
    music::mix(pcm.data(), pcm.size(), rate);
    for (int i = 0; i < frames * 2; ++i) MEM_H(i * 2, int32_t(ptr)) = pcm[i];
    static bool reported = false;
    if (!reported) { std::fprintf(stderr, "[music-mod] Mixing into original AI buffer at %u Hz.\n", rate); reported = true; }
}
EXPORT void rw_music_state(uint8_t* rdram, recomp_context*) { publish(rdram); }
