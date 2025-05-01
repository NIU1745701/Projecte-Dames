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

	Moviment moviments[MAX_MOVIMENTS];
	int nMovs = 0;
	fitxa.movimentsValids(moviments, nMovs);

	Moviment movimentFet;
	bool movimentValid = false;

	for (int i = 0; i < nMovs; ++i)
	{
		if (moviments[i].getFinal() == desti)
		{
			movimentFet = moviments[i];
			movimentValid = true;
			break;
		}
	}
	if (!movimentValid)
		return false;
	
	int nCaptures = movimentFet.getnFitxesEliminades();
	for (int i = 0; i < nCaptures; i++)
	{
		int f = movimentFet.getFilaFitxaEliminada(i);
		int c = movimentFet.getColFitxaEliminada(i);
		m_tauler[f][c] = Fitxa();
	}

	m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
	m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

	if (fitxa.getTipusFitxa() == TIPUS_NORMAL)
	{
		if ((fitxa.getColorFitxa() == COLOR_BLANC && desti.getFila() == 0) || (fitxa.getColorFitxa() == COLOR_NEGRE && desti.getFila() == 7))
		{
			m_tauler[desti.getFila()][desti.getColumna()].convertirEnDama();
		}
	}
	actualitzaMovimentsValids();
	bufarFitxa(origen, movimentFet, fitxa.getColorFitxa());
	return true;
}

void Tauler::bufarFitxa(const Posicio& origenMoviment, const Moviment& movimentFet, ColorFitxa tornActual)
{
	int capturesFetes = movimentFet.getnFitxesEliminades();
	int damesFetes = movimentFet.getNumDamesCapturades(m_tauler);

	int maxCaptures = capturesFetes;
	int maxDames = damesFetes;

	Posicio millorFitxa;
	bool trobat = false;

	for (int fila = 0; fila < N_FILES; fila++) 
	{
		for (int col = 0; col < N_COLUMNES; col++) 
		{
			const Fitxa& f = m_tauler[fila][col];
			if (f.getColorFitxa() == tornActual && f.getTipusFitxa() != TIPUS_EMPTY)
			{
				Moviment millor = getMillorMovimentDeFitxa(fila, col);
				if (millor.esCaptura()) 
				{
					int captures = millor.getnFitxesEliminades();
					int dames = millor.getNumDamesCapturades(m_tauler);
					bool millorQueFet = (captures > maxCaptures) || (captures == maxCaptures && dames > maxDames);

					//Actualitzar el nmax si hi ha un millor moviment i no es la fitxa que ja s'ha mogut
					if (millorQueFet && !(fila == origenMoviment.getFila() && col == origenMoviment.getColumna())) 
					{
						maxCaptures = captures;
						maxDames = dames;
						millorFitxa = Posicio(fila, col);
						trobat = true;
					}
				}
			}
		}
	}
	if (trobat) 
	{
		m_tauler[millorFitxa.getFila()][millorFitxa.getColumna()] = Fitxa();
		std::cout << "Fitxa bufada a " << millorFitxa.toString() << std::endl;
	}
}

Moviment Tauler::getMillorMovimentDeFitxa(int fila, int col) const 
{
	Moviment millor;
	int maxCaptures = 0;
	int maxDames = 0;

	const Fitxa& f = m_tauler[fila][col];
	if (f.getTipusFitxa() == TIPUS_EMPTY) 
		return millor;

	Moviment movs[MAX_MOVIMENTS];
	int nMovs = 0;
	f.getMovimentsValids(movs, nMovs);

	for (int i = 0; i < nMovs; i++) 
	{
		if (movs[i].esCaptura()) 
		{
			int captures = movs[i].getnFitxesEliminades();
			int dames = movs[i].getNumDamesCapturades(m_tauler);

			if (captures > maxCaptures || (captures == maxCaptures && dames > maxDames))
			{
				maxCaptures = captures;
				maxDames = dames;
				millor = movs[i];
			}
		}
	}
	return millor;
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
void Tauler::getPosicionsSimples(const Posicio& origen, ColorFitxa color, TipusFitxa tipus, int& nPosicions, Posicio posicions[]) const
{
	nPosicions = 0;
	int direccions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

	for (int d = 0; d < 4; ++d) 
	{
		int df = direccions[d][0];
		int dc = direccions[d][1];

		if (tipus == TIPUS_NORMAL)
		{
			bool direccioValida = (color == COLOR_BLANC && df < 0) ||(color == COLOR_NEGRE && df > 0);

			if (direccioValida) 
			{
				int nf = origen.fila + df;
				int nc = origen.columna + dc;
				if (esPosicioValida(nf, nc) && m_tauler[nf][nc].getTipus() == TIPUS_EMPTY) 
				{
					posicions[nPosicions++] = Posicio(nf, nc);
				}
			}
		}
		// Per a dames
		else 
			if (tipus == TIPUS_DAMA) 
			{
				int nf = origen.fila + df;
				int nc = origen.columna + dc;
				while (esPosicioValida(nf, nc) && m_tauler[nf][nc].getTipus() == TIPUS_EMPTY) 
				{
					posicions[nPosicions++] = Posicio(nf, nc);
					nf += df;
					nc += dc;
				}
			}
	}
}
bool Tauler::esCaptura(const Posicio& origen, const Posicio& desti) const
{
	int df = desti.fila - origen.fila;
	int dc = desti.columna - origen.columna;

	if ((df == 2 || df == -2) && (dc == 2 || dc == -2))
	{
		int fIntermedia = origen.fila + df / 2;
		int cIntermedia = origen.columna + dc / 2;
		const Fitxa& fitxaOr = m_tauler[origen.fila][origen.columna];
		const Fitxa& fitxaSaltada = m_tauler[fIntermedia][cIntermedia];

		return fitxaSaltada.getTipus() != TIPUS_EMPTY && fitxaSaltada.getColor() != fitxaOr.getColor();
	}
	return false;
}
Posicio Tauler::getFitxaCapturada(const Posicio& origen, const Posicio& desti) const
{
	int df = desti.fila - origen.fila;
	int dc = desti.columna - origen.columna;


	if ((df == 2 || df == -2) && (dc == 2 || dc == -2))
	{
		int fIntermedia = origen.fila + df / 2;
		int cIntermedia = origen.columna + dc / 2;
		return Posicio(fIntermedia, cIntermedia);
	}
	else
	{
		return Posicio(-1, -1); // posición no válida
	}
}
bool Tauler::esPosicioValida(int fila, int columna) const 
{
	return fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES;
}


