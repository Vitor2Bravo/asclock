/*
 * utils.c
 * Biblioteca de Utilitários
 * Carlos Almeida
 * v 0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
#include <stdbool.h>

/* posicionar cursor. */
void pCursor (int x, int y)
{
	printf ("\033[%d;%dH", y, x);
}

/* Imprimir na posição (x,y). 
 * Alinhamento al: e, c e d => Esquerda, Direita e Centro;
 */
void imprimir(int x, int y, const char *str, char al)
{
	int pos;

	switch (al)
	{
	case 'e':
	    pCursor(x, y);
	    printf("%s", str);
	    break;
    case 'c':
        pos = x - (strlen(str) / 2);
	    pCursor(pos, y);
	    printf("%s", str);
        break;
    case 'd':
        pos = x - strlen(str);
	    pCursor(pos, y);
	    printf("%s", str);
        break;
    }
}

/* Limpar tela.
 * modo m:
 *      > 0 - limpar toda a tela
 *      > 1 - limpar n carateres a partir da posição do cursor
 * n carateres: se modo for 0 n é irrelevante.
 */
void limparTela(int modo, int n)
{
	if (modo == 0)                 		// Limpar toda a tela;
    {
        printf("\033[2J");	    // Limpa ecrã.
	    printf("\033[3J");	    // Limpa as linhas salvas.
	    printf("\033[H");	    // Coloca o cursor na origem.
    } 
	else if (modo == 1)
    {
        printf ("\033[%dX", n); // Limpar n carateres a partir do cursor.;
    }
}

/*
 * Gera um array de números aleatórios.
 * @param nElementos - Número de elementos a gerar.
 * @param maximo - Limite máximo (exclusivo).
 * @param incluirZero - true para incluir 0 no intervalo [0, maximo-1], false para [1, maximo].
 */
int *gerRnd(int nElementos, int maximo, bool incluirZero)
{
	// srand() should be called once in main()
	int n = nElementos;
	int max = maximo;

	int *lista = (int*) malloc(nElementos * sizeof(int));
	if (lista == NULL) {
		// Handle allocation failure
		return NULL;
	}

	if (incluirZero) // Gera números em [0, maximo-1]
	{
		for (int i = 0; i < n; i++)
		{
			lista[i] = rand() % max; 
		}
	}
	else // Gera números em [1, maximo]
	{
		for (int i = 0; i < n; i++)
		{
			lista[i] = (rand() % max) + 1; 
		}
	}

	return lista;
}

/*
Pausa a execução do programa.
*/
void pausa()
{
	printf("Precione ENTER para continuar.\n");
	getchar();
}