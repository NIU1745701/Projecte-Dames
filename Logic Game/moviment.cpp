#include "Moviment.h"

Moviment::Moviment(const Posicio& posicioInicial)
{
    m_posicions.push_back(posicioInicial);
}

Moviment::Moviment(const Posicio& inici, const Posicio& fi)
{
    m_posicions.push_back(inici);
    m_posicions.push_back(fi);
}

void Moviment::getPosicions(std::vector<Posicio>& posicions) const
{
    posicions = m_posicions;
}

void Moviment::getEliminades(std::vector<Posicio>& posicions) const
{
    posicions = m_fitxesEliminades;
}

Posicio Moviment::getInici() const
{
    if (!m_posicions.empty())
    {
        return m_posicions.front();
    }
    return Posicio();
}

Posicio Moviment::getFinal() const
{
    if (!m_posicions.empty())
    {
        return m_posicions.back();
    }
    return Posicio();
}

void Moviment::afegirPosicio(const Posicio& posicio)
{
    m_posicions.push_back(posicio);
}

void Moviment::afegirFitxaEliminada(const Posicio& posicio)
{
    m_fitxesEliminades.push_back(posicio);
}