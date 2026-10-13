# Allegro Vivace - Tutorial Completo em C (Allegro 5)

Este repositório contém a implementação passo a passo de todos os capítulos do tutorial clássico e oficial **Allegro Vivace** ([Wiki Oficial](https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace)).

---

## 📁 Estrutura de Arquivos

- **`assets/`**: Imagens e arquivos de áudio necessários para os capítulos.
  - `mysha.png`: Mascote do Allegro para demonstração gráfica.
  - `spritesheet.png`: Sprites dos jogadores, alienígenas, tiros e explosões.
  - `elephant.wav`: Efeito sonoro do capítulo de áudio.
  - `shot.flac`, `explode1.flac`, `explode2.flac`: Sons do jogo de nave.
  - `music.opus`: Trilha sonora para o teste de streaming de áudio.
- **`src/`**: Códigos-fonte em C comentados didaticamente:
  - `01_basic_structure.c`: Capítulo 1 – Estrutura Básica do Jogo e Game Loop.
  - `02_graphics.c`: Capítulo 2 – Gráficos, Primitivas, Bitmaps e Animação.
  - `03_input_keyboard.c`: Capítulo 3A – Entrada por Teclado e Bitmask.
  - `03_input_mouse.c`: Capítulo 3B – Entrada por Mouse e Física de Inércia.
  - `04_sound.c`: Capítulo 4 – Áudio, Efeitos Sonoros e Streaming de Música.
  - `05_gameplay.c`: Capítulo 5 – O Jogo Completo (Space Shooter retro estilo arcade).
- **`bin/`**: Binários compilados prontos para execução.
- **`build.ps1`**: Script PowerShell para compilar todos os exemplos de uma vez.
- **`Makefile`**: Arquivo make padrão para ambientes Unix/MinGW.

---

## 🚀 Como Compilar e Executar

### 1. Compilação Automática (PowerShell)
Execute no terminal:
```powershell
.\build.ps1
```

### 2. Compilação Manual via GCC (MinGW / MSYS2 UCRT64)
Cada programa pode ser compilado individualmente com:
```powershell
gcc src/01_basic_structure.c -o bin/01_basic_structure.exe -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
gcc src/02_graphics.c -o bin/02_graphics.exe -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
gcc src/03_input_keyboard.c -o bin/03_input_keyboard.exe -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
gcc src/03_input_mouse.c -o bin/03_input_mouse.exe -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
gcc src/04_sound.c -o bin/04_sound.exe -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
gcc src/05_gameplay.c -o bin/05_gameplay.exe -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
```

### 3. Como Executar
```powershell
.\bin\01_basic_structure.exe
.\bin\02_graphics.exe
.\bin\03_input_keyboard.exe
.\bin\03_input_mouse.exe
.\bin\04_sound.exe
.\bin\05_gameplay.exe
```

---

## 🎮 Controles do Jogo Final (`05_gameplay.exe`)
- **Setas direcionais**: Movem a espaçonave.
- **Espaço**: Dispara rajadas de tiros contra as frotas alienígenas.
- **ESC**: Encerra a partida.
