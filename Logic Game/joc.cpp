#include "joc.hpp"
#include "GraphicManager.h"
#include <iostream>
#include <sstream>

Joc::Joc()
    : m_mode(MODE_JOC_NONE), m_torn(COLOR_BLANC),
    m_finalitzat(false), m_guanyador(COLOR_BLANC), m_hiHaSeleccio(false) {}


void Joc::inicialitza(ModeJoc mode, const std::string& nomFitxerTauler, const std::string& nomFitxerMoviments)
{
    m_mode = mode;
    m_nomFitxerMoviments = nomFitxerMoviments;
    m_tauler.inicialitza(nomFitxerTauler);

    if (mode == MODE_JOC_NORMAL)
    {
        m_cua.esborra();
        m_torn = COLOR_BLANC;
    }
    else if (mode == MODE_JOC_REPLAY)
    {
        m_cua.llegeixFitxer(nomFitxerMoviments);
    }

    m_finalitzat = false;
    m_hiHaSeleccio = false;
}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus)
{
    if (m_finalitzat)
    {
        visualitza();
        return true;
    }

    if (mouseStatus)
    {
        if (m_mode == MODE_JOC_NORMAL)
        {
            processaClickNormal(mousePosX, mousePosY);
        }
        else
            if (m_mode == MODE_JOC_REPLAY)
            {
                processaClickReplay();
            }
    }
    visualitza();
    return false;
}

void Joc::finalitza()
{
    if (m_mode == MODE_JOC_NORMAL)
    {
        m_cua.escriuFitxer(m_nomFitxerMoviments);
    }
}

void Joc::visualitza() const
{
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);

    if (m_mode == MODE_JOC_REPLAY)
    {
        int j;
        vector<Posicio> mov;
        for (j = 0; j < m_movReplay.getnPosicions(); j++)
        {
            
            m_movReplay.getPosicions(mov);
            
            int xReplay = (mov[j].getColumna() * COSTAT_CASELLA) + CASELLA_INICIAL_X;
            int yReplay = ((7 - mov[j].getFila()) * COSTAT_CASELLA) + CASELLA_INICIAL_Y;

            GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, xReplay, yReplay);
        }
    }

    m_tauler.visualitza();

    std::string modeStr;
    if (m_mode == MODE_JOC_NORMAL)
    {
        modeStr = "NORMAL";
    }
    else if (m_mode == MODE_JOC_REPLAY)
    {
        modeStr = "REPLAY";
    }
    else {
        modeStr = "NONE";
    }

    std::string tornStr;
    if (m_torn == COLOR_BLANC)
    {
        tornStr = "BLANC";
    }
    else {
        tornStr = "NEGRE";
    }

    std::ostringstream oss;
    oss << "Mode: " << modeStr << " | Torn: " << tornStr;
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 600, 1.0, oss.str().c_str());

    if (m_hiHaSeleccio)
    {
        int x = (m_posicioSeleccionada.getColumna() * COSTAT_CASELLA) + CASELLA_INICIAL_X;
        int y = ((7 - m_posicioSeleccionada.getFila()) * COSTAT_CASELLA) + CASELLA_INICIAL_Y;
        GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, x, y);

        for (size_t i = 0; i < m_posicionsValides.size(); i++) {
            int xv = (m_posicionsValides[i].getColumna() * COSTAT_CASELLA) + CASELLA_INICIAL_X;
            int yv = ((7 - m_posicionsValides[i].getFila()) * COSTAT_CASELLA) + CASELLA_INICIAL_Y;;
            GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, xv, yv);
        }
    }

    if (m_finalitzat) {
        std::string guanyador;
        if (m_guanyador == COLOR_BLANC) {
            guanyador = "BLANC";
        }
        else {
            guanyador = "NEGRE";
        }
        std::string missatge = "GUANYADOR: " + guanyador;
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 650, 1.0, missatge.c_str());
    }
}



void Joc::processaClickReplay()
{
    if (!m_cua.estaVuida())
    {
        Moviment mov;
        mov = m_cua.getInici();
        m_tauler.mouFitxaReplay(mov);
        canviarTorn();

        m_movReplay = mov;

        m_cua.eliminaInici();
    }
    else
    {
        m_finalitzat = true;
    }

    Sleep(200);
}

void Joc::processaClickNormal(int mousePosX, int mousePosY)
{
    Posicio pos = screenToBoard(mousePosX, mousePosY);

    if (!m_hiHaSeleccio)
    {
        const Fitxa* fitxa = m_tauler.getFitxa(pos);
        if (fitxa && fitxa->getColorFitxa() == m_torn && fitxa->getTipusFitxa() != TIPUS_EMPTY)
        {
            m_hiHaSeleccio = true;
            m_posicioSeleccionada = pos;

            int nPosicions = 0;
            Posicio posicionsPossibles[100];
            m_tauler.getPosicionsPossibles(pos, nPosicions, posicionsPossibles, m_movimentsPossibles);

            m_posicionsValides.clear();
            for (int i = 0; i < nPosicions; i++)
            {
                m_posicionsValides.push_back(posicionsPossibles[i]);
            }

            Sleep(50);
        }
    }
    else
    {
        bool movimentValid = false;
        for (size_t i = 0; i < m_posicionsValides.size(); i++)
        {
            if (m_posicionsValides[i] == pos)
            {
                movimentValid = true;
                break;
            }
        }
        if (movimentValid)
        {
            vector<Posicio> bufada;
            if (m_tauler.mouFitxa(m_posicioSeleccionada, pos, bufada))
            {
                Moviment mov;
                bool trobat = false;
                int j = 0;
                while (j < m_movimentsPossibles.size() && !trobat)
                {
                    if (m_movimentsPossibles[j].getFinal() == pos)
                    {
                        trobat = true;
                        mov = m_movimentsPossibles[j];

                        for (j = 0; j < bufada.size(); j++)
                        {
                            mov.afegirFitxaEliminada(bufada[j]);
                        }
                    }
                    else
                    {
                        j++;
                    }
                }

                if (trobat)
                {
                    m_cua.afegeixElement(mov);
                }
                    
                canviarTorn();
                comprovarFinal();
            }
        }
        m_hiHaSeleccio = false;
    }
}

Posicio Joc::screenToBoard(int mousePosX, int mousePosY) const
{
    // Coordenadas relativas al origen del tablero
    int dx = mousePosX - CASELLA_INICIAL_X;
    int dy = mousePosY - CASELLA_INICIAL_Y;

    // Si está fuera de los 8×8 casillas, devolvemos posición inválida
    if (dx < 0 || dx >= 8 * COSTAT_CASELLA ||
        dy < 0 || dy >= 8 * COSTAT_CASELLA)
    {
        return Posicio{ -1, -1 };
    }

    // Columna es división entera en X
    int col = dx / COSTAT_CASELLA;
    // Fila: invertimos Y porque en pantalla crece hacia abajo
    int fila = 7 - (dy / COSTAT_CASELLA);

    return Posicio(fila, col);
}



void Joc::canviarTorn()
{
    if (m_torn == COLOR_BLANC)
    {
        m_torn = COLOR_NEGRE;
    }
    else
    {
        m_torn = COLOR_BLANC;
    }
}

void Joc::comprovarFinal()
{
    int fitxesBlanques = 0;
    int fitxesNegres = 0;
    bool movimentsBlanc = false;
    bool movimentsNegre = false;

    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            const Fitxa* fitxa = m_tauler.getFitxa(Posicio(i, j));
            if (fitxa->getTipusFitxa() != TIPUS_EMPTY)
            {
                if (fitxa->getColorFitxa() == COLOR_BLANC)
                {
                    fitxesBlanques++;
                    if (fitxa->getNMovimentsValids() > 0)
                    {
                        movimentsBlanc = true;
                    }
                }
                else
                {
                    fitxesNegres++;
                    if (fitxa->getNMovimentsValids() > 0)
                    {
                        movimentsNegre = true;
                    }
                }
            }
        }
    }

    if (fitxesBlanques == 0)
    {
        m_finalitzat = true;
        m_guanyador = COLOR_NEGRE;
    }
    else if (fitxesNegres == 0) {
        m_finalitzat = true;
        m_guanyador = COLOR_BLANC;
    }
    else if (m_torn == COLOR_BLANC && !movimentsBlanc)
    {
        m_finalitzat = true;
        m_guanyador = COLOR_NEGRE;
    }
    else if (m_torn == COLOR_NEGRE && !movimentsNegre)
    {
        m_finalitzat = true;
        m_guanyador = COLOR_BLANC;
    }
}