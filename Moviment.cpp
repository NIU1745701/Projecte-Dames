#include "Moviment.h"

Moviment::Moviment(int fila0, int columna0)
    : m_nPosicions(1), m_nFitxesEliminades(0)
{
    m_posicions[0] = Posicio(fila0, columna0);
}

Moviment::Moviment(const Posicio& posicioInicial)
    : m_nPosicions(1), m_nFitxesEliminades(0)
{
    m_posicions[0] = posicioInicial;
}
void Moviment::getPosicions(std::vector<Posicio>& posicions) const
{
    posicions.clear();
    for (int i = 0; i < m_nPosicions; i++)
        posicions.push_back(m_posicions[i]);
}

void Moviment::getEliminades(std::vector<Posicio>& posicions) const
{
    posicions.clear();
    for (int i = 0; i < m_nFitxesEliminades; i++)
        posicions.push_back(m_fitxesEliminades[i]);
}
void Moviment::setInici(const Posicio& inici)
{
    m_posicions[0] = inici;
}

void Moviment::setFinal(const Posicio & final)
{
    if (m_nPosicions > 0)
        m_posicions[m_nPosicions - 1] = final;
}

void Moviment::setNFitxesEliminades(int n)
{
    m_nFitxesEliminades = n;
}

void Moviment::getInici(int& fila, int& columna) const
{
    fila = m_posicions[0].getFila();
    columna = m_posicions[0].getColumna();
}

Posicio Moviment::getInici() const
{
    return m_posicions[0];
}

void Moviment::getFinal(int& fila, int& columna) const
{
    if (m_nPosicions > 0)
    {
        fila = m_posicions[m_nPosicions - 1].getFila();
        columna = m_posicions[m_nPosicions - 1].getColumna();
    }
}

Posicio Moviment::getFinal() const
{
    return (m_nPosicions > 0) ? m_posicions[m_nPosicions - 1] : Posicio();
}

void Moviment::afegirPosicio(const Posicio& posicio)
{
    if (m_nPosicions < MAX_POSICIONS)
        m_posicions[m_nPosicions++] = posicio;
}

void Moviment::afegirFitxaEliminada(const Posicio& posicio)
{
    if (m_nFitxesEliminades < MAX_FITXES_ELIMINADES)
        m_fitxesEliminades[m_nFitxesEliminades++] = posicio;
}
