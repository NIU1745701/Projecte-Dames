//
//  main.cpp
//
//  Copyright � 2018 Compiled Creations Limited. All rights reserved.
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined  (_WIN64)

#include <iostream>
//Definicio necesaria per poder incloure la llibreria i que trobi el main
#define SDL_MAIN_HANDLED
#include <windows.h>
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#include <conio.h>      /* getch */ 

#elif __APPLE__
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

#endif

#include "./joc.hpp"
#include "./info_joc.hpp"
#include "GraphicManager.h";


ModeJoc escullMode(int mousePosX, int mousePosY, bool mouseStatus)
{
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);

    std::string escull = "ESCULL EL MODE DE JOC:";
    std::string replay = "Mode REPLAY";
    std::string normal = "Mode NORMAL";
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 100, 1.0, escull.c_str());

    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 170, 1.0, normal.c_str());
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 400, 170, 1.0, replay.c_str());

    if (mousePosY > 160 && mousePosY < 230)
    {
        if (mousePosX > 30 && mousePosX < 270)
        {
            GraphicManager::getInstance()->drawFont(FONT_RED_30, 50, 170, 1.0, normal.c_str());

            if (mouseStatus)
            {
                return MODE_JOC_NORMAL;
            }
        }
        else
        {
            if (mousePosX > 380 && mousePosX < 620)
            {
                GraphicManager::getInstance()->drawFont(FONT_RED_30, 400, 170, 1.0, replay.c_str());

                if (mouseStatus)
                {
                    return MODE_JOC_REPLAY;
                }
            }
        }
    }


    return MODE_JOC_NONE;
}

int tornarAJugar(int mousePosX, int mousePosY, bool mouseStatus)
{
    int tornar = -1;
    
    string text = "Vols tornar a jugar: ";
    string si = "SI";
    string no = "NO";

    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 700, 1.0, text.c_str());

    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 320, 703, 1.0, si.c_str());
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 370, 703, 1.0, no.c_str());

    if (mousePosY > 690 && mousePosY < 750)
    {
        if (mousePosX > 310 && mousePosX < 350)
        {
            GraphicManager::getInstance()->drawFont(FONT_RED_30, 320, 703, 1.0, si.c_str());

            if (mouseStatus)
            {
                tornar = 1;
            }
        }
        else
        {
            if (mousePosX > 360 && mousePosX < 400)
            {
                GraphicManager::getInstance()->drawFont(FONT_RED_30, 370, 703, 1.0, no.c_str());

                if (mouseStatus)
                {
                    tornar = 0;
                }
            }
        }
    }

    return tornar;
}

int main(int argc, const char* argv[])
{
    //Instruccions necesaries per poder incloure la llibreria i que trobi el main
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    //Inicialitza un objecte de la classe Screen que s'utilitza per gestionar la finestra grafica
    Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y);
    //Mostrem la finestra grafica
    pantalla.show();

    Joc joc;

    string nomFitxer = "..\\1. Resources\\data\\Games\\tauler_inicial.txt";
    string nomFitxerMoviment = "..\\1. Resources\\data\\Games\\moviments.txt";
    
    ModeJoc mode;
    int tornarJugar = 1;
    bool escape = false;
    bool final = false;
    
    do
    {
        do
        {
            pantalla.processEvents();
            bool mouseStatus = Mouse_getBtnLeft();
            int mousePosX = Mouse_getX();
            int mousePosY = Mouse_getY();

            mode = escullMode(mousePosX, mousePosY, mouseStatus);
            pantalla.update();

            if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE))
            {
                escape = true;
            }

        } while (mode == MODE_JOC_NONE && !escape);

        joc.inicialitza(mode, nomFitxer, nomFitxerMoviment);
        Sleep(100);
        do
        {
            // Captura tots els events de ratolí i teclat de l'ultim cicle
            pantalla.processEvents();

            bool mouseStatus = Mouse_getBtnLeft();
            int mousePosX = Mouse_getX();
            int mousePosY = Mouse_getY();
            final = joc.actualitza(mousePosX, mousePosY, mouseStatus);

            if (final)
            {
                joc.finalitza();

                do
                {
                    pantalla.processEvents();
                    mouseStatus = Mouse_getBtnLeft();
                    mousePosX = Mouse_getX();
                    mousePosY = Mouse_getY();

                    joc.visualitza();
                    tornarJugar = tornarAJugar(mousePosX, mousePosY, mouseStatus);

                    pantalla.update();

                    if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE))
                    {
                        escape = true;
                    }
                } while (!escape && tornarJugar == -1);
            }

            // Actualitza la pantalla
            pantalla.update();

            if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE))
            {
                escape = true;
            }

        } while (!escape && !final);
        // Sortim del bucle si pressionem ESC

    } while (!escape && tornarJugar);

    //Instruccio necesaria per alliberar els recursos de la llibreria 
    SDL_Quit();

    return 0;
}