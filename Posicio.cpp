#include "Posicio.h"

void Posicio::posicioToString(string& posicio) const
{
	posicio = "a1";
	posicio[0] = 'a' + m_columna;
	posicio[1] = '1' + (N_FILES - 1) - m_fila;
}

Posicio::Posicio(const string& posicio)
{
	stringToPosicio(posicio);
}

void Posicio::stringToPosicio(const string& posicio)
{
	m_fila = (N_FILES - 1) - (posicio[1] - '1');
	m_columna = posicio[0] - 'a';
}

bool Posicio::operator==(const Posicio& posicio) const
{
	return m_fila == posicio.m_fila && m_columna == posicio.m_columna;
}