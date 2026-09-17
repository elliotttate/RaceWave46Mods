# RaceWave46Mods

HD textures and a remixed soundtrack for **[RaceWave46](https://github.com/DomazinUS/RaceWave46)**, the Wave Race 64 native PC recompilation. Install either pack or both on the original **v1.0.0 Windows x64** release. **No recompilation or replacement executable is needed.**

## Downloads

**[Download RaceWave46Mods v1.0.0 — Windows x64](https://github.com/elliotttate/RaceWave46Mods/releases/latest/download/RaceWave46Mods-v1.0.0.zip)**

One ZIP includes the HD texture pack (1,806 images and 1,828 mappings), nine music recordings, the music mod, installation guide, credits and checksums. Both mods can be used independently.

[All releases](https://github.com/elliotttate/RaceWave46Mods/releases) · [Recomp repository](https://github.com/DomazinUS/RaceWave46) · [Download the recomp](https://github.com/DomazinUS/RaceWave46/releases/tag/v1.0.0) · [Asset source project](https://github.com/elliotttate/wave-race-64-recomp)

You need the recomp installed and your own supported **Wave Race 64 USA Rev 1** ROM. No ROM, save data or game executable is included here. Compatibility is tested with RaceWave46; other recompilation projects are not currently supported.

## Installation

1. Download **RaceWave46Mods-v1.0.0.zip** above and extract it to a temporary folder.
2. **Textures:** open the frontend, go to **Mods → Install Mods**, choose the extracted **WaveRace-HD-Textures.rtz**, and press **OK**. Leave this RTZ intact; it is already packaged for the importer. Enable **Wave Race HD Textures** in the Mods tab.
3. **Music:** close the game, then copy the extracted `racewave_music.dll` and `racewave_music` folder together into `.runtime/mods` beside `WaveRace64Recompiled.exe`. Create `mods` if needed. Open the game and enable **Wave Race Remixed Music**. The music DLL needs this manual step; the texture importer does not extract native music sidecars.
4. Start the game. Restart after enabling or disabling music.

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

**Already have an extracted texture pack?** Zip the contents of the folder containing `textures/`, `manifest.json` and `rt64.json`, then rename the ZIP extension to `.rtz`. Those entries must be at the archive root, without an extra enclosing folder. Install that RTZ through **Mods → Install Mods → OK**. The texture pack in this download is already prepared this way.

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
