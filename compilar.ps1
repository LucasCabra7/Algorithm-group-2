# Script de compilacao - Zombie Rampage
# Uso: .\compilar.ps1

$ErrorActionPreference = "Stop"

Write-Host "=== Compilando Zombie Rampage ===" -ForegroundColor Green
Write-Host ""

# Verifica se gcc esta disponivel
try {
    gcc --version 2>&1 | Out-Null
} catch {
    Write-Host "ERRO: gcc nao encontrado" -ForegroundColor Red
    exit 1
}

# Criar pastas se nao existirem
if (!(Test-Path "obj")) {
    New-Item -ItemType Directory -Path "obj" | Out-Null
    Write-Host "Pasta obj criada" -ForegroundColor Green
}

if (!(Test-Path "bin")) {
    New-Item -ItemType Directory -Path "bin" | Out-Null
    Write-Host "Pasta bin criada" -ForegroundColor Green
}

Write-Host ""
Write-Host "Compilando objetos..." -ForegroundColor Yellow

# Arquivos para compilar (inclui AVL e Heap)
$arquivos = @(
    "Arquivos.c",
    "Batalha.c",
    "GameState.c",
    "Grafo.c",
    "Inventario.c",
    "Mapa.c",
    "Personagem.c",
    "AVLInventario.c",
    "HeapMinimo.c",
    "main.c",
    "main_gui.c"
)

foreach ($f in $arquivos) {
    Write-Host "  - $f" -NoNewline
    gcc -Wall -std=c99 -g -Iinclude -I"C:\raylib\include" -c "src/$f" -o "obj/$($f -replace '.c', '.o')" 2>&1 | Out-Null
    Write-Host " OK" -ForegroundColor Green
}

Write-Host ""
Write-Host "Linkando executaveis..." -ForegroundColor Yellow

# Versao terminal
gcc -o bin/zombie.exe obj/Arquivos.o obj/Batalha.o obj/GameState.o obj/Grafo.o obj/Inventario.o obj/Mapa.o obj/Personagem.o obj/AVLInventario.o obj/HeapMinimo.o obj/main.o 2>&1 | Out-Null

# Versao grafica (raylib)
if (Test-Path "C:\raylib\lib") {
    gcc -o bin/zombie_gui.exe obj/Arquivos.o obj/Batalha.o obj/GameState.o obj/Grafo.o obj/Inventario.o obj/Mapa.o obj/Personagem.o obj/AVLInventario.o obj/HeapMinimo.o obj/main_gui.o -L"C:\raylib\lib" -lraylib -lwinmm -lgdi32 -lopengl32 2>&1 | Out-Null
}

Write-Host ""
if (Test-Path "bin/zombie.exe") {
    $tamanho = ((Get-Item "bin/zombie.exe").Length / 1MB).ToString('F2')
    Write-Host "SUCESSO! Executavel (terminal)" -ForegroundColor Green
    Write-Host "Localizacao: bin/zombie.exe" -ForegroundColor Green
    Write-Host "Tamanho: $tamanho MB" -ForegroundColor Green
    Write-Host "Para executar: .\bin\zombie.exe" -ForegroundColor Cyan
} else {
    Write-Host "ERRO: Executavel terminal nao foi criado" -ForegroundColor Red
}

if (Test-Path "bin/zombie_gui.exe") {
    $tamanhoGui = ((Get-Item "bin/zombie_gui.exe").Length / 1MB).ToString('F2')
    Write-Host "SUCESSO! Executavel grafico" -ForegroundColor Green
    Write-Host "Localizacao: bin/zombie_gui.exe" -ForegroundColor Green
    Write-Host "Tamanho: $tamanhoGui MB" -ForegroundColor Green
    Write-Host "Para executar: .\bin\zombie_gui.exe" -ForegroundColor Cyan
} else {
    Write-Host "Aviso: Executavel grafico nao foi criado (raylib ausente?)" -ForegroundColor Yellow
}
