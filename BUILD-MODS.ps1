[CmdletBinding()]
param([string]$BuildToolsRoot = '', [string]$LlvmBin = '', [switch]$UseWslCompiler,
    [string]$RecompSource = '', [switch]$SkipPackaging, [ValidateRange(1,64)][int]$Jobs = 8)
$ErrorActionPreference = 'Stop'
if (-not $RecompSource) { $RecompSource = Join-Path $PSScriptRoot 'dependencies/RaceWave46' }
$RecompSource = (Resolve-Path -LiteralPath $RecompSource -ErrorAction Stop).Path
if (!(Test-Path -LiteralPath (Join-Path $RecompSource 'lib/N64ModernRuntime/N64Recomp/CMakeLists.txt'))) {
    throw 'Pass -RecompSource pointing to a RaceWave46 source checkout. See BUILDING.md.'
}
. (Join-Path $PSScriptRoot 'tools/windows-build-environment.ps1')
$BuildToolsRoot = Initialize-Wr64BuildEnvironment -BuildToolsRoot $BuildToolsRoot
$Python = Resolve-Wr64Python
if (-not $LlvmBin) { $LlvmBin = Join-Path $BuildToolsRoot 'VC/Tools/Llvm/x64/bin' }
$clang = Join-Path $LlvmBin 'clang.exe'
$linker = Join-Path $LlvmBin 'ld.lld.exe'
if (!(Test-Path -LiteralPath $clang) -or !(Test-Path -LiteralPath $linker)) { throw 'Install LLVM (including MIPS target and lld) or pass -LlvmBin.' }
$output = Join-Path $PSScriptRoot 'build/mods/music'
New-Item -ItemType Directory -Force $output | Out-Null
$source = Join-Path $PSScriptRoot 'mods/replacement_music'
$compileFlags = @('--target=mips-unknown-elf', '-march=mips3', '-mabi=32', '-mno-abicalls', '-fno-pic', '-G0', '-O0', '-fno-stack-protector', '-fno-builtin', '-ffreestanding')
if ($UseWslCompiler) {
    $wslSource = & wsl.exe --exec wslpath -a (Join-Path $source 'hooks.c')
    $wslObject = & wsl.exe --exec wslpath -a (Join-Path $output 'hooks.o')
    & wsl.exe --exec clang @compileFlags -c $wslSource -o $wslObject
} else {
    & $clang @compileFlags -c (Join-Path $source 'hooks.c') -o (Join-Path $output 'hooks.o')
}
if ($LASTEXITCODE -ne 0) { throw 'MIPS hook compilation failed.' }
& $linker -m elf32btsmip --emit-relocs -T (Join-Path $source 'mod.ld') -e 0 (Join-Path $output 'hooks.o') -o (Join-Path $output 'hooks.elf')
if ($LASTEXITCODE -ne 0) { throw 'MIPS hook linking failed.' }
# Build packaging tools independently; the game executable is not a target.
$toolBuild = Join-Path $PSScriptRoot 'build/mods/tools'
& cmake -S (Join-Path $RecompSource 'lib/N64ModernRuntime/N64Recomp') -B $toolBuild -G Ninja -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { throw 'Mod tool configuration failed.' }
& cmake --build $toolBuild --target RecompModTool --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw 'Mod tool build failed.' }
& (Join-Path $toolBuild 'RecompModTool.exe') (Join-Path $source 'mod.toml') $output
if ($LASTEXITCODE -ne 0) { throw 'Mod packaging failed.' }
& cmake -S $source -B (Join-Path $output 'native') -G Ninja -DCMAKE_BUILD_TYPE=Release "-DRECOMP_SDK_ROOT=$RecompSource"
if ($LASTEXITCODE -ne 0) { throw 'Music DLL configuration failed.' }
& cmake --build (Join-Path $output 'native') --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw 'Music DLL build failed.' }
$testBuild = Join-Path $PSScriptRoot 'build/tests'
& cmake -S (Join-Path $PSScriptRoot 'tests') -B $testBuild -G Ninja -DCMAKE_BUILD_TYPE=Release "-DRECOMP_SDK_ROOT=$RecompSource"
if ($LASTEXITCODE -ne 0) { throw 'Tests configuration failed.' }
& cmake --build $testBuild --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw 'Tests build failed.' }
& ctest --test-dir $testBuild --output-on-failure
if ($LASTEXITCODE -ne 0) { throw 'Mod tests failed.' }
if (-not $SkipPackaging) {
    & $Python (Join-Path $PSScriptRoot 'tools/package_media_mods.py')
    if ($LASTEXITCODE -ne 0) { throw 'Media packaging failed.' }
}
