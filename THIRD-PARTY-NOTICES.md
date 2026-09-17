# Third-party notices

## RaceWave46

The mod integration, Windows build environment helper, validators, tests and
function symbol metadata were developed in or derived from
[RaceWave46](https://github.com/DomazinUS/RaceWave46). Original contributions are
GPL-3.0-only; the complete license is retained in [COPYING](COPYING).
The tested dependency revision is `ae4aac9a475e15fbcf325c6accdd8a05b341753b`.
The function metadata records its upstream LLONSIT/Wave-Race-64 origin.

## wave-race-64-recomp

The shared mixer in `src/wr64_music.cpp` and its interface were adapted from
[wave-race-64-recomp](https://github.com/elliotttate/wave-race-64-recomp), revision
`9041cc8024f6d2015859f88f5a0307b36cd4e50a`. Its original code is MIT licensed;
the complete notice is retained in
[assets/WAVE-RACE-64-RECOMP-LICENSE.txt](assets/WAVE-RACE-64-RECOMP-LICENSE.txt).

## Build dependencies

The source build uses N64Recomp, its mod tool and native ABI header, SDL2, and
nlohmann/json from a separate RaceWave46 source checkout. It does not vendor or
relicense that checkout. N64Recomp and N64ModernRuntime retain their upstream
license terms. The N64Recomp ABI header's MIT notice is retained in
`licenses/N64Recomp-LICENSE.MIT` and included with the music DLL. SDL2 is zlib
licensed; nlohmann/json is MIT licensed. The latter's
complete notice is retained in `licenses/nlohmann-json-LICENSE.MIT` and included
with the compiled music mod. The music DLL dynamically links to the SDL2 DLL
already shipped by RaceWave46; SDL2 is not bundled here.

## Media

Music: [credits and provenance](assets/music/CREDITS.md).
Textures: [credits and provenance](assets/textures/nano-banana-2/CREDITS.md).
Checksum manifests and loop settings are retained alongside those notices.
The media and underlying compositions, artwork, logos and trademarks are
excluded from the code licenses and retain their respective owners' rights.
