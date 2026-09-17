# Optional media mods for RaceWave46

These packs target the original **RaceWave46 v1.0.0 Windows x64** release and its
USA Rev 1 game. Installation does not require recompiling or replacing the EXE.
The texture pack and music mod can be installed and enabled independently.

Get the [mod downloads](https://github.com/elliotttate/RaceWave46Mods/releases/latest).
The target recomp is [RaceWave46](https://github.com/DomazinUS/RaceWave46);
download its [original v1.0.0 release](https://github.com/DomazinUS/RaceWave46/releases/tag/v1.0.0).

## Install

1. Download **RaceWave46Mods-v1.0.0.zip** and extract it to a temporary folder.
2. Open the frontend, choose **Mods > Install Mods**, select the extracted
   **WaveRace-HD-Textures.rtz**, and press **OK**. Do not extract the RTZ itself.
3. Close the game. Copy `racewave_music.dll` and the `racewave_music` directory
   together into `.runtime/mods` beside `WaveRace64Recompiled.exe`, creating `mods`
   if needed. The latter is a standard directory mod containing its manifest,
   hooks and recordings. The UI's texture importer does not install native music
   sidecars. A custom `--runtime-dir` uses that directory's `mods` instead.
4. Open **Settings > Mods**, enable **Wave Race HD Textures** and/or **Wave Race
   Remixed Music**, then start the game. Restart after toggling the music mod.

For other extracted texture packs, zip the contents of the folder containing
`textures/`, `manifest.json` and `rt64.json`, rename `.zip` to `.rtz`, then use
**Mods > Install Mods > OK**. Keep those entries at the ZIP root with no extra
enclosing folder. The supplied RTZ is already ready to import.

Expected layout:

```text
WaveRace64Recompiled.exe
.runtime/
  mods/
    WaveRace-HD-Textures.rtz
    racewave_music.dll
    racewave_music/
      mod.json
      mod_binary.bin
      mod_syms.bin
      loops.json
      main_theme.wav
      ... eight other WAVs and credits ...
```

The music mod has a **Replacement music volume** control in its mod settings
(default 65%). The game's **Main Volume** controls music and effects together.
Missing or invalid recordings fall back to their original sequences. Engines,
water effects and announcer remain native. Sequence scripts, cue restarts, fades,
pauses and loop points are preserved. The recordings are mixed through the
original game's audio buffer and sample rate, then its normal output resampler.
Courses without a matching remix use the supplied Seafoam Shoreline recording.

Disable either entry to restore its original media. To uninstall, close the game
and remove only the corresponding files listed above. No saves or ROM changes
are needed. These mods do not include the separate ray-tracing startup fix.

This is intended for the original release. On a build that already bundles these
same assets, choose **Original** for its built-in music/textures before enabling
the mods to avoid playing two replacement soundtracks.

## Build the mods from source

Run `BUILD-MODS.ps1` with Visual Studio C++ tools, CMake, Ninja, Python 3.11+, and
a full LLVM build that supports MIPS. Pass `-LlvmBin` to select LLVM. Visual
Studio's bundled Clang may omit MIPS; `-UseWslCompiler` uses WSL's Clang for the
small MIPS bridge while retaining Windows lld and MSVC for linking/native code.
The script builds only mod tools and the music DLL, never the game executable.
Existing validated media under `assets/music` and `assets/textures/nano-banana-2`
are required. Outputs are written to `dist/media-mods` with SHA-256 checksums.

See the standalone repository's
[build guide](https://github.com/elliotttate/RaceWave46Mods/blob/main/BUILDING.md)
for the pinned RaceWave46 SDK checkout and the media restore command. The script
runs seven synthetic mixer/bridge tests; use `-SkipPackaging` to build and test
without media downloads.

The native DLL uses the runtime's version-1 mod ABI and standard entry/return
hooks on four audio functions. It resolves recordings relative to its own file,
independent of the working directory. The texture pack uses the original RT64
hash database and original mipmaps.

## Credits and licenses

Recordings: **Bryan EL** and **Retro Game Remix**; see the included music
`CREDITS.md`. Texture direction: **Brian Tate** and the contributors credited
inside the RTZ. Assets came from
[wave-race-64-recomp](https://github.com/elliotttate/wave-race-64-recomp).
Media retains its existing rights and is not licensed under the code licenses.
Keep the supplied credits; these packages do not grant redistribution rights.
The new integration code follows this project's GPL-3.0-only license; the reused
music mixer retains the source project's MIT notice. No ROM is included.
