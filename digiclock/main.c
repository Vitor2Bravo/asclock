/*
  Digiclock - versão c / Raylib
  Carlos Almeida
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "includes/src/raylib.h"
#include "includes/myargs.h"


#define HELP    "DigiClock v0.1\n\n"                                                                              \
                "Utilização: casclock.exe [opção]\n\n"                                                        \
                "Opções:\n"                                                                                   \
                "   -h, --help          Mostra esta mensagem e sai.\n"                                        \
                "   -c, --crono         Contagem ascendente a partir do zero (cronómetro).\n"                 \
                "   -t, --tempo MIN     Contagem descendente a partir de um valor em minutos [MIN] dado.\n"   \
                "   -e, --esp VAL       Especifica a espessura dos segmentos.\n"   \
                "   -o   --cor   r g b  Define a cor dos digitos do relógio no formato RGB.\n"                \
                "MODO PADRÃO (Sem Opção): Relógio, cor = VERMELHO, espessura = 25.\n\n"

const Option opcoes[] = {
    {"h", "help", "Mostra esta mensagem e sai."},
    {"c", "crono", "Contagem ascendente a partir do zero (cronómetro)." },
    {"t", "tempo", "Contagem descendente a partir de um valor em minutos [MIN] dado."},
    {"e", "esp", "Especifica a espessura dos segmentos."},
    {"o", "cor", "Define a cor dos digitos do relógio no formato RGB."},
    {"r", "rel", "Relógio = Modo Padrão."}
};





#define _DARKGRAY           (Color){ 20, 20, 20, 255 }
float espessura = 25;       // Tamanho predefinido da espessura do segmento;

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
void renderSegmento(Vector2 posicao, bool horizontal, bool on, Color color)
{
    // Definições segmento    
    //float espessura = 25;
    float comprimento = 3 * espessura;
    float c_x = posicao.x;      // Centro x
    float c_y = posicao.y;      // Centro y

    Color cor;

    if (on)
        cor = color;
    else
        cor = _DARKGRAY;

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
void renderDigito(int digito, Vector2 posicao, Color cor)
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
        {true, true, true, true, false, true, true}
    };

    float pos_x = posicao.x;
    float pos_y = posicao.y;
    float comprimento = espessura * 3;
    float deslocamento = 2;     // Espaço entre segmentos;

    /*a*/renderSegmento((Vector2) {pos_x, pos_y - comprimento - (deslocamento * 2)}, true, (digitos[digito][0]? true : false), cor);     // a
    /*b*/renderSegmento((Vector2) {pos_x + (comprimento / 2) + deslocamento, pos_y - (comprimento / 2) - deslocamento}, false, (digitos[digito][1]? true : false), cor);    // b   
    /*c*/renderSegmento((Vector2) {pos_x + (comprimento / 2) + deslocamento, pos_y + (comprimento / 2) + deslocamento}, false, (digitos[digito][2]? true : false), cor);    // c
    /*d*/renderSegmento((Vector2) {pos_x, pos_y + comprimento + (deslocamento * 2)}, true, (digitos[digito][3]? true : false), cor);    // d
    /*e*/renderSegmento((Vector2) {pos_x - (comprimento / 2) - deslocamento, pos_y + (comprimento / 2) + deslocamento}, false, (digitos[digito][4]? true : false), cor);    // e
    /*f*/renderSegmento((Vector2) {pos_x - (comprimento / 2) - deslocamento, pos_y - (comprimento / 2) - deslocamento}, false, (digitos[digito][5]? true : false), cor);    // f
    /*g*/renderSegmento((Vector2) {pos_x, pos_y}, true, (digitos[digito][6]? true : false), cor);    // g // Segmento central;

}

/*
    Renderiza o separador (:).
*/
void renderSeparador(Vector2 posicao, Color cor)
{
    DrawRectangle(posicao.x - (espessura / 2), posicao.y - (espessura * 2), espessura, espessura, cor);
    DrawRectangle(posicao.x - (espessura / 2), posicao.y + espessura, espessura, espessura, cor);
}

/* 
    Renderiza o relógio na tela.
*/
void render(int horas, int minutos, int segundos, Color cor)
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
    
    float x_ini = 3 * espessura;
    float y_ini = 4.5 * espessura;
    
    // Horas
    renderDigito(hora_1, (Vector2){x_ini, y_ini}, cor);
    renderDigito(hora_2, (Vector2){x_ini + (espessura * 5), y_ini}, cor);
    // Separador
    renderSeparador((Vector2){x_ini + (espessura * 8.5), y_ini}, cor);
    // Minutos
    renderDigito(minuto_1, (Vector2){x_ini + (espessura * 12), y_ini}, cor);
    renderDigito(minuto_2, (Vector2){x_ini + (espessura * 17), y_ini}, cor);
    // Separador
    renderSeparador((Vector2){x_ini + (espessura * 20.5), y_ini}, cor);

    // Segundos
    renderDigito(segundo_1, (Vector2){x_ini + (espessura * 24), y_ini}, cor);
    renderDigito(segundo_2, (Vector2){x_ini + (espessura * 29), y_ini}, cor);
}


/*
    Efetua contagem ascendente a partir do zero.
*/
void cronometro(Color cor)
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
        
        render(hora, minuto, segundo, cor);
        WaitTime(1);  
    }
}

/*
    Efetua contagem descendente a partir de um valor em minutos dado.
*/
void temporizador(int minutos, Color cor)
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
        render(hora, minuto, segundo, cor);
        WaitTime(1);  
    } 
}
/*
    Retorna a Hora do sistema para renderização.
*/
void relogio(Color cor)
{
    time_t t;
    time(&t);
    struct tm tempo;
    localtime_s(&tempo, &t);
    int hora = tempo.tm_hour;
    int minutos = tempo.tm_min;
    int segundos = tempo.tm_sec;
    render(hora, minutos, segundos, cor);
}


int main(int argc, char *argv[])
{
    const int screenWidth = 35 * espessura;
    const int screenHeight = 9 * espessura;
    Color cor = RED;    // Cor padrão

    char opcao = 'r';

    if (!validarArgumentos(argc, argv))     //FIXME
        help(); 

    if (argc == 1)      // PADRÃO: Relógio;
    {
        relogio(cor);
    }
    else if (argc == 2)
    {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) help();
        
        else if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "--crono"))
        {
            cronometro(cor);
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
                temporizador(minutos, cor);
            }
            else help();

        }
        else help();
    }

/* ================= Declaração e inicialização ================= */
             
    InitWindow(screenWidth, screenHeight, "Relógio Digital");
    SetWindowState(FLAG_WINDOW_TOPMOST);
    SetTargetFPS(60);

/* ================= Loop Principal ================= */

    while (!WindowShouldClose())
    {
        
        /*----------------------------------------------------- */
        // TODO: Update your variables here
        /*----------------------------------------------------- */

        /* ================= Renderização ================= */
        BeginDrawing();

            ClearBackground(BLACK);
            
            relogio(cor);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
