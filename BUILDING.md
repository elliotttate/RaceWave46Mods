# Building the mods

Playing only requires the [release downloads](https://github.com/elliotttate/RaceWave46Mods/releases/latest). The steps below are for source development and do not compile the game executable.

## Requirements

- Windows x64, Visual Studio C++ Build Tools, Windows SDK, CMake and Ninja.
- Python 3.11 or newer.
- Clang with MIPS support and `ld.lld`. Visual Studio's bundled Clang may omit MIPS; use a full [LLVM distribution](https://github.com/llvm/llvm-project/releases), or pass `-UseWslCompiler` to use WSL's Clang with Windows lld.
- The RaceWave46 source checkout for its vendored build tools, SDL2, JSON library and mod ABI headers.

## Prepare dependencies

```powershell
git clone https://github.com/elliotttate/RaceWave46Mods.git
cd RaceWave46Mods
git clone https://github.com/DomazinUS/RaceWave46.git dependencies/RaceWave46
git -C dependencies/RaceWave46 checkout ae4aac9a475e15fbcf325c6accdd8a05b341753b
```

You may instead pass `-RecompSource C:\path\to\RaceWave46` to use an existing checkout. The pinned revision above is the tested SDK baseline. Its vendored dependencies are already included in that repository.

## Restore media

Download the single mod bundle from this repository's release, then run:

```powershell
python tools/restore_media.py --bundle C:\Downloads\RaceWave46Mods-v1.0.0.zip
```

This restores only the manifest-listed media and verifies its hashes. Media stays excluded from Git. Existing source manifests and credits are preserved.

## Build and test

```powershell
.\BUILD-MODS.ps1 -LlvmBin C:\LLVM\bin
# Or use WSL's MIPS-capable Clang and Visual Studio's Windows linker:
.\BUILD-MODS.ps1 -UseWslCompiler
```

The script builds the MIPS hooks, RecompModTool and native music DLL, runs seven automated mixer/bridge tests, validates the media, and writes the single release download `dist/media-mods/RaceWave46Mods-v1.0.0.zip`. Separate packs are retained there as build intermediates. The standalone test harness creates its own synthetic audio, so `-SkipPackaging` builds and tests without requiring media downloads. `-Jobs` controls build concurrency.

To test against the original release with your own USA Rev 1 ROM:

```powershell
python tools/test_media_mods.py --exe C:\RaceWave46\WaveRace64Recompiled.exe --rom C:\ROMs\WaveRace64.z64
```

This creates an isolated profile under `build/mods`, compares the executable hash with its package manifest, and checks enabled playback, zero volume, missing-recording fallback and disabled mods. It leaves the original installation's saves/settings alone. No ROM is required to compile the mods or run the seven synthetic tests.
