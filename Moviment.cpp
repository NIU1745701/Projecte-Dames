#include "Moviment.h"

void Moviment::getInici(int& fila, int& columna) const
{
	Posicio posicio = m_posicions[0];
	
	fila = posicio.getFila();
	columna = posicio.getColumna();
}

void Moviment::getFinal(int& fila, int& columna) const
{
	Posicio posicio = m_posicions[m_nPosicions];

	fila = posicio.getFila();
	columna = posicio.getColumna();
}

void Moviment::AfegirPosicio(const Posicio posicio)
{

}

void Moviment::AfegirFitxaEliminada(const Posicio posicio)
{

}