/*
  Digiclock - versão c / Raylib
  Carlos Almeida
*/

#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
//#include <string.h>
#include <stdbool.h>
#include "includes/src/raylib.h"
//#include "includes/src/raymath.h"


#define HELP    "AsClock v0.1\n"                                                                              \
                "Versão C\n"                                                                                  \
                "\n"                                                                                          \
                "Utilização: casclock.exe [opção]\n\n"                                                        \
                "Opções:\n"                                                                                   \
                "   -h, --help          Mostra esta mensagem e sai.\n"                                        \
                "   -c, --crono         Contagem ascendente a partir do zero (cronómetro).\n"                 \
                "   -t, --tempo MIN     Contagem descendente a partir de um valor em minutos [MIN] dado.\n"   \
                "MODO PADRÃO (Sem Opção): Relógio.\n\n"

float espessura = 25;

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
    Renderiza um segmento na tela.
*/
void renderSegmento(Vector2 posicao, bool horizontal, bool on)
{
    // Definições segmento    
    //float espessura = 25;
    float comprimento = 3 * espessura;
    float c_x = posicao.x;      // Centro x
    float c_y = posicao.y;      // Centro y

    Color cor;

    if (on)
        cor = RED;
    else
        cor = DARKGRAY;

    Vector2 p0, p1, p2, p3, p4, p5;

    if (horizontal)
    {
        p0 = (Vector2) {c_x - (comprimento / 2), c_y}; 
        p1 = (Vector2) {c_x - espessura, c_y + (espessura / 2)}; 
        p2 = (Vector2) {c_x - espessura, c_y - (espessura / 2)}; 
        p3 = (Vector2) {c_x + espessura, c_y + (espessura / 2)}; 
        p4 = (Vector2) {c_x + (espessura), c_y - (espessura / 2)}; 
        p5 = (Vector2) {c_x + (comprimento / 2), c_y}; 
    }
    else 
    {
        p0 = (Vector2) {c_x - (espessura / 2), c_y + espessura};
        p1 = (Vector2) {c_x, c_y + (comprimento / 2)};
        p2 = (Vector2) {c_x - (espessura / 2), c_y - espessura};
        p3 = (Vector2) {c_x + (espessura / 2), c_y + espessura};
        p4 = (Vector2) {c_x, c_y - (comprimento / 2)};
        p5 = (Vector2) {c_x + (espessura / 2), c_y - espessura};
    }

    Vector2 segmento[6] = {p0, p1, p2, p3, p4, p5};
    DrawTriangleStrip(segmento, 6, cor);

}


/* 
    Renderiza um digito na tela.
*/
void renderDigito(int digito, Vector2 posicao)
{
    bool digitos[10][7] = {
        {true, true, true, true, true, true, false},
        {false, true, true, false, false, false, false},
        {true, true, false, true, true, false, true},
        {true, true, true, true, false, false, true},
        {false, true, true, false, false, true, true},
        {true, false, true, true, false, true, true},
        {true, false, true, true, true, true, true},
        {true, true, true, false, false, false, false},
        {true, true, true, true, true, true, true},
        {true, true, true, true, false, false, true}
    };

    float pos_x = posicao.x;
    float pos_y = posicao.y;
    float comprimento = espessura * 3;
    float deslocamento = 2;     // Espaço entre segmentos;

    /*a*/renderSegmento((Vector2) {pos_x, pos_y - comprimento - (deslocamento * 2)}, true, (digitos[digito][0]? true : false));     // a
    /*b*/renderSegmento((Vector2) {pos_x + (comprimento / 2) + deslocamento, pos_y - (comprimento / 2) - deslocamento}, false, (digitos[digito][1]? true : false));    // b   
    /*c*/renderSegmento((Vector2) {pos_x + (comprimento / 2) + deslocamento, pos_y + (comprimento / 2) + deslocamento}, false, (digitos[digito][2]? true : false));    // c
    /*d*/renderSegmento((Vector2) {pos_x, pos_y + comprimento + (deslocamento * 2)}, true, (digitos[digito][3]? true : false));    // d
    /*e*/renderSegmento((Vector2) {pos_x - (comprimento / 2) - deslocamento, pos_y + (comprimento / 2) + deslocamento}, false, (digitos[digito][4]? true : false));    // e
    /*f*/renderSegmento((Vector2) {pos_x - (comprimento / 2) - deslocamento, pos_y - (comprimento / 2) - deslocamento}, false, (digitos[digito][5]? true : false));    // f
    /*g*/renderSegmento((Vector2) {pos_x, pos_y}, true, (digitos[digito][6]? true : false));    // g // Segmento central;

}

/*
    Renderiza o separador (:).
*/
void renderSeparador(Vector2 posicao)
{
    DrawRectangle(posicao.x, posicao.y, espessura, espessura, RED);
    DrawRectangle(posicao.x, posicao.y + (espessura * 3), espessura, espessura, RED);
}

/* 
    Renderiza o relógio na tela.
*/
void render(int horas, int minutos, int segundos)
{
    //  #####  #####     #####  #####     #####  #####  //
    //  #   #  #   #  #  #   #  #   #  #  #   #  #   #  //
    //  #####  #####     #####  #####     #####  #####  //
    //  #   #  #   #  #  #   #  #   #  #  #   #  #   #  //
    //  #####  #####     #####  #####     #####  #####  //

    int hora_1 = horas / 10;
    int hora_2 = horas % 10;
    int minuto_1 = minutos / 10;
    int minuto_2 = minutos % 10;
    int segundo_1 = segundos / 10;
    int segundo_2 = segundos % 10;

    float x_ini = 100;
    // Horas
    renderDigito(hora_1, (Vector2){x_ini, 200});
    renderDigito(hora_2, (Vector2){x_ini + (espessura * 5.5), 200});
    // Separador
    renderSeparador((Vector2){x_ini - (espessura * 0.5) + (espessura * 9.5), 200 - (espessura * 2)});
    // Minutos
    renderDigito(minuto_1, (Vector2){x_ini + (espessura * 13.5), 200});
    renderDigito(minuto_2, (Vector2){x_ini + (espessura * 19), 200});
    // Separador
    renderSeparador((Vector2){x_ini - (espessura * 0.5) + (espessura * 23), 200 - (espessura * 2)});

    // Segundos
    renderDigito(segundo_1, (Vector2){x_ini + (espessura * 27), 200});
    renderDigito(segundo_2, (Vector2){x_ini + (espessura * 32.5), 200});
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
        //tempo = localtime(&t);
        
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

/* ================= Declaração e inicialização ================= */

    const int screenWidth = 800;
    const int screenHeight = 400;
         
    InitWindow(screenWidth, screenHeight, "AsClock - Digital Clock");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

/* ================= Loop Principal ================= */

    while (!WindowShouldClose())
    {
        
        /*----------------------------------------------------- */
        // TODO: Update your variables here
        /*----------------------------------------------------- */

        /* ================= Renderização ================= */
        BeginDrawing();

            ClearBackground(BLACK);
            DrawText("Pressione ESC para sair", 10, screenHeight - 20, 20, GRAY);
            
            //GetFrameTime(); 
            render(0,0,0);

        EndDrawing();
    }


    CloseWindow();        // Close window and OpenGL context


    return 0;
}
