# v1.0.0 — HD textures and remixed music

Two independent drop-in mods for the original **RaceWave46 v1.0.0 Windows x64** release. No game recompilation or replacement executable is required.

- **WaveRace-HD-Textures.rtz** — 1,806 replacement images and 1,828 RT64 mappings, with mipmaps.
- **WaveRace-Remixed-Music-Windows-x64.zip** — nine recordings, loop settings and a native music mod. Includes a volume control, original soundtrack fallback, and preservation of effects/announcer and sequence timing.
- **INSTALL-MODS.md** — installation and troubleshooting.
- **SHA256SUMS.txt** — checksums for both packs.
- **VALIDATION.md** — test scope and results.

## Install

Close the game. Copy the RTZ intact into `.runtime/mods` beside your game executable. Extract the music ZIP directly into the same `mods` folder, keeping `racewave_music.dll` next to the `racewave_music` directory. Enable the desired entries in **Settings → Mods** and start the game. Restart after toggling music.

[Full installation guide](https://github.com/elliotttate/RaceWave46Mods#installation) · [Recomp repository](https://github.com/DomazinUS/RaceWave46) · [Download RaceWave46 v1.0.0](https://github.com/DomazinUS/RaceWave46/releases/tag/v1.0.0)

Tested with the unchanged original release executable: enabled playback, zero volume, missing-track fallback and disabled mods. Seven standalone automated mixer/bridge tests also pass. Full-course visual review and listening across every track are not covered. This release does not include the separate ray-tracing startup fix.

Music by **Bryan EL** and **Retro Game Remix**; texture direction by **Brian Tate** and credited contributors. Full credits and existing media rights notices are included. No game ROM, executable, saves or account data is included. The GitHub-generated source archives contain source and metadata; use the two named mod downloads to install.
