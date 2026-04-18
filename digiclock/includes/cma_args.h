#ifndef CMA_ARGS_H
#define CMA_ARGS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* ================ DEFINIÇÕES ================ */

#define N_ARG   32
#define N_PAR   32

/* ================ ESTRUTURAS ================ */

typedef struct
{
    char *s_arg;        // Argumento curto;
    char *l_arg;        // Argumento longo;
    char *descricao;    // Dscrição do argumento;
} Option;

typedef struct
{
    char argumento[N_ARG];
    char parametro[N_PAR];
} Arg;

typedef struct
{
    int n_args;
    Arg *dados;
} VetArgs;


/* ================== FUNÇÕES ================= */

char *trimArg(char *argv);

bool validarArgumento(char *argv, const Option *opcoes, int n_opcoes);

VetArgs *extrairArgumentos(int n_args, int argc, char *argv[], const Option *opcoes, int n_opcoes);

int numeroArgumentos(int argc, char *argv[], const Option *opcoes, int n_opcoes);

#endif //CMA_ARGS_H

/* ================== IMPLEMENTAÇÃO ================= */

#ifdef CMA_ARGS_IMPLEMENTATION

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

            #ifdef _WIN32 
            strncpy_s(args->dados[count].argumento, N_ARG, cleanArg, strlen(cleanArg) + 1);
            #else
            strncpy(args->dados[count].argumento, cleanArg, strlen(cleanArg) + 1);
            #endif // _WIN32
            args->dados[count].parametro[0] = '\0'; // Inicializa vazio
            
            // Verifica se este argumento espera um parâmetro (t, e, o ou formas longas)
            if (!strcmp(cleanArg, "t") || !strcmp(cleanArg, "tempo") ||
                !strcmp(cleanArg, "e") || !strcmp(cleanArg, "esp"))
            {
                if (i + 1 < argc && argv[i + 1][0] != '-') 
                {
                    #ifdef _WIN32
                    strncpy_s(args->dados[count].parametro, N_PAR, argv[i + 1], strlen(argv[i + 1]));
                    #else
                    strncpy(args->dados[count].parametro, argv[i + 1], strlen(argv[i + 1]));
                    #endif // _WIN32
                    i++; // Pula o próximo pois ele é o valor do parâmetro
                }
            } 
            else if (!strcmp(cleanArg, "o") || !strcmp(cleanArg, "cor"))
            {
                if (i + 3 < argc)
                {
                    #ifdef _WIN32
                    sprintf_s(args->dados[count].parametro, N_PAR, "%s %s %s", argv[i + 1], argv[i + 2], argv[i + 3]);
                    #else
                    sprintf(args->dados[count].parametro, "%s %s %s", argv[i + 1], argv[i + 2], argv[i + 3]);
                    #endif // _WIN32
                    i += 3; // Pula os 3 valores RGB consumidos
                }
            }
            count ++;
        }
    }

    if(count != n_args)
    {
        fprintf(stderr, "Número de Argumentos Inválidos!\n");
        exit(-1);
    }

    return args;
}
#endif //CMA_ARGS_IMPLEMENTATION
