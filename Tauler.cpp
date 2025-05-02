#include "tauler.hpp"

void Tauler::afegeixFitxa(const Fitxa& fitxa)
{
	Posicio posicio;
	posicio = fitxa.getPosicio();

	m_tauler[posicio.getFila()][posicio.getColumna()] = fitxa;
}

// Metodes per Inicialitza

void Tauler::inicialitzaEmpty()
{
	Fitxa fitxa(TIPUS_EMPTY, COLOR_BLANC);

	int i, j;
	for (i = 0; i < N_FILES; i++)
	{
		for (j = 0; j < N_COLUMNES; j++)
		{
			fitxa.setPosicio(i, j);
			afegeixFitxa(fitxa);
		}
	}
}

void Tauler::inicialitza(const string& nomFitxer)
{
	inicialitzaEmpty();

	ifstream fitxer;
	fitxer.open(nomFitxer);

	Fitxa fitxa;

	while (fitxer >> fitxa)
	{
		afegeixFitxa(fitxa);
	}


	fitxer.close();
}

//Metode to string

string Tauler::toString() const
{
	string tauler;

	int i, j;
	for (i = N_FILES - 1; i >= 0; i--)
	{
		tauler += to_string(i + 1) + ": ";

		for (j = 0; j < N_COLUMNES; j++)
		{
			tauler += m_tauler[i][j].toString() + " ";
		}

		tauler += "\n";
	}

	tauler += "  a b c d e f g h";
	return tauler;
}


//Metodes per moure fitxa

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxa.getTipusFitxa() == TIPUS_EMPTY) return false;

	Moviment moviments[MAX_MOVIMENTS];
	int nMovs = fitxa.getNMovimentsValids();
	fitxa.getMovimentsValids(moviments);

	Moviment movimentFet;
	bool movimentValid = false;

	Posicio posicioFinal;
	int fila;
	int columna;
	for (int i = 0; i < nMovs; ++i)
	{
		moviments[i].getFinal(fila, columna);
		posicioFinal.setFila(fila);
		posicioFinal.setColumna(columna);
		if (posicioFinal == desti)
		{
			movimentFet = moviments[i];
			movimentValid = true;
			break;
		}
	}
	if (!movimentValid)
		return false;

	int nCaptures = movimentFet.getnFitxesEliminades();
	Posicio fitxesEliminades[MAX_FITXES_ELIMINADES];
	movimentFet.getEliminades(fitxesEliminades);
	for (int i = 0; i < nCaptures; i++)
	{
		Fitxa fitxaEmpty(fitxesEliminades[i], TIPUS_EMPTY, COLOR_BLANC);
		afegeixFitxa(fitxaEmpty);
	}

	m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
	m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

	if (fitxa.getTipusFitxa() == TIPUS_NORMAL)
	{
		if ((fitxa.getColorFitxa() == COLOR_BLANC && desti.getFila() == 0) || (fitxa.getColorFitxa() == COLOR_NEGRE && desti.getFila() == 7))
		{
			m_tauler[desti.getFila()][desti.getColumna()].convertirDama();
		}
	}
	actualitzaMovimentsValids();
	bufarFitxa(origen, movimentFet, fitxa.getColorFitxa());
	return true;
}

void Tauler::bufarFitxa(const Posicio& origenMoviment, const Moviment& movimentFet, ColorFitxa tornActual)
{
	int capturesFetes = movimentFet.getnFitxesEliminades();
	int damesFetes = getNumDamesCapturades(movimentFet);

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
					int dames = getNumDamesCapturades(millor);
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
		Posicio posicioEmpty(millorFitxa.getFila(), millorFitxa.getColumna());
		Fitxa fitxaEmpty(posicioEmpty, TIPUS_EMPTY, COLOR_BLANC);
		afegeixFitxa(fitxaEmpty);
	}
}

Moviment Tauler::getMillorMovimentDeFitxa(int fila, int col) const
{
	Moviment millor;
	int maxCaptures = 0;
	int maxDames = 0;

	const Fitxa& fitxa = m_tauler[fila][col];
	if (fitxa.getTipusFitxa() == TIPUS_EMPTY)
		return millor;

	Moviment moviments[MAX_MOVIMENTS];
	int nMovs = fitxa.getNMovimentsValids();
	fitxa.getMovimentsValids(moviments);

	for (int i = 0; i < nMovs; i++)
	{
		if (moviments[i].esCaptura())
		{
			int captures = moviments[i].getnFitxesEliminades();
			int dames = getNumDamesCapturades(moviments[i]);

			if (captures > maxCaptures || (captures == maxCaptures && dames > maxDames))
			{
				maxCaptures = captures;
				maxDames = dames;
				millor = moviments[i];
			}
		}
	}
	return millor;
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
			bool direccioValida = (color == COLOR_BLANC && df < 0) || (color == COLOR_NEGRE && df > 0);

			if (direccioValida)
			{
				int nf = origen.getFila() + df;
				int nc = origen.getColumna() + dc;
				if (esPosicioValida(nf, nc) && m_tauler[nf][nc].getTipusFitxa() == TIPUS_EMPTY)
				{
					Posicio posicio(nf, nc);
					posicions[nPosicions++] = posicio;
				}
			}
		}
		// Per a dames
		else
		{
			if (tipus == TIPUS_DAMA)
			{
				int nf = origen.getFila() + df;
				int nc = origen.getColumna() + dc;
				while (esPosicioValida(nf, nc) && m_tauler[nf][nc].getTipusFitxa() == TIPUS_EMPTY)
				{
					posicions[nPosicions++] = Posicio(nf, nc);
					nf += df;
					nc += dc;
				}
			}
		}
	}
}

bool Tauler::esCaptura(const Posicio& origen, const Posicio& desti) const
{
	int df = desti.getFila() - origen.getFila();
	int dc = desti.getColumna() - origen.getColumna();

	if ((df == 2 || df == -2) && (dc == 2 || dc == -2))
	{
		int fIntermedia = origen.getFila() + df / 2;
		int cIntermedia = origen.getColumna() + dc / 2;
		Fitxa fitxaOr = m_tauler[origen.getFila()][origen.getColumna()];
		Fitxa fitxaSaltada = m_tauler[fIntermedia][cIntermedia];

		return fitxaSaltada.getTipusFitxa() != TIPUS_EMPTY && fitxaSaltada.getColorFitxa() != fitxaOr.getColorFitxa();
	}
	return false;
}

Posicio Tauler::getFitxaCapturada(const Posicio& origen, const Posicio& desti) const
{
	int df = desti.getFila() - origen.getFila();
	int dc = desti.getColumna() - origen.getColumna();


	if ((df == 2 || df == -2) && (dc == 2 || dc == -2))
	{
		int fIntermedia = origen.getFila() + df / 2;
		int cIntermedia = origen.getColumna() + dc / 2;
		return Posicio(fIntermedia, cIntermedia);
	}
	else
	{
		return Posicio(-1, -1); // posici n no v lida
	}
}
bool Tauler::esPosicioValida(int fila, int columna) const
{
	return fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES;
}

int Tauler::getNumDamesCapturades(const Moviment& moviment) const
{
	int numDames = 0;

	Posicio pos;

	Posicio eliminades[MAX_FITXES_ELIMINADES];
	moviment.getEliminades(eliminades);

	for (int i = 0; i < moviment.getnFitxesEliminades(); i++)
	{
		pos = eliminades[i];

		if (m_tauler[pos.getFila()][pos.getColumna()].getTipusFitxa() == TIPUS_DAMA)
		{
			numDames++;
		}
	}

	return numDames;
}

void Tauler::getPosicionsPossibles(const Posicio& origen,
	int& nPosicions, Posicio posicionsPossibles[])
{
	actualitzaMovimentsValids();

	Moviment moviments[MAX_MOVIMENTS];

	nPosicions = m_tauler[origen.getFila()][origen.getColumna()].getNMovimentsValids();
	m_tauler[origen.getFila()][origen.getColumna()].getMovimentsValids(moviments);

	int i;
	int fila;
	int columna;
	for (i = 0; i < nPosicions; i++)
	{
		moviments[i].getFinal(fila, columna);
		posicionsPossibles[i].setFila(fila);
		posicionsPossibles[i].setColumna(columna);
	}
}

//Implementacio Actualitza moviments valids

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
		fitxa.setMovimentsValids(moviments, nMoviments);
		break;

	case TIPUS_DAMA:
		trobaMovimentsValidsDama(posicio, color, moviments, nMoviments);
		fitxa.setMovimentsValids(moviments, nMoviments);
		break;

	default:
		break;
	}
}

void Tauler::trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments)
{
	Moviment movimentsPendents[50];
	int nPendents = 0;
	int incrementPendents = 0;

	nMoviments = 0;

	Moviment movimentPendent;
	Moviment movimentActual;
	movimentActual.afegirPosicio(posicio);
	movimentsPendents[nPendents++] = movimentActual;

	Posicio posicioActual = posicio;
	Posicio posicionsValides[2];
	Posicio eliminada;
	int i;

	do
	{
		movimentActual = movimentsPendents[0];
		for (i = 1; i < nPendents; ++i)
			movimentsPendents[i - 1] = movimentsPendents[i];
		--nPendents;

		posicioActual = movimentActual.getFinal();

		posicionsValidesNormal(posicioActual, color, posicionsValides, incrementPendents);

		while (incrementPendents > 0)
		{
			movimentActual.afegirPosicio(posicionsValides[0]);
			if (mataFitxa(posicioActual, posicionsValides[0], eliminada))
				movimentActual.afegirFitxaEliminada(eliminada);

			for (i = 1; i < incrementPendents; ++i)
			{
				movimentPendent = movimentActual;
				movimentPendent.afegirPosicio(posicionsValides[i]);
				if (mataFitxa(posicioActual, posicionsValides[i], eliminada))
				{
					movimentPendent.afegirFitxaEliminada(eliminada);
				}
				if (nPendents < 50)
					movimentsPendents[nPendents++] = movimentPendent;
			}

			posicioActual = posicionsValides[0];
			posicionsValidesNormal(posicioActual, color, posicionsValides, incrementPendents);
		}

		if (!(movimentActual.getFinal() == movimentActual.getInici()))
			moviments[nMoviments++] = movimentActual;

	} while (nPendents > 0);
}
void Tauler::trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments)
{
	Moviment movimentsPendents[50];
	int nPendents = 0;
	int incrementPendents = 0;

	nMoviments = 0;

	Moviment movimentPendent;
	Moviment movimentActual;
	movimentActual.afegirPosicio(posicio);
	movimentsPendents[nPendents++] = movimentActual;

	Posicio posicioActual = posicio;
	Posicio posicionsValides[2];
	Posicio eliminada;
	int i;

	do
	{
		movimentActual = movimentsPendents[0];
		for (i = 1; i < nPendents; ++i)
			movimentsPendents[i - 1] = movimentsPendents[i];
		--nPendents;

		posicioActual = movimentActual.getFinal();

		posicionsValidesDama(posicioActual, color, posicionsValides, incrementPendents);

		while (incrementPendents > 0)
		{
			movimentActual.afegirPosicio(posicionsValides[0]);
			if (mataFitxa(posicioActual, posicionsValides[0], eliminada))
				movimentActual.afegirFitxaEliminada(eliminada);

			for (i = 1; i < incrementPendents; ++i)
			{
				movimentPendent = movimentActual;
				movimentPendent.afegirPosicio(posicionsValides[i]);
				if (mataFitxa(posicioActual, posicionsValides[i], eliminada))
				{
					movimentPendent.afegirFitxaEliminada(eliminada);
				}
				if (nPendents < 50)
					movimentsPendents[nPendents++] = movimentPendent;
			}

			posicioActual = posicionsValides[0];
			posicionsValidesDama(posicioActual, color, posicionsValides, incrementPendents);
		}

		if (!(movimentActual.getFinal() == movimentActual.getInici()))
			moviments[nMoviments++] = movimentActual;

	} while (nPendents > 0);
}

void Tauler::posicionsValidesNormal(const Posicio& posicio, ColorFitxa color, Posicio posicions[], int& nPosicions)
{
	nPosicions = 0;
	Posicio pos;

	switch (color)
	{
	case COLOR_BLANC:
		// La fitxa es blanca
		// Evitar desbordament: nom s permet capturar si hi ha espai per a fila+2
		if (posicio.getFila() != N_FILES - 1)
		{
			// Comprovem a l'esquerra
			if (posicio.getColumna() != 0)
			{
				if (posicio.getColumna() != N_COLUMNES - 1)
				{
					// Moviment simple
					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 1);
						pos.setColumna(posicio.getColumna() - 1);
						posicions[nPosicions++] = pos;
					}
					else
					{
						if (
							m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getColorFitxa() == COLOR_NEGRE
							&& posicio.getColumna() > 1
							&& posicio.getFila() + 2 < N_FILES
							&& m_tauler[posicio.getFila() + 2][posicio.getColumna() - 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							// Pot matar fitxa
							pos.setFila(posicio.getFila() + 2);
							pos.setColumna(posicio.getColumna() - 2);
							posicions[nPosicions++] = pos;
						}
					}

					// Comprovem a la dreta
					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 1);
						pos.setColumna(posicio.getColumna() + 1);
						posicions[nPosicions++] = pos;
					}
					else
					{
						if (
							m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getColorFitxa() == COLOR_NEGRE
							&& posicio.getColumna() < N_COLUMNES - 2
							&& posicio.getFila() + 2 < N_FILES
							&& m_tauler[posicio.getFila() + 2][posicio.getColumna() + 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							// Pot matar fitxa
							pos.setFila(posicio.getFila() + 2);
							pos.setColumna(posicio.getColumna() + 2);
							posicions[nPosicions++] = pos;
						}
					}
				}
				else
				{
					// Columna  ltima: nom s esquerra
					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 1);
						pos.setColumna(posicio.getColumna() - 1);
						posicions[nPosicions++] = pos;
					}
					else
					{
						if (
							m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getColorFitxa() == COLOR_NEGRE
							&& posicio.getFila() + 2 < N_FILES
							&& m_tauler[posicio.getFila() + 2][posicio.getColumna() - 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							// Pot matar fitxa
							pos.setFila(posicio.getFila() + 2);
							pos.setColumna(posicio.getColumna() - 2);
							posicions[nPosicions++] = pos;
						}
					}
				}
			}
			else
			{
				// Primera columna: nom s dreta
				if (m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getTipusFitxa() == TIPUS_EMPTY)
				{
					pos.setFila(posicio.getFila() + 1);
					pos.setColumna(posicio.getColumna() + 1);
					posicions[nPosicions++] = pos;
				}
				else
				{
					if (
						m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getColorFitxa() == COLOR_NEGRE
						&& posicio.getFila() + 2 < N_FILES
						&& m_tauler[posicio.getFila() + 2][posicio.getColumna() + 2].getTipusFitxa() == TIPUS_EMPTY)
					{
						// Pot matar fitxa
						pos.setFila(posicio.getFila() + 2);
						pos.setColumna(posicio.getColumna() + 2);
						posicions[nPosicions++] = pos;
					}
				}
			}
		}
		break;

	case COLOR_NEGRE:
		// La fitxa es negra
		// Evitar desbordament: nom s permet capturar si hi ha espai per a fila-2
		if (posicio.getFila() != 0)
		{
			if (posicio.getColumna() != 0)
			{
				if (m_tauler[posicio.getFila() - 1][posicio.getColumna() - 1].getTipusFitxa() == TIPUS_EMPTY)
				{
					pos.setFila(posicio.getFila() - 1);
					pos.setColumna(posicio.getColumna() - 1);
					posicions[nPosicions++] = pos;
				}
				else
				{
					if (
						m_tauler[posicio.getFila() - 1][posicio.getColumna() - 1].getColorFitxa() == COLOR_BLANC
						&& posicio.getColumna() > 1
						&& posicio.getFila() - 2 >= 0
						&& m_tauler[posicio.getFila() - 2][posicio.getColumna() - 2].getTipusFitxa() == TIPUS_EMPTY)
					{
						// Pot matar fitxa
						pos.setFila(posicio.getFila() - 2);
						pos.setColumna(posicio.getColumna() - 2);
						posicions[nPosicions++] = pos;
					}
				}
			}

			if (posicio.getColumna() != N_COLUMNES - 1)
			{
				if (m_tauler[posicio.getFila() - 1][posicio.getColumna() + 1].getTipusFitxa() == TIPUS_EMPTY)
				{
					pos.setFila(posicio.getFila() - 1);
					pos.setColumna(posicio.getColumna() + 1);
					posicions[nPosicions++] = pos;
				}
				else
				{
					if (
						m_tauler[posicio.getFila() - 1][posicio.getColumna() + 1].getColorFitxa() == COLOR_BLANC
						&& posicio.getColumna() < N_COLUMNES - 2
						&& posicio.getFila() - 2 >= 0
						&& m_tauler[posicio.getFila() - 2][posicio.getColumna() + 2].getTipusFitxa() == TIPUS_EMPTY)
					{
						// Pot matar fitxa
						pos.setFila(posicio.getFila() - 2);
						pos.setColumna(posicio.getColumna() + 2);
						posicions[nPosicions++] = pos;
					}
				}
			}
		}
		break;

	default:
		break;
	}
}


void Tauler::posicionsValidesDama(const Posicio& posicio, ColorFitxa color, Posicio posicions[], int& nPosicions)
{
	nPosicions = 0;
	Posicio pos;
	// Matriu amb els despla aments
	int desplacaments[4][2] = {
		{  1,  1 }, // inferior dreta
		{  1, -1 }, // inferior esquerra
		{ -1,  1 }, // superior dreta
		{ -1, -1 }  // superior esquerra
	};

	int f0 = posicio.getFila();
	int c0 = posicio.getColumna();

	for (int i = 0; i < 4; ++i)
	{
		int sumaFila = desplacaments[i][0];
		int sumaColumna = desplacaments[i][1];
		int f1 = f0 + sumaFila;
		int c1 = c0 + sumaColumna;

		// Comprovem que no surti del tauler
		if (f1 >= 0 && f1 < N_FILES && c1 >= 0 && c1 < N_COLUMNES)
		{
			if (m_tauler[f1][c1].getTipusFitxa() == TIPUS_EMPTY) {
				pos.setFila(f1);
				pos.setColumna(c1);
				posicions[nPosicions++] = pos;
			}
			else
			{
				if (m_tauler[f1][c1].getColorFitxa() != color)
				{
					// Hi ha fitxa enemiga
					int f2 = f0 + 2 * sumaFila;
					int c2 = c0 + 2 * sumaColumna;

					if (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES
						&& m_tauler[f2][c2].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(f2);
						pos.setColumna(c2);
						posicions[nPosicions++] = pos;
					}
				}
			}

		}
	}
}

bool Tauler::mataFitxa(const Posicio& inicial, const Posicio & final, Posicio& morta)
{
	// Càlcul desplaçament files i columnes
	int despFila = final.getFila() - inicial.getFila();
	int despColumna = final.getColumna() - inicial.getColumna();

	// Només és una captura si es mou dues caselles en diagonal
	if (abs(despFila) == 2 && abs(despColumna) == 2)
	{
		// Posició de la fitxa morta (punt mig del salt)
		int filaMitja = inicial.getFila() + despFila / 2;
		int colMitja = inicial.getColumna() + despColumna / 2;

		// Assignem la posició de la fitxa capturada
		morta = Posicio(filaMitja, colMitja);
		return true;
	}
	return false;
}