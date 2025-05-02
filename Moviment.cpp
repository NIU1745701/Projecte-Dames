#include "Moviment.h"
#include "tauler.hpp"

Moviment::Moviment(int fila0, int columna0)
{
	Posicio pos(fila0, columna0);
	m_posicions[0] = pos;

	m_nPosicions = 1;
	m_nFitxesEliminades = 0;
}

Moviment::Moviment(const Posicio& posicioInicial)
{
	m_posicions[0] = posicioInicial;

	m_nPosicions = 1;
	m_nFitxesEliminades = 0;
}

void Moviment::getInici(int& fila, int& columna) const
{
	Posicio posicio = m_posicions[0];

	fila = posicio.getFila();
	columna = posicio.getColumna();
}

Posicio Moviment::getInici() const
{
	return m_posicions[0];
}

void Moviment::getFinal(int& fila, int& columna) const
{
	Posicio posicio = m_posicions[m_nPosicions];

	fila = posicio.getFila();
	columna = posicio.getColumna();
}

Posicio Moviment::getFinal() const
{
	return m_posicions[m_nPosicions];
}

void Moviment::afegirPosicio(const Posicio& posicio)
{
	if (m_nPosicions < MAX_POSICIONS)
	{
		m_posicions[m_nPosicions] = posicio;
		m_nPosicions++;
	}
}

void Moviment::afegirFitxaEliminada(const Posicio& posicio)
{
	if (m_nFitxesEliminades < MAX_FITXES_ELIMINADES)
	{
		m_fitxesEliminades[m_nFitxesEliminades] = posicio;
		m_nFitxesEliminades++;
	}
}

void Moviment::getPosicions(Posicio posicions[]) const
{
	int i;
	for (i = 0; i < m_nPosicions; i++)
	{
		posicions[i] = m_posicions[i];
	}
}

void Moviment::getEliminades(Posicio posicions[]) const
{
	int i;
	for (i = 0; i < m_nFitxesEliminades; i++)
	{
		posicions[i] = m_fitxesEliminades[i];
	}
}