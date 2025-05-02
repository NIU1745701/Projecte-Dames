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
    if (m_tipus == TIPUS_EMPTY)
        return "_";

    if (m_tipus == TIPUS_NORMAL)
    {
        if (m_color == COLOR_BLANC)
            return "O";
        else if (m_color == COLOR_NEGRE)
            return "X";
    }

    if (m_tipus == TIPUS_DAMA)
    {
        if (m_color == COLOR_BLANC)
            return "D";
        else if (m_color == COLOR_NEGRE)
            return "R";
    }
    return "_";
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

istream& operator>>(istream& in, Fitxa& fitxa)
{
    char tipusChar;
    string posStr;

    if (in >> tipusChar >> posStr)
    {
        TipusFitxa tipus;
        ColorFitxa color;

        switch (tipusChar)
        {
        case 'O':
            tipus = TIPUS_NORMAL;
            color = COLOR_BLANC;
            break;
        case 'X':
            tipus = TIPUS_NORMAL;
            color = COLOR_NEGRE;
            break;
        case 'D':
            tipus = TIPUS_DAMA;
            color = COLOR_BLANC;
            break;
        case 'R':
            tipus = TIPUS_DAMA;
            color = COLOR_NEGRE;
            break;
        default:
            in.setstate(ios::failbit);
            return in;
        }

        Posicio pos;
        pos.fromString(posStr);

        fitxa.setTipus(tipus);
        fitxa.setColor(color);
        fitxa.setPosicio(pos);
    }

    return in;
}
