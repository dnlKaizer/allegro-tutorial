# Script de compilação em PowerShell para os exemplos do Allegro Vivace
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path

if (-not (Test-Path "bin")) {
    New-Item -ItemType Directory -Force "bin" | Out-Null
}

$flags = @(
    "-Wall",
    "-O2",
    "-lallegro_font",
    "-lallegro_primitives",
    "-lallegro_acodec",
    "-lallegro_audio",
    "-lallegro_image",
    "-lallegro"
)

$sources = @(
    "src/01_basic_structure.c",
    "src/02_graphics.c",
    "src/03_input_keyboard.c",
    "src/03_input_mouse.c",
    "src/04_sound.c",
    "src/05_gameplay.c"
)

Write-Host "Compilando capitulos do tutorial Allegro Vivace..." -ForegroundColor Cyan

foreach ($src in $sources) {
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($src)
    $out = "bin/$baseName.exe"
    Write-Host "-> Compilando $src para $out..." -NoNewline
    & gcc $src -o $out @flags
    if ($LASTEXITCODE -eq 0) {
        Write-Host " [OK]" -ForegroundColor Green
    } else {
        Write-Host " [ERRO]" -ForegroundColor Red
    }
}

# Copia os assets para bin para execução sem erro de arquivo não encontrado
Copy-Item assets/* bin/ -Force

# Garante que todas as DLLs necessárias do Allegro e dependências estejam em bin/
$ucrtBin = "C:\msys64\ucrt64\bin"
Get-ChildItem $ucrtBin -Filter "allegro*.dll" | Copy-Item -Destination "bin/" -Force
$extraDlls = @("libdumb.dll", "libFLAC.dll", "libgcc_s_seh-1.dll", "libogg-0.dll", "libopenal-1.dll", "libopus-0.dll", "libopusfile-0.dll", "libsharpyuv-0.dll", "libstdc++-6.dll", "libvorbis-0.dll", "libvorbisfile-3.dll", "libwebp-7.dll", "libwinpthread-1.dll")
foreach ($d in $extraDlls) {
    $src = Join-Path $ucrtBin $d
    if (Test-Path $src) { Copy-Item $src "bin/" -Force }
}

Write-Host "Concluido! Todos os executaveis e DLLs estao na pasta bin/" -ForegroundColor Green
