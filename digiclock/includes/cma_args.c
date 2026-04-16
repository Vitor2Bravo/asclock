#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "cma_args.h"


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

/*
    Testa quanto à validade, um argumento passado pela linha de comandos.
*/
bool validarArgumento(char *argv, const Option *opcoes, int n_opcoes)
{
    if ((argv[0] == '-' && argv[1] != '-') || (argv[0] == '-' && argv[1] == '-'))
    {
        for (int j = 0; j < n_opcoes; j++) 
        {
            if ((!strcmp(trimArg(argv), opcoes[j].s_arg) || (!strcmp(trimArg(argv), opcoes[j].l_arg))))  // strcmp retorna 0 quando as strings são iguais;
                return true;
        }
        fprintf(stderr, "Agumento inválido: \"%s\"\n", argv);
        return false;
    }
    
    return true;
}

/* 
    Retorna o número de argumentos. Isto é, iniciados por "-" ou "--".
*/
int numeroArgumentos(int argc, char *argv[], const Option *opcoes, int n_opcoes)
{
    int n_args = 0;

    for (int i = 0; i < argc; i++)
    {
        if ((argv[i][0] == '-' && argv[i][1] != '-') || (argv[i][0] == '-' && argv[i][1] == '-'))
            if (validarArgumento(argv[i], opcoes, n_opcoes))
                n_args ++;
    }

    return n_args;
}


VetArgs *extrairArgumentos(int n_args, int argc, char *argv[], const Option *opcoes, int n_opcoes)
{
    if (n_args == 0)
        return NULL;

    VetArgs *args = (VetArgs *) malloc(sizeof(VetArgs));
    if (args == NULL)
    {
        perror("Erro de alocação de memória!");
        exit(-1);
    }

    args->n_args = n_args;
    args->dados = (Arg *) malloc(sizeof(Arg) * n_args);
    if (args->dados == NULL)
    {
        free(args);
        perror("Erro de alocação de memória!");
        exit(-1);
    }

    int count = 0;
    for (int i = 1; i < argc; i++)
    {
        // Verifica se a string atual é um argumento (começa com -)
        if ((argv[i][0] == '-') && (validarArgumento(argv[i], opcoes, n_opcoes)))
        {
            char *cleanArg = trimArg(argv[i]);
            
            strncpy(args->dados[count].argumento, cleanArg, strlen(cleanArg));
            args->dados[count].argumento[strlen(cleanArg)] = '\0';
            args->dados[count].parametro[0] = '\0'; // Inicializa vazio
            
            // Verifica se este argumento espera um parâmetro (t, e, o ou formas longas)
            if (!strcmp(cleanArg, "t") || !strcmp(cleanArg, "tempo") ||
                !strcmp(cleanArg, "e") || !strcmp(cleanArg, "esp"))
            {
                if (i + 1 < argc && argv[i+1][0] != '-') 
                {
                    strcpy_s(args->dados[count].parametro, strlen(argv[i + 1]), argv[i + 1]);
                    args->dados[count].parametro[strlen(argv[i + 1]) + 1] = '\0';
                    i++; // Pula o próximo pois ele é o valor do parâmetro
                }
            } 
            else if (!strcmp(cleanArg, "o") || !strcmp(cleanArg, "cor"))
            {
                // Verifica se existem os 3 parâmetros de cor (R G B)
                if (i + 3 < argc)
                {
                    // Formata R, G e B em uma única string separada por espaços
                    snprintf(args->dados[count].parametro, N_PAR, "%3s%3s%3s", 
                             argv[i + 1], argv[i + 2], argv[i + 3]);
                    i += 3; // Pula os 3 parâmetros consumidos
                }
            }
            count ++;
        }
    }

    if(count != n_args)
    {
        fprintf(stderr, "Número de argumentos discrepantes: %d != %d.\n", count, n_args);
        exit(-1);
    }

    return args;
}
