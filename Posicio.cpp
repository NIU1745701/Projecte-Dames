#include "posicio.hpp"
#include "tauler.hpp"

void Posicio::ToString(string& posicio) const
{
	posicio = "a1";
	posicio[0] = 'a' + m_columna;
	posicio[1] = '1' + (N_FILES - 1) - m_fila;
}

Posicio::Posicio(const string& posicio)
{
	fromString(posicio);
}

void Posicio::fromString(const string& posicio)
{
	m_fila = (N_FILES - 1) - (posicio[1] - '1');
	m_columna = posicio[0] - 'a';
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
std::ostream& operator<<(std::ostream& os, const Posicio& pos)
{
	string posStr;
	pos.ToString(posStr);
	os << posStr;
	return os;
}
