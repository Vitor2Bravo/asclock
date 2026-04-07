#ifndef MYARGS_H
#define MYARGS_H

#include <stdbool.h>

/* ================ ESTRUTURAS ================ */

typedef struct
{
    char *s_arg;         // Argumento curto;
    char *l_arg;        // Argumento longo;
    char *descricao;    // Dscrição do argumento;
} Option;


/* ================== FUNÇÕES ================= */

char *trimArg(char *argv);

/*
    Teste
*/
bool validarArgumentos(int argc, char *argv[], const Option *opcoes, int n_opcoes);

#endif //MYARGS_H