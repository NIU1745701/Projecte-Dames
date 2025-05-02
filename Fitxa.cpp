#include "Fitxa.h"

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

    switch (tipus)
    {
    case TIPUS_EMPTY:
        caracter = '_';
        break;

    case TIPUS_NORMAL:
        if (color == COLOR_BLANC)
        {
            caracter = 'O';
        }
        else
        {
            caracter = 'X';
        }
        break;

    case TIPUS_DAMA:
        if (color == COLOR_BLANC)
        {
            caracter = 'D';
        }
        else
        {
            caracter = 'R';
        }
        break;

    default:
        break;
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