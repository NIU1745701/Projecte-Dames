#include "Fitxa.h"
#include "tauler.hpp"
#include "posicio.hpp"

Fitxa::Fitxa(Posicio posicioInicial, TipusFitxa tipus, ColorFitxa color)
{
    m_posicio = posicioInicial;
    m_tipus = tipus;
    m_color = color;
    m_nMovimentsValids = 0;
}

Fitxa::Fitxa(TipusFitxa tipus, ColorFitxa color)
{
    m_tipus = tipus;
    m_color = color;
    m_nMovimentsValids = 0;
}

string Fitxa::toString() const
{
    string caracter;

    TipusFitxa tipus = getTipusFitxa();
    ColorFitxa color = getColorFitxa();
    if (tipus == TIPUS_EMPTY)
    {
        caracter = '_';
    }
    else
    {
        if (tipus == TIPUS_NORMAL)
        {
            if (color == COLOR_BLANC)
            {
                caracter = 'O';
            }
            else
            {
                caracter = 'X';
            }
        }
        else
        {
            if (color == COLOR_BLANC)
            {
                caracter = 'D';
            }
            else
            {
                caracter = 'R';
            }
        }
    }
    
    return caracter;
}

void Fitxa::setMovimentsValids(const Moviment movimentsValids[], int nMoviments)
{
    int i;

    for (i = 0; i < nMoviments; i++)
    {
        m_movimentsValids[i] = movimentsValids[i];
    }
}

void Fitxa::getMovimentsValids(Moviment moviments[]) const
{
    int i;
    for (i = 0; i < m_nMovimentsValids; i++)
    {
        moviments[i] = m_movimentsValids[i];
    }
}
ifstream& operator>>(ifstream& input, Fitxa& fitxa)
{
	string tipus;
	Posicio posicio;

    input >> tipus;
    input >> posicio;

	fitxa.setPosicio(posicio);

	TipusFitxa tipusFitxa;
	ColorFitxa colorFitxa;

    if (tipus == "O")
    {
        tipusFitxa = TIPUS_NORMAL;
        colorFitxa = COLOR_BLANC;
    }
    else
    {
        if (tipus == "X")
        {
            tipusFitxa = TIPUS_NORMAL;
            colorFitxa = COLOR_NEGRE;
        }
        else
        {
            if (tipus == "D")
            {
                tipusFitxa = TIPUS_DAMA;
                colorFitxa = COLOR_BLANC;
            }
            else
            {
                tipusFitxa = TIPUS_DAMA;
                colorFitxa = COLOR_NEGRE;
            }
        }
    }


	fitxa.setColor(colorFitxa);
	fitxa.setTipus(tipusFitxa);

	return input;
}
