"""Package locally supplied media as original-release mods; never bundle a ROM."""
from pathlib import Path
import hashlib
import json
import shutil
import zipfile
import bundled_assets

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "dist/media-mods"
TEXTURE_ID = "racewave_hd_textures"


def main():
    report = bundled_assets.validate(ROOT / "assets")
    OUT.mkdir(parents=True, exist_ok=True)
    manifest = dict(id=TEXTURE_ID, game_id="waverace64", version="1.0.0",
                    minimum_recomp_version="1.0.0", display_name="Wave Race HD Textures",
                    authors=["Brian Tate", "Wave Race 64 recomp texture contributors"],
                    short_description="1,806 replacement images with original layout and mipmaps.",
                    description="Nano Banana 2 texture collection from wave-race-64-recomp. See CREDITS.md for source artwork and media rights.")
    texture = OUT / "WaveRace-HD-Textures.rtz"
    pack = ROOT / "assets/textures/nano-banana-2"
    # Stored entries support RT64 streaming without decompressing entire images.
    with zipfile.ZipFile(texture, "w", compression=zipfile.ZIP_STORED) as archive:
        archive.writestr("mod.json", json.dumps(manifest, indent=2))
        for file in sorted(pack.rglob("*")):
            if file.is_file():
                archive.write(file, file.relative_to(pack).as_posix())
    music = OUT / "WaveRace-Remixed-Music-Windows-x64.zip"
    source = ROOT / "build/mods/music"
    with zipfile.ZipFile(music, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=6) as archive:
        # A directory mod keeps its media inside the mod's own root. The original
        # loader treats every immediate subdirectory as a mod, so a separate
        # unmanifested media directory would create a spurious installation error.
        with zipfile.ZipFile(source / "WaveRace-Remixed-Music.nrm") as code:
            for name in ("mod.json", "mod_binary.bin", "mod_syms.bin"):
                archive.writestr("racewave_music/" + name, code.read(name))
        archive.write(source / "native/racewave_music.dll", "racewave_music.dll")
        for file in sorted((ROOT / "assets/music").iterdir()):
            if file.is_file():
                archive.write(file, "racewave_music/" + file.name)
        archive.write(ROOT / "mods/README.md", "INSTALL-MODS.md")
        archive.write(ROOT / "assets/WAVE-RACE-64-RECOMP-LICENSE.txt", "racewave_music/SOURCE-LICENSE.txt")
        archive.write(ROOT / "COPYING", "racewave_music/CODE-LICENSE-GPL-3.0.txt")
        archive.write(ROOT / "licenses/nlohmann-json-LICENSE.MIT", "racewave_music/JSON-LICENSE.MIT")
        archive.write(ROOT / "licenses/N64Recomp-LICENSE.MIT", "racewave_music/N64Recomp-LICENSE.MIT")
        archive.write(ROOT / "THIRD-PARTY-NOTICES.md", "racewave_music/THIRD-PARTY-NOTICES.md")
    shutil.copy2(ROOT / "mods/README.md", OUT / "INSTALL-MODS.md")
    shutil.copy2(ROOT / "mods/VALIDATION.md", OUT / "VALIDATION.md")
    hashes = []
    for file in (texture, music):
        with zipfile.ZipFile(file) as archive:
            if archive.testzip() is not None:
                raise ValueError(f"Corrupt archive: {file}")
        with file.open('rb') as stream:
            hashes.append(f"{hashlib.file_digest(stream, 'sha256').hexdigest()}  {file.name}")
    (OUT / "SHA256SUMS.txt").write_text("\n".join(hashes) + "\n")
    # One user-facing download: an intact RTZ for the frontend importer plus
    # the directory music mod and its native sidecar. Avoid a nested music ZIP.
    bundle = OUT / "RaceWave46Mods-v1.0.0.zip"
    with zipfile.ZipFile(bundle, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=6) as archive:
        archive.write(texture, texture.name)
        with zipfile.ZipFile(music) as music_archive:
            for entry in music_archive.infolist():
                archive.writestr(entry.filename, music_archive.read(entry))
        archive.write(ROOT / "mods/VALIDATION.md", "VALIDATION.md")
        # Checksums refer directly to installed payload files, not a nested ZIP.
        payload_hashes = [hashes[0]]
        with zipfile.ZipFile(music) as music_archive:
            for entry in music_archive.infolist():
                payload_hashes.append(f"{hashlib.sha256(music_archive.read(entry)).hexdigest()}  {entry.filename}")
        archive.writestr("SHA256SUMS.txt", "\n".join(payload_hashes) + "\n")
    with zipfile.ZipFile(bundle) as archive:
        if archive.testzip() is not None:
            raise ValueError(f"Corrupt archive: {bundle}")
    with bundle.open('rb') as stream:
        bundle_hash = hashlib.file_digest(stream, 'sha256').hexdigest()
    (OUT / 'BUNDLE-SHA256.txt').write_text(f'{bundle_hash}  {bundle.name}\n')
    print(json.dumps(report, indent=2))
    for file in (texture, music, bundle):
        print(f"{file}: {file.stat().st_size:,} bytes")


if __name__ == "__main__":
    main()
