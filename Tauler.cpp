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
bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxa.getTipusFitxa() == TIPUS_EMPTY) return false;

	bool movimentValid = false;
	Posicio moviments[MAX_MOVIMENTS];
	int nMovs = 0;

	fitxa.movimentsValids(moviments, nMovs);

	for (int i = 0; i < nMovs; ++i) 
	{
		if (moviments[i] == desti) 
		{
			movimentValid = true;
			break;
		}
	}

	if (!movimentValid) 
		return false;

	// Comprobar si fitxa ha matado a alguien (si la diferencia de movimiento = 2 significa que ha saltado a alguien y lo ha matado)
	int df = desti.getFila() - origen.getFila();
	int dc = desti.getColumna() - origen.getColumna();
	bool haCapturat = false;

	if ((df == 2 || df == -2) && (dc == 2 || dc == -2))
	{
		int fCaptura = origen.fila + df / 2;
		int cCaptura = origen.columna + dc / 2;

		Fitxa& fitxaCapturada = m_tauler[fCaptura][cCaptura];
		if (fitxaCapturada.getColorFitxa() != fitxa.getColorFitxa() && fitxaCapturada.getTipusFitxa() != TIPUS_EMPTY) 
		{
			m_tauler[fCaptura][cCaptura] = Fitxa();
			haCapturat = true;
		}
	}

	m_tauler[desti.fila][desti.columna] = fitxa;
	m_tauler[origen.fila][origen.columna] = Fitxa();

	//Convertir en dama
	if (fitxa.getTipusFitxa() == TIPUS_NORMAL) 
	{
		if ((fitxa.getColorFitxa() == COLOR_BLANC && desti.getFila() == 0) || (fitxa.getColorFitxa() == COLOR_NEGRE && desti.getFila() == 7))
		{
			m_tauler[desti.getFila()][desti.getColumna()].convertirEnDama();
		}
	}

	if (!haCapturat) 
	{
		actualitzaMovimentsValids(); 
		bufarFitxa(fitxa.getColor());
	}

	return true;
}
void Tauler::bufarFitxa(ColorFitxa tornActual) 
{
	int maxCaptures = 0;
	int maxDamesCapturades = 0;
	Posicio fitxaBufada;
	bool trobat = false;

	for (int fila = 0; fila < N_FILES; fila++) 
	{
		for (int col = 0; col < N_COLUMNES; col++) 
		{
			Fitxa& f = m_tauler[fila][col];
			if (f.getColorFitxa() == tornActual && f.getTipusFitxa() != TIPUS_EMPTY) 
			{
				Moviment movs[MAX_MOVIMENTS];
				int nMovs = 0;
				f.getMovimentsValids(movs, nMovs);

				for (int i = 0; i < nMovs; i++) 
				{
					if (movs[i].esCaptura()) 
					{
						int nCaptures = movs[i].getnFitxesEliminades();
						int nDames = movs[i].getNumDamesCapturades(m_tauler);

						if (nCaptures > maxCaptures ||	(nCaptures == maxCaptures && nDames > maxDamesCapturades)) 
						{
							maxCaptures = nCaptures;
							maxDamesCapturades = nDames;
							fitxaBufada = Posicio(fila, col);
							trobat = true;
						}
					}
				}
			}
		}
	}

	if (trobat) 
	{
		m_tauler[fitxaBufada.getFila()][fitxaBufada.getColumna()] = Fitxa();
		cout << "Fitxa bufada a " << fitxaBufada.toString() << endl;
	}
}
string Tauler::toString() const
{
	string resultat;

	for (int fila = N_FILES-1; fila >= 0; fila--)
	{
		resultat += char('1' + fila);
		resultat += ": ";
		for (int columna = 0; i < N_COLUMNES; columna++)
		{
			resultat += tauler[fila][columna].getTipusFitxa();
		}
		resultat += '\n';
	}
	resultat += "  ABCDEFGH\n";
	return resultat;
}