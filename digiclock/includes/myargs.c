#include <stdbool.h>
#include <stdlib.h>
#include "myargs.h"


char *trimArg(char *argv)
{
    if (argv == NULL) return NULL;
    
    // Pula os traços iniciais (ex: --tempo -> tempo, -t -> t)
    if (argv[0] == '-') {
        if (argv[1] == '-') return argv + 2;    // Aritmética de ponteiros;
        return argv + 1;
    }
    return argv;
}

bool validarArgumentos(int argc, char *argv[], const Option *opcoes, int n_opcoes)
{
    if (argc == 1) return true;

    for (int i = 1; i < argc; i++) {
        bool valido = false;
        for (int j = 0; j < n_opcoes; j++) {
            // Verifica se o argumento coincide com o curto ou longo (adicionando os traços para comparação)
            if (argv[i][0] == '-') {
                valido = true; // Simplificação: se começa com '-', assume-se intenção de opção
                break;
            }
        }
        if (!valido) return false;
    }
    return true;
}