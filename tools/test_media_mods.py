"""Smoke-test the built packs on an unchanged Windows release in an isolated profile."""
import argparse
import array
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile
import wave
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def audio_stats(path):
    with wave.open(str(path)) as wav:
        assert wav.getnchannels() == 4
        samples = array.array('h', wav.readframes(wav.getnframes()))
        changed = sum(samples[i] != samples[i + 2] or samples[i + 1] != samples[i + 3]
                      for i in range(0, len(samples), 4))
        return dict(frames=wav.getnframes(), rate=wav.getframerate(), changed_frames=changed)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exe', type=Path, required=True)
    parser.add_argument('--rom', type=Path, required=True)
    parser.add_argument('--scenarios', nargs='+', choices=('enabled', 'zero-volume', 'missing-track', 'disabled'),
                        default=['enabled', 'zero-volume', 'missing-track', 'disabled'])
    args = parser.parse_args()
    exe, rom = args.exe.resolve(), args.rom.resolve()
    expected = json.loads((exe.parent / 'package-manifest.json').read_text())['executable_sha256']
    before = hashlib.sha256(exe.read_bytes()).hexdigest()
    assert before == expected, 'Executable differs from published package manifest'
    root = ROOT / 'build/mods'
    root.mkdir(parents=True, exist_ok=True)
    profile = Path(tempfile.mkdtemp(prefix='validation-', dir=root))
    mods = profile / 'mods'
    mods.mkdir()
    with zipfile.ZipFile(ROOT / 'dist/media-mods/RaceWave46Mods-v1.0.0.zip') as archive:
        archive.extractall(mods)
    config = profile / 'mod_config'
    config.mkdir()
    results = dict(executable_sha256=before, profile=str(profile), scenarios={})
    for scenario in args.scenarios:
        enabled = [] if scenario == 'disabled' else ['racewave_hd_textures', 'racewave_remixed_music']
        (profile / 'mods.json').write_text(json.dumps(dict(enabled_mods=enabled,
                        mod_order=['racewave_hd_textures', 'racewave_remixed_music'], latest_game_mode='')))
        (config / 'racewave_remixed_music.json').write_text(json.dumps(dict(
            mod_id='racewave_remixed_music', mod_version='1.0.0', recomp_version='1.0.0',
            storage={'volume': 0 if scenario == 'zero-volume' else 65})))
        track = mods / 'racewave_music/main_theme.wav'
        backup = track.with_suffix('.wav.disabled')
        if scenario == 'missing-track':
            track.rename(backup)
        try:
            import os
            env = dict(os.environ, WR64_AUDIO_CAPTURE=str(profile / scenario))
            startup = subprocess.STARTUPINFO()
            startup.dwFlags |= subprocess.STARTF_USESHOWWINDOW
            startup.wShowWindow = 0
            proc = subprocess.run([str(exe), '--launch', '--windowed', '--seconds', '16',
                        '--runtime-dir', str(profile), '--rom', str(rom)], cwd=exe.parent,
                        env=env, startupinfo=startup, capture_output=True, timeout=55)
            stderr = proc.stderr.decode('utf-8', errors='replace')
            stdout = proc.stdout.decode('utf-8', errors='replace')
            (profile / f'{scenario}.stderr.log').write_text(stderr)
            (profile / f'{scenario}.stdout.log').write_text(stdout)
            assert proc.returncode == 0, (scenario, proc.returncode, stderr, stdout)
            assert 'Error opening mod' not in stdout, stdout
            assert 'Failed' not in stderr, stderr
            if enabled:
                assert '[textures] Loaded 1 enabled texture pack(s).' in stderr, stderr
                assert '[music-mod] Mixing into original AI buffer at 32000 Hz.' in stderr, stderr
                stats = audio_stats(profile / f'{scenario}-0.wav')
                if scenario == 'enabled':
                    assert 'started sequence 3 (replacement)' in stderr, stderr
                    assert stats['changed_frames'] > stats['frames'] // 2, stats
                else:
                    assert stats['changed_frames'] == 0, (scenario, stats)
                    if scenario == 'missing-track':
                        assert 'started sequence 3 (original)' in stderr, stderr
            else:
                assert '[music-mod]' not in stderr and '[music]' not in stderr, stderr
                assert 'Loaded 1 enabled texture pack' not in stderr, stderr
                stats = {'mods_loaded': False}
            results['scenarios'][scenario] = dict(exit_code=proc.returncode, **stats)
            print(f'PASS: {scenario}: {stats}', flush=True)
        finally:
            if backup.exists():
                backup.rename(track)
    assert hashlib.sha256(exe.read_bytes()).hexdigest() == before
    (root / 'validation.json').write_text(json.dumps(results, indent=2) + '\n')
    print(f'Results: {root / "validation.json"}', flush=True)


if __name__ == '__main__':
    main()
