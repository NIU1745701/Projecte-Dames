
#ifndef JOC_H
#define JOC_H

#include "tauler.hpp"
#include "CuaMoviments.h"
#include "info_joc.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

class Joc
{
public:
    Joc();
    void inicialitza(ModeJoc mode, const std::string& nomFitxerTauler, const std::string& nomFitxerMoviments);
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    void finalitza();
    void visualitza() const;

private:
    Tauler m_tauler;
    CuaMoviments m_cua;
    ModeJoc m_mode;
    ColorFitxa m_torn;
    bool m_finalitzat;
    ColorFitxa m_guanyador;
    bool m_hiHaSeleccio;
    Posicio m_posicioSeleccionada;
    std::vector<Posicio> m_posicionsValides;
    std::string m_nomFitxerMoviments;
    std::vector<Moviment> m_movimentsPossibles;

    Moviment m_movReplay;

    Posicio screenToBoard(int mousePosX, int mousePosY) const;
    void canviarTorn();
    void comprovarFinal();
    void processaClickNormal(int mousePosX, int mousePosY);
    void processaClickReplay();
};

#endif
