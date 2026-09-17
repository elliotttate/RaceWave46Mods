# Media mod validation — 2026-09-17

Tested against the downloaded, unchanged [RaceWave46 v1.0.0 Windows x64 release](https://github.com/DomazinUS/RaceWave46/releases/tag/v1.0.0).
The executable matched its published package manifest before and after testing:

```text
a5b00cadb0a390854603456f4c8015fb379a349d878e918291e28065b1edc3d1
```

Four separate 16-second title-screen runs used an isolated profile and the user's
existing USA Rev 1 ROM. All exited successfully, with no mod-opening errors:

| Configuration | Observed result |
| --- | --- |
| Both mods enabled, music 65% | One texture pack loaded; all nine recordings loaded; title sequence used its replacement. Audio capture showed replacement samples in 468,727 of 504,816 stereo frames. |
| Both enabled, music 0% | Mod volume reached the DLL; mixed samples exactly matched the pre-mix buffer. |
| Title recording temporarily missing | Title sequence selected original music; no replacement samples were added. |
| Both mods disabled | No audio DLL hooks or replacement playback; textures disabled. |

Audio capture was taken immediately before and after the mod's mixer at the
game's native 32 kHz rate, upstream of the original output resampler and master
volume. The normal distribution does not record audio unless the optional
`WR64_AUDIO_CAPTURE` diagnostic environment variable is set.

Eight automated tests passed: seven existing mixer/native-hook tests and the
new mod bridge test. These cover missing/invalid tracks, mixing, asset overrides,
cue restarts, nested disable calls, clobbered return-hook registers, native gain
restoration, pauses, effects isolation, all three audio buffer indices, skipped
audio ticks, stereo memory ordering and invalid buffer bounds.

Asset validation passed for 1,806 images, 1,828 texture mappings, nine recordings
and their source hashes/loop settings. Both output archives passed ZIP integrity
checks. `dist/media-mods/SHA256SUMS.txt` identifies the packaged files.

This verifies installation, boot/title playback, audio fallback and enable/disable
behavior. Full-course visual review and gameplay listening across every track
were not performed. The mods do not change the release's rendering shaders or
include the separate ray-tracing startup fix.

The standalone repository retains six mixer tests and the native mod bridge test;
its build script runs all seven. The additional bundled-host wrapper test above
was run in the original development checkout. Reproduce the standalone checks
with `BUILD-MODS.ps1` (see `BUILDING.md`) and
`python tools/test_media_mods.py --exe <original-release-exe> --rom <your-rom>`.
