# v1.0.0 — HD textures and remixed music

Two independent drop-in mods for the original **RaceWave46 v1.0.0 Windows x64** release. No game recompilation or replacement executable is required.

**One download: RaceWave46Mods-v1.0.0.zip.** It contains:

- **WaveRace-HD-Textures.rtz** — 1,806 replacement images and 1,828 RT64 mappings, with mipmaps.
- **Music mod** — nine recordings, loop settings, native DLL and directory mod. Includes a volume control, original soundtrack fallback, and preservation of effects/announcer and sequence timing.
- **INSTALL-MODS.md**, **SHA256SUMS.txt**, **VALIDATION.md**, and credits.

## Install

Extract the download. For textures, open the frontend and choose **Mods → Install Mods**, select **WaveRace-HD-Textures.rtz**, and press **OK**. This RTZ is already packaged correctly; do not extract it.

For music, close the game and copy `racewave_music.dll` plus the `racewave_music` folder together into `.runtime/mods` beside your game executable. Enable the desired entries in **Settings → Mods** and start the game. Restart after toggling music. The music sidecars require this separate copy step because the RTZ importer handles textures.

If you have another extracted texture pack, zip the contents of its folder containing `textures/`, `manifest.json` and `rt64.json`, rename `.zip` to `.rtz`, and install through **Mods → Install Mods → OK**. Keep those entries at the archive root, without an extra folder.

[Full installation guide](https://github.com/elliotttate/RaceWave46Mods#installation) · [Recomp repository](https://github.com/DomazinUS/RaceWave46) · [Download RaceWave46 v1.0.0](https://github.com/DomazinUS/RaceWave46/releases/tag/v1.0.0)

Tested with the unchanged original release executable: enabled playback, zero volume, missing-track fallback and disabled mods. Seven standalone automated mixer/bridge tests also pass. Full-course visual review and listening across every track are not covered. This release does not include the separate ray-tracing startup fix.

Music by **Bryan EL** and **Retro Game Remix**; texture direction by **Brian Tate** and credited contributors. Full credits and existing media rights notices are included. No game ROM, executable, saves or account data is included. The GitHub-generated source archives contain source and metadata; use **RaceWave46Mods-v1.0.0.zip** to install.
