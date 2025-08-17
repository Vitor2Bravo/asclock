"""
Relógio, Cronómetro e Contagem Decrescente em ASCII Art
==============================================

Este programa mostra um cronómetro ou uma contagem decrescente no terminal,
com dígitos grandes desenhados em caracteres ASCII.

Utilização:
-----------
1. Cronómetro (conta para cima):
   python asclock.py -c

2. Temporizador (em minutos):
   python asclock.py -t <min>

3. Relógio
   python asclock.py

Funcionalidades:
----------------
- Dígitos grandes em ASCII com tamanho configurável (variável SCALE).
- Atualização em tempo real (1 segundo).
- Cronómetro e contagem decrescente num único programa.
- Mensagem de aviso no final da contagem decrescente.
- Interrupção com CTRL+C.
"""
import time
import os
import sys
import argparse
   
# Códigos ANSI para cores
GREEN  = "\033[92m"
YELLOW = "\033[93m"
RED    = "\033[91m"
RESET  = "\033[0m"

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


# Parser Argumentos
def setup_arg_parser():
    """Configura e retorna o parser de argumentos da linha de comando."""
    parser = argparse.ArgumentParser(
        description="""Relógio, Cronómetro e Contagem Decrescente em ASCII Art.
v 0.1.0
Carlos Almeida""",
        formatter_class=argparse.RawTextHelpFormatter
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-c", "--cronometro", action="store_true", help="Executa em modo cronómetro (conta para cima).")
    group.add_argument("-t", "--temporizador", type=int, metavar="MIN", help="Executa em modo temporizador por MIN minutos.")
    return parser.parse_args()

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

    time_parts = text.split(":")
    colors = [GREEN, YELLOW, RED]
    
    # Gera os glifos para cada parte do tempo (HH, MM, SS)
    part_glyphs = [[scale_glyph(DIGITS[char], k) for char in part] for part in time_parts]
    colon_glyph = scale_glyph(DIGITS[':'], k)
    
    height = len(colon_glyph)
    gap = " " * (spacing * k)
    lines = []

    for r in range(height):
        line_parts = []
        for i, glyphs in enumerate(part_glyphs):
            # Adiciona a parte do tempo (ex: horas) com cor
            line_parts.append(colors[i] + gap.join(g[r] for g in glyphs) + RESET)
        
        # Junta as partes com o separador ":"
        separator = gap + colon_glyph[r] + gap
        lines.append(separator.join(line_parts))
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
            clock_str = f"{h:02d}:{m:02d}:{s:02d}"

            clear_screen()
            print(render_big(clock_str))

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
    """Mostra o relógio com a hora atual do sistema."""
    start = time.monotonic()
    try:
        while True:
            now = time.localtime()
            # Usa os campos tm_hour, tm_min, e tm_sec da estrutura de tempo
            texto = f"{now.tm_hour:02d}:{now.tm_min:02d}:{now.tm_sec:02d}"
            clear_screen()
            print(render_big(texto))

            # sincroniza com o próximo segundo para reduzir “drift”
            elapsed = int(time.monotonic() - start)
            target = start + (elapsed + 1)
            time.sleep(max(0, target - time.monotonic()))
    except KeyboardInterrupt:
        print("\n⏰ Relógio Terminado!")


# TAG: Main
def main():
    # Em Windows antigos, isto ajuda o suporte a ANSI (no Win10+ já não costuma ser preciso).
    # Especifico do windows.
    if os.name == "nt":
        os.system("")
        os.system("mode 49,7") # Redimensiona o terminal do windows

    args = setup_arg_parser()

    if args.cronometro:
        cronometro()
    elif args.temporizador:
        # argparse já garante que o valor é um inteiro
        temporizador(args.temporizador)
    else:
        # Modo padrão: relógio
        relogio()


if __name__ == "__main__":
    main()