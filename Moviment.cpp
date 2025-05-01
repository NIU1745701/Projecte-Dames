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
	if (m_nPosicions < MAX_POSICIONS) 
	{
		m_posicions[m_nPosicions][0] = posicio.getFila();
		m_posicions[m_nPosicions][1] = posicio.getColumna();
		m_nPosicions++;
	}
}

void Moviment::AfegirFitxaEliminada(const Posicio posicio)
{
	if (m_nFitxesEliminades < MAX_FITXES_ELIMINADES)
	{
		m_fitxesEliminades[m_nFitxesEliminades][0] = posicio.getFila();
		m_fitxesEliminades[m_nFitxesEliminades][1] = posicio.getColumna();
		m_nFitxesEliminades++;
	}
}

int Movimiento::getNumDamesCapturades(const Fitxa tauler[N_FILES][N_COLUMNES]) const
{
	int numDames = 0;
	for (int i = 0; i < m_nFitxesEliminades; i++)
	{
		int fila = m_fitxesEliminades[i][0];
		int col = m_fitxesEliminades[i][1];
		TipusFitxa tipus = tauler[fila][col].getTipusFitxa();

		if (tipus == TIPUS_DAMA) 
		{
			numDames++;
		}
	}
	return numDames;
}