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

#define CMA_ARGS_IMPLEMENTATION
#include "includes/cma_args.h"

#define CMA_UTILS_IMPLEMENTATION
#include "includes/cma_utils.h"

/* ============== CONFLITOS WINDOWS ============== */

#ifdef _WIN32
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
//#define NONLS             // All NLS defines and routines
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

// Type required before windows.h inclusion
typedef struct tagMSG *LPMSG;

#include <Windows.h>
#endif // _WIN32

typedef struct
{
    Color cor;
    Color bg_cor;
    float espessura;
    
} Context;

/* ============== CONFLITOS WINDOWS ============== */

#define HELP    "DigiClock v1.0.0\n\n"                                                                          \
                "Utilização: digiclock.exe [opção] [parametro]\n\n"                                                        \
                "Opções:\n"                                                                                   \
                "   -h, --help              Mostra esta mensagem e sai.\n"                                        \
                "   -c, --crono             Contagem ascendente a partir do zero (cronómetro).\n"                 \
                "   -t, --tempo MIN         Contagem descendente a partir de um valor em minutos [MIN] dado.\n"   \
                "   -e, --esp VAL           Especifica a espessura dos segmentos.\n"                              \
                "   -o, --cor rrr ggg bbb   Define a cor dos digitos do relógio no formato RGB.\n"                \
                "MODO PADRÃO (Sem Opção): Relógio, cor = VERMELHO, espessura = 25.\n\n"

/* Ref.: cma_args.h */
const Option opcoes[] = {
    {"h", "help",  "Mostra esta mensagem e sai."},
    {"c", "crono", "Contagem ascendente a partir do zero (cronómetro)." },
    {"t", "tempo", "Contagem descendente a partir de um valor em minutos [MIN] dado."},
    {"e", "esp",   "Especifica a espessura dos segmentos."},
    {"o", "cor",   "Define a cor dos digitos do relógio no formato RGB."},
    {"r", "rel",   "Relógio = Modo Padrão."}
};

#define _DARKGRAY           (Color){ 20, 20, 20, 255 }

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
void renderSegmento(Context *cnt, Vector2 posicao, bool horizontal, bool on)
{
    // Definições segmento    
    float comprimento = 3 * cnt->espessura;
    float espessura = cnt->espessura;
    float c_x = posicao.x;      // Centro x
    float c_y = posicao.y;      // Centro y

    Color cor;

    if (on)
        cor = cnt->cor;
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
void renderDigito(Context *cnt, int digito, Vector2 posicao)
{
    bool digitos[10][7] = {
        {true,  true,  true,  true,  true,  true,  false},
        {false, true,  true,  false, false, false, false},
        {true,  true,  false, true,  true,  false, true},
        {true,  true,  true,  true,  false, false, true},
        {false, true,  true,  false, false, true,  true},
        {true,  false, true,  true,  false, true,  true},
        {true,  false, true,  true,  true,  true,  true},
        {true,  true,  true,  false, false, false, false},
        {true,  true,  true,  true,  true,  true,  true},
        {true,  true,  true,  true,  false, true,  true}
    };

    float pos_x = posicao.x;
    float pos_y = posicao.y;
    float comprimento = cnt->espessura * 3;
    float deslocamento = 2;     // Espaço entre segmentos;

    /*a*/renderSegmento(cnt, (Vector2) {pos_x, pos_y - comprimento - (deslocamento * 2)}, true, (digitos[digito][0]? true : false));
    /*b*/renderSegmento(cnt, (Vector2) {pos_x + (comprimento / 2) + deslocamento, pos_y - (comprimento / 2) - deslocamento}, false, digitos[digito][1]);
    /*c*/renderSegmento(cnt, (Vector2) {pos_x + (comprimento / 2) + deslocamento, pos_y + (comprimento / 2) + deslocamento}, false, digitos[digito][2]);
    /*d*/renderSegmento(cnt, (Vector2) {pos_x, pos_y + comprimento + (deslocamento * 2)}, true, digitos[digito][3]);
    /*e*/renderSegmento(cnt, (Vector2) {pos_x - (comprimento / 2) - deslocamento, pos_y + (comprimento / 2) + deslocamento}, false, digitos[digito][4]);
    /*f*/renderSegmento(cnt, (Vector2) {pos_x - (comprimento / 2) - deslocamento, pos_y - (comprimento / 2) - deslocamento}, false, digitos[digito][5]);
    /*g*/renderSegmento(cnt, (Vector2) {pos_x, pos_y}, true, digitos[digito][6]); // Segmento central;

}

/*
    Renderiza o separador (:).
*/
void renderSeparador(Context *cnt, Vector2 posicao)
{
    float espessura = cnt->espessura;
    DrawRectangle(posicao.x - (espessura / 2), posicao.y - (espessura * 2), espessura, espessura, cnt->cor);
    DrawRectangle(posicao.x - (espessura / 2), posicao.y + espessura, espessura, espessura, cnt->cor);
}

/* 
    Renderiza o relógio na tela.
*/
void render(Context *cnt, int horas, int minutos, int segundos)
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
    
    float x_ini = 3 * cnt->espessura;
    float y_ini = 4.5 * cnt->espessura;
    float espessura = cnt->espessura;
    
    // Horas
    renderDigito(cnt, hora_1, (Vector2){x_ini, y_ini});
    renderDigito(cnt, hora_2, (Vector2){x_ini + (espessura * 5), y_ini});
    // Separador
    renderSeparador(cnt, (Vector2){x_ini + (espessura * 8.5), y_ini});
    // Minutos
    renderDigito(cnt, minuto_1, (Vector2){x_ini + (espessura * 12), y_ini});
    renderDigito(cnt, minuto_2, (Vector2){x_ini + (espessura * 17), y_ini});
    // Separador
    renderSeparador(cnt, (Vector2){x_ini + (espessura * 20.5), y_ini});

    // Segundos
    renderDigito(cnt, segundo_1, (Vector2){x_ini + (espessura * 24), y_ini});
    renderDigito(cnt, segundo_2, (Vector2){x_ini + (espessura * 29), y_ini});
}


/*
    Efetua contagem ascendente a partir do zero.
*/
void cronometro(Context *cnt)
{
    static int hora = 0;
    static int minuto = 0;
    static int segundo = 0;

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
        
    render(cnt, hora, minuto, segundo);
    WaitTime(1);  
    
}

/*
    Efetua contagem descendente a partir de um valor em minutos dado.
*/
void temporizador(Context *cnt, long minutos)
{
    static long totalSegundos = -1;
    static double ultimaAtualizacao = 0;

    if (totalSegundos == -1) {
        totalSegundos = minutos * 60;
        ultimaAtualizacao = GetTime();
    }

    double tempoAtual = GetTime();
    if (tempoAtual - ultimaAtualizacao >= 1.0) {
        if (totalSegundos > 0) {
            totalSegundos--;
        } else {
            render(cnt, 0, 0, 0);
        }
        ultimaAtualizacao = tempoAtual;
    }
    
    int h = totalSegundos / 3600;
    int m = (totalSegundos % 3600) / 60;
    int s = totalSegundos % 60;

    render(cnt, h, m, s);
}

/*
    Retorna a Hora do sistema para renderização.
*/
void relogio(Context *cnt)
{
    time_t t;
    time(&t);
    struct tm *tempo;
    tempo = localtime(&t);
    int hora = tempo->tm_hour;
    int minutos = tempo->tm_min;
    int segundos = tempo->tm_sec;
    render(cnt, hora, minutos, segundos);
}


int main(int argc, char *argv[])
{
    Context cnt = (Context) {  
            .bg_cor =       BLACK,
            .cor    =       RED,
            .espessura =    25
    };
    
    Image icon      = LoadImage("./resources/digi_icon.png");
    char opcao      = 'r';       // Opções: r (Padrão), h, c, t, e, o;
    long min        = 0;         // Temporizador;

    int n_args = numeroArgumentos(argc, argv, opcoes, 6);
    VetArgs *args = extrairArgumentos(n_args, argc, argv, opcoes, 6);
    
    for (int i = 0; i < n_args; i++)
    {
        if (!strcmp(args->dados[i].argumento, "h") || !strcmp(args->dados[i].argumento, "help"))
        {
            #ifdef _WIN32
                STARTUPINFO si;
                PROCESS_INFORMATION pi;

                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));
                TODO("Implementar HELP compativel com Windows e Unix.");
                if(!CreateProcess(NULL,                         // No module name (use command line)
                                    "notepad digiclock.hlp",    // Command line
                                    NULL,                   // Process handle not inheritable
                                    NULL,                       // Thread handle not inheritable
                                    false,                      // Set handle inheritance to FALSE
                                    0,                          // No creation flags
                                    NULL,                       // Use parent's environment block
                                    NULL,                       // Use parent's starting directory 
                                    &si,                        // Pointer to STARTUPINFO structure
                                    &pi)                        // Pointer to PROCESS_INFORMATION structure
                ) 
                {
                    printf( "CreateProcess failed (%lu).\n", GetLastError() );
                    return -1;
                }
                
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            
            #else
                help();
            #endif // _WIN32

            return 0;
        }
        
        if (!strcmp(args->dados[i].argumento, "c") || !strcmp(args->dados[i].argumento, "crono"))
            opcao = 'c';

        if (!strcmp(args->dados[i].argumento, "t") || !strcmp(args->dados[i].argumento, "tempo"))
        {
            opcao = 't';
            min = atoi(args->dados[i].parametro);
        }

        if (!strcmp(args->dados[i].argumento, "e") || !strcmp(args->dados[i].argumento, "esp"))
            cnt.espessura = atof(args->dados[i].parametro);
        
        if (!strcmp(args->dados[i].argumento, "o") || !strcmp(args->dados[i].argumento, "cor"))
        {
            int red, green, blue;
            
            #ifdef _WIN32
            if(sscanf_s(args->dados[i].parametro, "%d %d %d", &red, &green, &blue) == 3)
            #else
            if(sscanf(args->dados[i].parametro, "%d %d %d", &red, &green, &blue) == 3)
            #endif
                
                cnt.cor = (Color) {red, green, blue, 255};
        }
    }
    
    const int screenWidth  = 35 * cnt.espessura;
    const int screenHeight = 9 * cnt.espessura;

/* ================= Declaração e inicialização ================= */
    #ifdef _WIN32
    ;
    #else 
        SetTraceLogLevel(LOG_NONE);
    #endif // _WIN32

    InitWindow(screenWidth, screenHeight, "Relógio Digital");
    SetWindowState(FLAG_WINDOW_TOPMOST);
    SetWindowIcon(icon);
    SetTargetFPS(60);

/* ================= Loop Principal ================= */

    while (!WindowShouldClose())
    {
        /* ================= Renderização ================= */
        BeginDrawing();
        
        ClearBackground(cnt.bg_cor);
        
        switch (opcao)
        {
            case ('c'):
            {
                cronometro(&cnt);
                break;
            }
            
            case ('t'):
            {
                temporizador(&cnt, min);
                break;
            }
           
            default:
            relogio(&cnt);
            break;
        };
        
        EndDrawing();
    }

    CloseWindow();                    // Close window and OpenGL context

    return 0;
}
