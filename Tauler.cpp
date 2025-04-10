#include "Tauler.h"

void Tauler::afegeixFitxa(const Fitxa& fitxa, const Posicio& posicio)
{
	m_tauler[posicio.getFila()][posicio.getColumna()] = fitxa;
}

void Tauler::trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments)
{

}

void Tauler::trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments)
{

}

void Tauler::trobaMovimentsValids(Fitxa& fitxa)
{
	Posicio posicio = fitxa.getPosicio();
	Moviment moviments[MAX_MOVIMENTS];
	int nMoviments;
	TipusFitxa tipus = fitxa.getTipusFitxa();
	ColorFitxa color = fitxa.getColorFitxa();

	switch (tipus)
	{
	case TIPUS_NORMAL:
		trobaMovimentsValidsNormal(posicio, color, moviments, nMoviments);
		fitxa.movimentsValids(moviments, nMoviments);
		break;

	case TIPUS_DAMA:
		trobaMovimentsValidsDama(posicio, color, moviments, nMoviments);
		fitxa.movimentsValids(moviments, nMoviments);
		break;

	default:
		break;
	}
}

void Tauler::inicialitza(const string& nomFitxer)
{
	//Inicialitzem el tauler amb fitxes Empty

	Fitxa fitxa(TIPUS_EMPTY, COLOR_BLANC);
	int i, j;
	for (i = 0; i < N_FILES; i++)
	{
		for (j = 0; i < N_COLUMNES; j++)
		{
			fitxa.setPosicio(i, j);
			m_tauler[i][j] = fitxa;
		}
	}

	//Llegeix el fitxer i omple el tauler
	
	ifstream fitxer;
	fitxer.open(nomFitxer);

	char tipus;
	string stringPosicio;

	TipusFitxa tipusFitxa;
	ColorFitxa colorFitxa;

	Posicio posicio;

	while (!fitxer.eof())
	{
		fitxer >> tipus >> stringPosicio;

		switch (tipus)
		{
		case 'O':
			tipusFitxa = TIPUS_NORMAL;
			colorFitxa = COLOR_BLANC;
			break;

		case 'X':
			tipusFitxa = TIPUS_NORMAL;
			colorFitxa = COLOR_NEGRE;
			break;

		case 'D':
			tipusFitxa = TIPUS_DAMA;
			colorFitxa = COLOR_BLANC;
			break;

		case 'R':
			tipusFitxa = TIPUS_DAMA;
			colorFitxa = COLOR_NEGRE;
			break;

		default:
			break;
		}

		posicio.stringToPosicio(stringPosicio);

		fitxa.setPosicio(posicio);
		fitxa.setColor(colorFitxa);
		fitxa.setTipus(tipusFitxa);

		afegeixFitxa(fitxa, posicio);
	}

	fitxer.close();
}

void Tauler::actualitzaMovimentsValids()
{
	int i, j;
	for (i = 0; i < N_FILES; i++)
	{
		for (j = 0; j < N_COLUMNES; j++)
		{
			if (m_tauler[i][j].getTipusFitxa() != TIPUS_EMPTY)
			{
				trobaMovimentsValids(m_tauler[i][j]);
			}
		}
	}
}