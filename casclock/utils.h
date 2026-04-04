/*
 * utils.h
 * Interface Biblioteca de Utilitários
 * Carlos Almeida
 * v 0.1
 */

#pragma once

#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


// Texto
#define LINHA_BRANCO	printf("\n");
#define SEPARADOR		printf("----------------------------------------");
#define RESET       	"\033[0m"
#define BOLD        	"\033[1m"

// Cores:
#define BLACK       	"\033[30m"
#define RED         	"\033[31m"
#define GREEN       	"\033[32m"
#define YELLOW      	"\033[33m"
#define BLUE        	"\033[34m"
#define MAGENTA     	"\033[35m"
#define CYAN        	"\033[36m"
#define WHITE       	"\033[37m"

// Fundo
#define BG_BLACK    	"\033[40m"
#define BG_RED      	"\033[41m"
#define BG_GREEN    	"\033[42m"
#define BG_YELLOW   	"\033[43m"
#define BG_BLUE     	"\033[44m"
#define BG_MAGENTA  	"\033[45m"
#define BG_CYAN     	"\033[46m"
#define BG_WHITE    	"\033[47m"



//TAG: Funções

/* posicionar cursor. */
void pCursor(int x, int y);

/* Imprimir na posição (x,y). 
 * Alinhamento al: e, c e d => Esquerda, Direita e Centro;
 */
void imprimir(int x, int y, const char *str, char alinhamento);

/* Limpar tela.
 * modo m:
 *      > 0 - limpar toda a tela
 *      > 1 - limpar n carateres a partir da posição do cursor
 * n carateres: se modo for 0 n é irrelevante.
 */
void limparTela(int modo, int n);

/*
 * Gera um array de números aleatórios.
 * @param nElementos - Número de elementos a gerar.
 * @param maximo - Limite máximo (exclusivo).
 * @param incluirZero - true para incluir 0 no intervalo [0, maximo-1], false para [1, maximo].
 */
int *gerRnd(int nElementos, int maximo, bool incluirZero);

void pausa();

#endif // UTILS_H