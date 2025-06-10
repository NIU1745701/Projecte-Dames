#include "Posicio.hpp"


void Posicio::ToString(string& posicio) const
{
    posicio = "a1";
    posicio[0] = 'a' + m_columna;
    posicio[1] = '1' + m_fila;
}

Posicio::Posicio(const string& posicio)
{
    fromString(posicio);
}

void Posicio::fromString(const string& posicio)
{
    if (posicio.length() >= 2)
    {
        m_columna = posicio[0] - 'a';
        m_fila = posicio[1] - '1';
    }
    else
    {
        m_fila = -1;
        m_columna = -1;
    }
}

bool Posicio::operator==(const Posicio& posicio) const
{
    return m_fila == posicio.m_fila && m_columna == posicio.m_columna;
}

ifstream& operator>>(ifstream& fitxer, Posicio& posicio)
{
    string pos;
    fitxer >> pos;
    posicio.fromString(pos);
    return fitxer;
}

ostream& operator<<(ostream& os, const Posicio& pos)
{
    string posStr;
    pos.ToString(posStr);
    os << posStr;
    return os;
}
