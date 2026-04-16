#include <stdio.h>
//#include <string.h>
#include "includes/cma_args.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    const Option opcoes[] = {
    {"h", "help", "Mostra esta mensagem e sai."},
    {"c", "crono", "Contagem ascendente a partir do zero (cronómetro)." },
    {"t", "tempo", "Contagem descendente a partir de um valor em minutos [MIN] dado."},
    {"e", "esp", "Especifica a espessura dos segmentos."},
    {"o", "cor", "Define a cor dos digitos do relógio no formato RGB."},
    {"r", "rel", "Relógio = Modo Padrão."}
    };

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    //if (argc > 1)
    //{
    //    for (int i = 1; i < argc; i ++)     // Ciclo sobre os argumentos
    //        printf("%s\n", trimArg(argv[i]));
    //}
    //for (int i = 1; i < argc; i++)
    //{
    //    printf("%d\n", validarArgumento(argv[i], opcoes, 6));
    //}

    int n_args = numeroArgumentos(argc, argv, opcoes, 6);
    
    printf("Número de argumentos: %d\n",n_args);

    VetArgs *args = extrairArgumentos(n_args, argc, argv, opcoes, 6);

    for (int i = 0; i < n_args; i ++)
        printf("Argumento: %s - Parametro: %s\n", args->dados[i].argumento, args->dados[i].parametro);
    
    return 0;
}
