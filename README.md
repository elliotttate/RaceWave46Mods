# RaceWave46Mods

HD textures and a remixed soundtrack for **[RaceWave46](https://github.com/DomazinUS/RaceWave46)**, the Wave Race 64 native PC recompilation. Install either pack or both on the original **v1.0.0 Windows x64** release. **No recompilation or replacement executable is needed.**

## Downloads

| Download | Includes |
| --- | --- |
| **[HD texture pack](https://github.com/elliotttate/RaceWave46Mods/releases/latest/download/WaveRace-HD-Textures.rtz)** | 1,806 replacement images, 1,828 texture mappings and mipmaps |
| **[Remixed music — Windows x64](https://github.com/elliotttate/RaceWave46Mods/releases/latest/download/WaveRace-Remixed-Music-Windows-x64.zip)** | Nine recordings, loop settings, native audio mod and credits |
| [SHA-256 checksums](https://github.com/elliotttate/RaceWave46Mods/releases/latest/download/SHA256SUMS.txt) | Verify the downloaded packs |

[All releases](https://github.com/elliotttate/RaceWave46Mods/releases) · [Recomp repository](https://github.com/DomazinUS/RaceWave46) · [Download the recomp](https://github.com/DomazinUS/RaceWave46/releases/tag/v1.0.0) · [Asset source project](https://github.com/elliotttate/wave-race-64-recomp)

You need the recomp installed and your own supported **Wave Race 64 USA Rev 1** ROM. No ROM, save data or game executable is included here. Compatibility is tested with RaceWave46; other recompilation projects are not currently supported.

## Installation

1. Close the game. Open its `.runtime/mods` folder beside `WaveRace64Recompiled.exe`. Create `mods` if needed.
2. Copy **WaveRace-HD-Textures.rtz** into `mods`. **Leave the RTZ intact.** You can also import it through **Settings → Mods**.
3. Extract **WaveRace-Remixed-Music-Windows-x64.zip** directly into `mods`. Keep `racewave_music.dll` next to the `racewave_music` folder. **Do not add an extra enclosing folder.** The music ZIP must be extracted; importing only an individual file through the game UI will not install the full music mod.
4. Open the game and go to **Settings → Mods**. Enable **Wave Race HD Textures** and/or **Wave Race Remixed Music**, then start playing. Restart the game after enabling or disabling music.

```text
Your RaceWave46 installation/
├── WaveRace64Recompiled.exe
└── .runtime/
    └── mods/
        ├── WaveRace-HD-Textures.rtz
        ├── racewave_music.dll
        └── racewave_music/
            ├── mod.json
            ├── mod_binary.bin
            ├── mod_syms.bin
            ├── loops.json
            ├── main_theme.wav
            └── ... eight other WAVs and credits
```

If you use `--runtime-dir`, install into that directory's `mods` folder instead.

## Music controls

The music mod's settings include **Replacement music volume**, initially **65%**. The game's **Main Volume** controls the final music and effects mix. Engines, water effects and the announcer remain native, and the mod retains the game's sequence scripts, fades, pauses and cue restarts. Missing or invalid recordings fall back to the original soundtrack.

The pack includes Main Theme, Options, Dolphin Park, Sunny Beach, Sunset Bay, Twilight City, Glacier Coast, Victory and Seafoam Shoreline. Courses without a matching remix use Seafoam Shoreline. Recordings are mixed at the game's native audio rate before its normal output resampler.

## Disable, uninstall and troubleshooting

- Disable either mod in **Settings → Mods** to restore the corresponding original media. Restart after changing the music mod's enabled state.
- To uninstall, close the game and remove the RTZ and/or `racewave_music.dll` plus the `racewave_music` folder. Keep your ROM and saves.
- If music fails to load, check that the DLL and music folder are siblings, and that your ZIP extraction completed.
- On a newer build that already bundles these same assets, select **Original** for the built-in music and textures before enabling these mods, to avoid duplicate replacement music.
- These packs do not include the separate ray-tracing startup fix or change the original release's rendering shaders.

## Source and verification

This repository contains the mod source, metadata, build scripts and tests. Large media files are hosted as release assets rather than in Git history. See **[BUILDING.md](BUILDING.md)** to restore the media and rebuild the mods without compiling the game, and **[validation results](mods/VALIDATION.md)** for the checks performed. Title-screen smoke tests and automated audio tests passed; full-course visual review and listening across every track remain outside that validation.

## Credits

- **[RaceWave46](https://github.com/DomazinUS/RaceWave46)** and its contributors — the target recompilation and mod loader integration.
- **[wave-race-64-recomp](https://github.com/elliotttate/wave-race-64-recomp)** — source of the supplied textures, recordings and music mixer.
- **Bryan EL** and **Retro Game Remix** — recordings; see [full music credits](assets/music/CREDITS.md), including the [Dolphin Park album](https://retrogameremix.bandcamp.com/album/dolphin-park).
- **Brian Tate** and the texture contributors — visual direction and artwork preparation; see [texture credits](assets/textures/nano-banana-2/CREDITS.md).
- **[N64Recomp](https://github.com/N64Recomp/N64Recomp)**, **[N64ModernRuntime](https://github.com/N64Recomp/N64ModernRuntime)** and **[RT64](https://github.com/rt64/rt64)** — the underlying mod and rendering technology.

Integration code is GPL-3.0-only; reused components retain their notices. Music, artwork, underlying compositions and trademarks retain their respective owners' rights and are excluded from the code licenses. This project is not affiliated with Nintendo. See [LICENSE](LICENSE) and [third-party notices](THIRD-PARTY-NOTICES.md).
