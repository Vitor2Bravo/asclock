/*
  Asclock - versão c
  Carlos Almeida
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "includes/src/raylib.h"
#include "includes/src/raymath.h"


//#ifdef _WIN32
//#include <windows.h>
//#define SLEEP(mili)     Sleep(mili)
//#define SETOUTPUT() do                  \
//    {                                   \
//        SetConsoleOutputCP(CP_UTF8);    \
//        SetConsoleCP(CP_UTF8);          \
//    } while(0)
//#else
//#include <unistd.h>
//#define SLEEP(mili)     sleep(mili/1000)    // O parametro linux é em segundos;
//#define OUTPUT()        ;;
//#endif      // _WIN32

#define _WHITE       	"\033[37m"
#define _RED         	"\033[31m"
#define _GREEN       	"\033[32m"
#define _YELLOW      	"\033[33m"
#define _RESET       	"\033[0m"


#define HELP    "AsClock v0.1\n"                                                                              \
                "Versão C\n"                                                                                  \
                "\n"                                                                                          \
                "Utilização: casclock.exe [opção]\n\n"                                                        \
                "Opções:\n"                                                                                   \
                "   -h, --help          Mostra esta mensagem e sai.\n"                                        \
                "   -c, --crono         Contagem ascendente a partir do zero (cronómetro).\n"                 \
                "   -t, --tempo MIN     Contagem descendente a partir de um valor em minutos [MIN] dado.\n"   \
                "MODO PADRÃO (Sem Opção): Relógio.\n\n"

            
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
Pausa a execução do programa.
*/
void pausa()
{
	printf("Precione ENTER para continuar.\n");
	getchar();
}


/*
    Apresenta a mensagem de ajuda e utilização.
*/
void help()
{
    printf(HELP);
    pausa();
}


/* 
    Renderiza o relógio na tela do terminal.
*/
void render(int hora, int minutos, int segundos)
{
   
    limparTela(0, 0);

    int hora_1 = hora / 10;
    int hora_2 = hora % 10;
    int minuto_1 = minutos / 10;
    int minuto_2 = minutos % 10;
    int segundo_1 = segundos / 10;
    int segundo_2 = segundos % 10;

    
    //  #####  #####     #####  #####     #####  #####  //
    //  #   #  #   #  #  #   #  #   #  #  #   #  #   #  //
    //  #####  #####     #####  #####     #####  #####  //
    //  #   #  #   #  #  #   #  #   #  #  #   #  #   #  //
    //  #####  #####     #####  #####     #####  #####  //

    /* Espaço necessário para o Buffer de cada linha:
       Digitos:         5 * 8 = 40
       Espaços:         5 * 2 = 10
       Cores:           6 * 6 = 36
       Terminador Nulo:         1
       Total:                   87
    
       Acrescentar + 10 por segurança.
    */
    char buffer[5][97]; 

    //Os digitos são formados linha a linha;
    for (int i = 0; i < 5; i++)
    {
        //strcpy_s(buffer[i], sizeof(buffer[i]), _GREEN);                      // Inicializa o beffer e define BRANCO
        //strcat_s(buffer[i], sizeof(buffer[i]), "  ");                            
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[hora_1][i]);
        //strcat_s(buffer[i], sizeof(buffer[i]), "  ");                            
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[hora_2][i]);     
        //strcat_s(buffer[i], sizeof(buffer[i]), _WHITE);                      // BRANCO
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[10][i]);             // Dois pontos
        //strcat_s(buffer[i], sizeof(buffer[i]), _YELLOW);                     // Amarelo;
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[minuto_1][i]);
        //strcat_s(buffer[i], sizeof(buffer[i]), "  ");                            
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[minuto_2][i]);
        //strcat_s(buffer[i], sizeof(buffer[i]), _WHITE);                      // BRANCO
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[10][i]);             // Dois pontos
        //strcat_s(buffer[i], sizeof(buffer[i]), _RED);                        // VERMELHO
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[segundo_1][i]);
        //strcat_s(buffer[i], sizeof(buffer[i]), "  ");                            
        //strcat_s(buffer[i], sizeof(buffer[i]), digitos[segundo_2][i]);
        //strcat_s(buffer[i], sizeof(buffer[i]), "  ");                            
        //strcat_s(buffer[i], sizeof(buffer[i]), _RESET);                      // Reset Cor
    }
            
    // Aceleramos alguma coisa evitando o ciclo for, uma vez que o número de linhas é pequeno.
    printf("%s\n", buffer[0]);
    printf("%s\n", buffer[1]);
    printf("%s\n", buffer[2]);
    printf("%s\n", buffer[3]);
    printf("%s\n", buffer[4]);
}

/*
    Efetua contagem ascendente a partir do zero.
*/
void cronometro()
{
    int hora = 0;
    int minuto = 0;
    int segundo = 0;

    while(true)
    {
        segundo ++;

        if (segundo == 60)
        {
            minuto ++;
            segundo = 0;
        }
        
        if (minuto == 60)
        {
            hora ++;
            minuto = 0;
        }
        
        render(hora, minuto, segundo);
       WaitTime(1);  
    }
}

/*
    Efetua contagem descendente a partir de um valor em minutos dado.
*/
void temporizador(int minutos)
{
    int hora = 0;
    int minuto = 0;
    int segundo = 0;
    int contador = minutos * 60;

    if (minutos > 60)   // Tempo superior a 1 hora;
    {
        hora = (minutos / 60);
        minuto = (minutos % 60) - 1;
        segundo = 60;
    }
    else                // Tempo inferior a 1 hora;
    {
        minuto = minutos - 1;
        segundo = 60;
    }    

    while (contador > 0)
    {
        segundo --;
                
        if (segundo == 0 && minuto > 0 && hora > 0)
        {
            segundo = 60;
            minuto --;
        }
        
        if (segundo == 0 && minuto == 0 && hora > 0)
        {
            segundo = 60;
            minuto = 60;
            hora --;
        }

        contador --;
        render(hora, minuto, segundo);
        WaitTime(1);  
    } 
}
/*
    Retorna a Hora do sistema para renderização.
*/
void relogio()
{
    while(true)
    {
        time_t t;
        time(&t);
        
        struct tm tempo;
        localtime_s(&tempo, &t);
        
        int hora = tempo.tm_hour;
        int minutos = tempo.tm_min;
        int segundos = tempo.tm_sec;
        
        render(hora, minutos, segundos);
        WaitTime(1);  
    }
}


int main(int argc, char *argv[])
{
    /*    
    if (argc == 1)      // PADRÃO: Relógio;
    {
        relogio();
    }
    else if (argc == 2)
    {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) help();
        
        else if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "--crono"))
        {
            cronometro();
        }
        else help();
    }
    else if (argc == 3)
    {
        if (!strcmp(argv[1], "-t") || !strcmp(argv[1], "--tempo"))
        {
            if(atoi(argv[2]))
            {
                int minutos = atoi(argv[2]);
                temporizador(minutos);
            }
            else help();

        }
        else help();
    }
    */


    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "AsClock - Digital Clock");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            DrawText("Pressione ESC para sair", 10, 10, 20, GRAY);

            DrawPolyLines((Vector2){100, 100}, 6, 50, 0, RED);
            DrawPolyLinesEx((Vector2){250, 300}, 6, 30, 90, 10, GREEN);
            DrawPoly((Vector2){300, 300}, 6, 50, 0, BLUE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------







    return 0;
}
