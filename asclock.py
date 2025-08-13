"""
Cronómetro e Contagem Decrescente em ASCII Art
==============================================

Este programa mostra um cronómetro ou uma contagem decrescente no terminal,
com dígitos grandes desenhados em caracteres ASCII.

Utilização:
-----------
1. Cronómetro (conta para cima):
   python cronometro.py

2. Contagem decrescente (em minutos):
   python cronometro.py <minutos>

Exemplos:
---------
   python cronometro.py       # inicia cronómetro
   python cronometro.py 5     # conta decrescente de 5 minutos

Funcionalidades:
----------------
- Dígitos grandes em ASCII com tamanho configurável (variável SCALE).
- Atualização em tempo real (1 segundo).
- Cronómetro e contagem decrescente num único programa.
- Mensagem de aviso no final da contagem decrescente.
- Interrupção com CTRL+C.
"""

import time
import sys
import os

# Códigos ANSI para cores
GREEN = "\033[92m"
YELLOW = "\033[93m"
RED = "\033[91m"
RESET = "\033[0m"

HELP = """
    AsClock - ASCII CLOCK
    V 0.1.0
    Carlos Almeida

    UTILIZAÇÃO:
        $ asclock.py [opções] [parametros]
    
    OPÇÕES:
        -t <min>    -> temporizador de min minutos;
        -c          -> cronómetro;
"""
# Algarismos ASCII base (5x5). Usa apenas ASCII.
DIGITS = {
    "0": [
        " ### ",
        "#   #",
        "#   #",
        "#   #",
        " ### ",
    ],
    "1": [
        "  #  ",
        " ##  ",
        "  #  ",
        "  #  ",
        " ### ",
    ],
    "2": [
        " ### ",
        "#   #",
        "   # ",
        "  #  ",
        "#####",
    ],
    "3": [
        " ### ",
        "    #",
        "  ## ",
        "    #",
        " ### ",
    ],
    "4": [
        "#   #",
        "#   #",
        "#####",
        "    #",
        "    #",
    ],
    "5": [
        "#####",
        "#    ",
        "#### ",
        "    #",
        "#### ",
    ],
    "6": [
        " ### ",
        "#    ",
        "#### ",
        "#   #",
        " ### ",
    ],
    "7": [
        "#####",
        "    #",
        "   # ",
        "  #  ",
        "  #  ",
    ],
    "8": [
        " ### ",
        "#   #",
        " ### ",
        "#   #",
        " ### ",
    ],
    "9": [
        " ### ",
        "#   #",
        " ####",
        "    #",
        " ### ",
    ],
    ":": [
        "     ",
        "  #  ",
        "     ",
        "  #  ",
        "     ",
    ],
    " ": [
        "     ",
        "     ",
        "     ",
        "     ",
        "     ",
    ],
}

SCALE = 1          # aumenta/diminui o tamanho (1, 2, 3,...)
SPACE_BETWEEN = 1  # espaçamento entre dígitos (em “colunas base”)

def scale_glyph(glyph_lines, k):
    """Escala um glifo 5x5 em kx (horizontal e vertical)."""
    out = []
    for line in glyph_lines:
        scaled_h = "".join(ch * k for ch in line)
        for _ in range(k):
            out.append(scaled_h)
    return out

def render_big(text, k=SCALE, spacing=SPACE_BETWEEN):
    """Converte o texto (ex: '12:34:56') em ASCII grande."""

    # Separar por partes
    horas = text[0:2]               # O limite direito do intervalo é excluído;
    minuto = text[3:5]
    segundos = text[6:8]


    glyphsHoras = [scale_glyph(DIGITS[ch], k) for ch in horas]
    glyphsMinutos = [scale_glyph(DIGITS[ch], k) for ch in minuto]
    glyphsSegundos = [scale_glyph(DIGITS[ch], k) for ch in segundos]
    glyphsColon = scale_glyph(DIGITS[':'], k)

    height = len(glyphsHoras[0])
    gap = " " * (spacing * k)
    lines = []
    for r in range(height):
        linha = (
            GREEN + gap.join(g[r] for g in glyphsHoras) + RESET +
            gap + gap.join(glyphsColon[r:r+1]) + gap +
            YELLOW + gap.join(g[r] for g in glyphsMinutos) + RESET +
            gap + gap.join(glyphsColon[r:r+1]) + gap +
            RED + gap.join(g[r] for g in glyphsSegundos) + RESET
        )
        lines.append(linha)
    return "\n".join(lines)

def clear_screen():
    """Limpa e move cursor para topo (ANSI). Funciona na maioria dos terminais modernos."""
    sys.stdout.write("\033[2J\033[3J\033[H")
    sys.stdout.flush()

def cronometro():
    """Função cronómetro."""
    start = time.monotonic()
    try:
        while True:
            elapsed = int(time.monotonic() - start)
            h = elapsed // 3600
            m = (elapsed % 3600) // 60
            s = elapsed % 60
            clock_str = f"{h:02d}:{m:02d}:{s:02d}"

            clear_screen()
            print(render_big(clock_str))

            # sincroniza com o próximo segundo para reduzir “drift”
            target = start + (elapsed + 1)
            time.sleep(max(0, target - time.monotonic()))
    except KeyboardInterrupt:
        print("\n⏰ Cronómetro parado.")

def temporizador(minutos):
    """
    Temporizador.
    input: número de minutos.
    Conta o número de minutos em forma decrescente.
    """

    totalSegundos = minutos * 60
    start = time.monotonic()

    try:
        while totalSegundos >= 0:
            h = totalSegundos // 3600
            m = (totalSegundos % 3600) // 60
            s = totalSegundos % 60
            clockStr = f"{h:02d}:{m:02d}:{s:02d}T"

            clear_screen()
            print(render_big(clockStr))

            if totalSegundos == 0:
                print("\n⏰ Acabou o tempo")
                for _ in range(5):
                    print('\a', end='', flush=True)
                    time.sleep(1)
                break
            
            target = start + ((minutos * 60) - totalSegundos + 1)
            time.sleep(max(0, target - time.monotonic()))
            totalSegundos -= 1

    except KeyboardInterrupt:
        print("\n⏰ Contagem Interrompida")

def relogio():
    start = time.monotonic()
    try:
        while True:
            tempo = time.asctime()
            hora = tempo[11:13]
            minutos = tempo[14:16]
            segundos = tempo[17:19]
            texto = f"{hora:02s}:{minutos:02s}:{segundos:02s}"
            clear_screen()
            print(render_big(texto))

            # sincroniza com o próximo segundo para reduzir “drift”
            elapsed = int(time.monotonic() - start)
            target = start + (elapsed + 1)
            time.sleep(max(0, target - time.monotonic()))
    except KeyboardInterrupt:
        print("\n⏰ Relógio Terminado!")


# ============================= TAG: MAIN ============================= #
if __name__ == "__main__":
    # Em Windows antigos, isto ajuda o suporte a ANSI (no Win10+ já não costuma ser preciso).
    if os.name == "nt":
        os.system("")
   
    if len(sys.argv) > 1:
        if sys.argv[1] == "-t":
            try:
                try:
                    minutos = int(sys.argv[2])
                    temporizador(minutos)
                except:
                    print(f"UTILIZAÇÃO: {sys.argv[0]} -t <minutos>")
            except ValueError:
                print("[ERRO]: Introduza um número inteiro em minutos!")
        elif sys.argv[1] == "-c":
            cronometro()
        elif sys.argv[1] == "-h" or sys.argv[1] == "-help":
            print (HELP)
    else:
        relogio()