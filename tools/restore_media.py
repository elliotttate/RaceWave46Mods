"""Restore manifest-pinned media from the two local release downloads."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile
import bundled_assets

ROOT = Path(__file__).resolve().parents[1]


def restore(archive, archive_name, destination, expected_hash):
    content = archive.read(archive_name)
    if hashlib.sha256(content).hexdigest() != expected_hash:
        raise ValueError(f'Checksum mismatch for {archive_name}; use matching release media')
    if not destination.resolve().is_relative_to((ROOT / 'assets').resolve()) or destination.is_symlink():
        raise ValueError(f'Unsafe destination: {destination}')
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(content)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--bundle', type=Path, help='The single RaceWave46Mods release ZIP')
    parser.add_argument('--textures', type=Path, help='Legacy separate texture RTZ')
    parser.add_argument('--music', type=Path, help='Legacy separate music ZIP')
    args = parser.parse_args()
    if args.bundle:
        import io
        with zipfile.ZipFile(args.bundle) as archive:
            texture_input = io.BytesIO(archive.read('WaveRace-HD-Textures.rtz'))
        music_input = args.bundle
    elif args.textures and args.music:
        texture_input, music_input = args.textures, args.music
    else:
        parser.error('Provide --bundle, or both --textures and --music')
    texture_root = ROOT / 'assets/textures/nano-banana-2'
    texture_manifest = json.loads((texture_root / 'manifest.json').read_text())
    with zipfile.ZipFile(texture_input) as archive:
        for entry in texture_manifest['files']:
            restore(archive, entry['path'], texture_root / entry['path'], entry['sha256'])
    music_root = ROOT / 'assets/music'
    music_manifest = json.loads((music_root / 'manifest.json').read_text())
    with zipfile.ZipFile(music_input) as archive:
        for track in music_manifest['tracks']:
            restore(archive, 'racewave_music/' + track['output'], music_root / track['output'], track['output_sha256'])
    print(json.dumps(bundled_assets.validate(ROOT / 'assets'), indent=2))


if __name__ == '__main__':
    main()
