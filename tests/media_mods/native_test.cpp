#include "recomp.h"
#include "wr64_music.hpp"
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
extern "C" {
void rw_music_load_enter(uint8_t*, recomp_context*);
void rw_music_load_return(uint8_t*, recomp_context*);
void rw_music_disable(uint8_t*, recomp_context*);
void rw_music_sound_enter(uint8_t*, recomp_context*);
void rw_music_sound_return(uint8_t*, recomp_context*);
void rw_music_tick(uint8_t*, recomp_context*);
void rw_music_state(uint8_t*, recomp_context*);
}
namespace {
struct State { int starts = 0, stops = 0; float gain = 0; bool paused = false; };
std::array<State, 4> state;
bool replacement = true;
unsigned mixes = 0;
double volume;
void require(bool ok, const char* why) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", why); std::exit(1); }
}
int32_t address(unsigned p) { return int32_t(0x8003FCC8 + p * 0x140); }
void put(uint8_t* rdram, int32_t ptr, int offset, float f) {
    int32_t bits; std::memcpy(&bits, &f, 4); MEM_W(offset, ptr) = bits;
}
float real(uint8_t* rdram, int32_t ptr, int offset) {
    int32_t bits = MEM_W(offset, ptr); float f; std::memcpy(&f, &bits, 4); return f;
}
}
namespace wr64::music {
void initialize(const std::filesystem::path&, const std::filesystem::path&) {}
bool replacement_available(uint8_t sequence) { return replacement && sequence == 7; }
void sequence_started(unsigned p, uint8_t) { ++state[p].starts; }
void sequence_stopped(unsigned p) { ++state[p].stops; }
void sequence_state(unsigned p, uint8_t, float gain, bool paused) { state[p].gain = gain; state[p].paused = paused; }
void set_replacement_volume(double value) { volume = value; }
void mix(int16_t* samples, size_t count, uint32_t rate) {
    require(count == 4 && rate == 32000, "exact native buffer extent and frequency");
    require(samples[0] == 11 && samples[1] == 22 && samples[2] == 33 && samples[3] == 44,
            "word-swapped RDRAM decodes to logical left/right");
    samples[0] = 111; samples[1] = 222; samples[2] = 333; samples[3] = 444;
    ++mixes;
}
}
int main() {
    std::vector<uint8_t> memory(8 * 1024 * 1024);
    auto* rdram = memory.data(); recomp_context ctx{};
    MEM_H(0, int32_t(0x80045514)) = 20;
    const auto ptr = address(1);
    for (int restart = 0; restart < 2; ++restart) {
        ctx.r4 = 1; ctx.r5 = 7; rw_music_load_enter(rdram, &ctx);
        ctx.r4 = ptr; rw_music_disable(rdram, &ctx);
        MEM_B(0, ptr) = int8_t(0x80); MEM_B(4, ptr) = 7;
        // Return hooks must not depend on registers clobbered by the native body.
        ctx.r4 = 0; ctx.r5 = 0; rw_music_load_return(rdram, &ctx);
    }
    require(state[1].starts == 2 && state[1].stops == 0, "same cue restart; nested disable ignored");
    put(rdram, ptr, 0x18, 0.5f); put(rdram, ptr, 0x28, 0.8f);
    ctx.r4 = ptr; rw_music_sound_enter(rdram, &ctx);
    require(real(rdram, ptr, 0x28) == 0, "native music synthesis muted");
    ctx.r4 = address(0); rw_music_sound_return(rdram, &ctx);
    require(real(rdram, ptr, 0x28) == 0.8f, "fade scale restored despite clobbered argument");
    rw_music_state(rdram, &ctx);
    require(std::abs(state[1].gain - 0.4f) < 0.0001f, "real gain published");
    MEM_B(0, ptr) = int8_t(0xA0); MEM_B(3, ptr) = int8_t(0x80);
    rw_music_state(rdram, &ctx); require(state[1].paused, "script pause respected");
    MEM_B(3, ptr) = 0x20; put(rdram, ptr, 0x24, 0.5f);
    rw_music_state(rdram, &ctx);
    require(!state[1].paused && std::abs(state[1].gain - 0.2f) < 0.0001f, "soft mute respected");
    MEM_B(0, ptr) = int8_t(0x80); replacement = false; ctx.r4 = ptr;
    rw_music_sound_enter(rdram, &ctx);
    require(real(rdram, ptr, 0x28) == 0.8f && (MEM_BU(0, ptr) & 4), "missing track restores native synthesis");
    rw_music_sound_return(rdram, &ctx);
    replacement = true; ctx.r4 = address(0);
    MEM_B(0, address(0)) = int8_t(0x80); MEM_B(4, address(0)) = 7;
    put(rdram, address(0), 0x28, 0.7f);
    rw_music_sound_enter(rdram, &ctx); rw_music_sound_return(rdram, &ctx);
    require(real(rdram, address(0), 0x28) == 0.7f && state[0].starts == 0, "SFX/announcer untouched");
    ctx.r4 = ptr; rw_music_disable(rdram, &ctx); MEM_B(0, ptr) = 0;
    rw_music_state(rdram, &ctx); require(state[1].stops == 1, "natural ending stopped once");

    // Validate every triple-buffer permutation and early-return boundary.
    constexpr int32_t audio = int32_t(0x80100000);
    MEM_H(0, int32_t(0x80045520)) = 2;
    MEM_H(0, int32_t(0x80045524)) = 32000;
    for (int old = 0; old < 3; ++old) {
        for (int i = 0; i < 3; ++i) {
            MEM_H(i * 2, int32_t(0x80045624)) = 0;
            MEM_W(i * 4, int32_t(0x80045618)) = 0;
        }
        MEM_W(0, int32_t(0x8004555C)) = old;
        const int slot = (old + 2) % 3;
        MEM_H(slot * 2, int32_t(0x80045624)) = 2;
        MEM_W(slot * 4, int32_t(0x80045618)) = audio;
        for (int i = 0; i < 4; ++i) MEM_H(i * 2, audio) = (i + 1) * 11;
        MEM_W(0, int32_t(0x80045550)) = 0; ctx.r4 = 65;
        rw_music_tick(rdram, &ctx); require(mixes == unsigned(old), "skip tick without AI submission");
        MEM_W(0, int32_t(0x80045550)) = 1;
        rw_music_tick(rdram, &ctx); require(mixes == unsigned(old + 1) && volume == 65, "exact queue slot and volume");
        for (int i = 0; i < 4; ++i) require(MEM_H(i * 2, audio) == (i + 1) * 111, "mixed stereo restored to RDRAM");
        MEM_W(slot * 4, int32_t(0x80045618)) = int32_t(0x807FFFFC);
        rw_music_tick(rdram, &ctx); require(mixes == unsigned(old + 1), "invalid buffer does not cross RDRAM");
    }
    std::puts("PASS: native mod lifecycle, clobbered registers, fades, pauses, fallback, effects, AI timing and stereo");
}
