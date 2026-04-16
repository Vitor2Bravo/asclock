#ifndef CMA_ARGS_H
#define CMA_ARGS_H

#include <stdbool.h>

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
