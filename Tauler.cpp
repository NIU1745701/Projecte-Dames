#include "tauler.hpp"

void Tauler::afegeixFitxa(const Fitxa& fitxa)
{
	Posicio posicio;
	posicio = fitxa.getPosicio();
	m_tauler[posicio.getFila()][posicio.getColumna()] = fitxa;
}

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

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxa.getTipusFitxa() == TIPUS_EMPTY)
		return false;
	vector<Moviment> moviments;
	fitxa.getMovimentsValids(moviments);
	Moviment movimentFet;
	bool movimentValid = false;
	Posicio posicioFinal;
	for (size_t i = 0; i < moviments.size(); ++i)
	{
		posicioFinal = moviments[i].getFinal();
		if (posicioFinal == desti)
		{
			movimentFet = moviments[i];
			movimentValid = true;
			break;
		}
	}
	if (!movimentValid)
		return false;

	vector<Posicio> fitxesEliminades;
	movimentFet.getEliminades(fitxesEliminades);
	for (size_t i = 0; i < fitxesEliminades.size(); i++)
	{
		Fitxa fitxaEmpty(fitxesEliminades[i], TIPUS_EMPTY, COLOR_BLANC);
		afegeixFitxa(fitxaEmpty);
	}
	m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
	m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
	if (fitxa.getTipusFitxa() == TIPUS_NORMAL)
	{
		if ((fitxa.getColorFitxa() == COLOR_BLANC && desti.getFila() == 0) ||
			(fitxa.getColorFitxa() == COLOR_NEGRE && desti.getFila() == 7))
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
		Fitxa fitxaEmpty(millorFitxa, TIPUS_EMPTY, COLOR_BLANC);
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
	vector<Moviment> moviments;
	fitxa.getMovimentsValids(moviments);

	for (size_t i = 0; i < moviments.size(); i++)
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
		return Posicio(-1, -1);
	}
}
bool Tauler::esPosicioValida(int fila, int columna) const
{
	return fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES;
}

int Tauler::getNumDamesCapturades(const Moviment& moviment) const
{
	int numDames = 0;
	vector<Posicio> eliminades;
	moviment.getEliminades(eliminades);

	for (size_t i = 0; i < eliminades.size(); i++)
	{
		const Posicio& pos = eliminades[i];
		if (m_tauler[pos.getFila()][pos.getColumna()].getTipusFitxa() == TIPUS_DAMA)
		{
			numDames++;
		}
	}
	return numDames;
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	if (m_tauler[origen.getFila()][origen.getColumna()].getTipusFitxa() != TIPUS_EMPTY)
	{
		actualitzaMovimentsValids();

		vector<Moviment> moviments;
		m_tauler[origen.getFila()][origen.getColumna()].getMovimentsValids(moviments);

		nPosicions = static_cast<int>(moviments.size());

		for (int i = 0; i < nPosicions; i++)
		{
			Posicio final = moviments[i].getFinal();
			posicionsPossibles[i] = final;
		}
	}
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
void Tauler::trobaMovimentsValids(Fitxa& fitxa)
{
	Posicio posicio = fitxa.getPosicio();
	vector<Moviment> moviments;
	TipusFitxa tipus = fitxa.getTipusFitxa();
	ColorFitxa color = fitxa.getColorFitxa();
	switch (tipus)
	{
	case TIPUS_NORMAL:
		trobaMovimentsValidsNormal(posicio, color, moviments);
		fitxa.setMovimentsValids(moviments);
		break;

	case TIPUS_DAMA:
		trobaMovimentsValidsDama(posicio, color, moviments);
		fitxa.setMovimentsValids(moviments);
		break;

	default:
		break;
	}
}

void Tauler::trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments)
{
	vector<Moviment> movimentsPendents;
	Moviment movimentActual;
	movimentActual.afegirPosicio(posicio);
	movimentsPendents.push_back(movimentActual);
	Posicio posicioActual;
	vector<Posicio> posicionsValides(2);
	int incrementPendents = 0;
	Posicio eliminada;
	while (!movimentsPendents.empty())
	{
		movimentActual = movimentsPendents.front();
		movimentsPendents.erase(movimentsPendents.begin());
		posicioActual = movimentActual.getFinal();
		posicionsValidesNormal(posicioActual, color, posicionsValides.data(), incrementPendents);
		while (incrementPendents > 0)
		{
			movimentActual.afegirPosicio(posicionsValides[0]);
			if (mataFitxa(posicioActual, posicionsValides[0], eliminada))
			{
				movimentActual.afegirFitxaEliminada(eliminada);
			}
			for (int i = 1; i < incrementPendents; ++i)
			{
				Moviment movimentPendent = movimentActual;
				movimentPendent.afegirPosicio(posicionsValides[i]);
				if (mataFitxa(posicioActual, posicionsValides[i], eliminada))
				{
					movimentPendent.afegirFitxaEliminada(eliminada);
				}
				movimentsPendents.push_back(movimentPendent);
			}
			posicioActual = posicionsValides[0];
			posicionsValidesNormal(posicioActual, color, posicionsValides.data(), incrementPendents);
		}

		if (!(movimentActual.getFinal() == movimentActual.getInici()))
		{
			moviments.push_back(movimentActual);
		}
	}
}

void Tauler::trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments)
{
	moviments.clear();
	vector<Moviment> movimentsPendents;
	vector<Posicio> posicionsVisitades;
	Moviment movimentInicial;
	movimentInicial.afegirPosicio(posicio);
	movimentsPendents.push_back(movimentInicial);

	while (!movimentsPendents.empty())
	{
		Moviment movimentActual = movimentsPendents.back();
		movimentsPendents.pop_back();

		Posicio posicioActual = movimentActual.getFinal();
		bool jaVisitada = false;
		for (size_t i = 0; i < posicionsVisitades.size(); ++i)
		{
			if (posicionsVisitades[i] == posicioActual)
			{
				jaVisitada = true;
				break;
			}
		}
		if (jaVisitada)
			continue;
		posicionsVisitades.push_back(posicioActual);
		vector<Posicio> posicionsValides;
		posicionsValidesDama(posicioActual, color, posicionsValides, posicionsVisitades);

		if (!posicionsValides.empty())
		{
			for (size_t i = 0; i < posicionsValides.size(); ++i)
			{
				Posicio novaPosicio = posicionsValides[i];
				Moviment nouMoviment = movimentActual;
				nouMoviment.afegirPosicio(novaPosicio);

				Posicio fitxaEliminada;
				if (mataFitxa(posicioActual, novaPosicio, fitxaEliminada))
				{
					nouMoviment.afegirFitxaEliminada(fitxaEliminada);
				}
				movimentsPendents.push_back(nouMoviment);
			}
		}
		else
			if (!(movimentActual.getFinal() == movimentActual.getInici()))
			{
				moviments.push_back(movimentActual);
			}
	}
}


void Tauler::posicionsValidesNormal(const Posicio& posicio, ColorFitxa color, Posicio posicions[], int& nPosicions)
{
	nPosicions = 0;
	Posicio pos;

	switch (color)
	{
	case COLOR_BLANC:

		if (posicio.getFila() < N_FILES - 1)
		{

			if (posicio.getColumna() > 0)
			{
				if (posicio.getColumna() < N_COLUMNES - 1)
				{

					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 1);
						pos.setColumna(posicio.getColumna() - 1);
						posicions[nPosicions++] = pos;
					}
					else
					{
						if (m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getColorFitxa() == COLOR_NEGRE && posicio.getColumna() > 1 && posicio.getFila() + 2 < N_FILES && m_tauler[posicio.getFila() + 2][posicio.getColumna() - 2].getTipusFitxa() == TIPUS_EMPTY)
						{

							pos.setFila(posicio.getFila() + 2);
							pos.setColumna(posicio.getColumna() - 2);
							posicions[nPosicions++] = pos;
						}
					}


					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 1);
						pos.setColumna(posicio.getColumna() + 1);
						posicions[nPosicions++] = pos;
					}
					else
					{
						if (m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getColorFitxa() == COLOR_NEGRE && posicio.getColumna() < N_COLUMNES - 2 && posicio.getFila() + 2 < N_FILES && m_tauler[posicio.getFila() + 2][posicio.getColumna() + 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							pos.setFila(posicio.getFila() + 2);
							pos.setColumna(posicio.getColumna() + 2);
							posicions[nPosicions++] = pos;
						}
					}
				}
				else
				{
					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 1);
						pos.setColumna(posicio.getColumna() - 1);
						posicions[nPosicions++] = pos;
					}
					else
					{
						if (m_tauler[posicio.getFila() + 1][posicio.getColumna() - 1].getColorFitxa() == COLOR_NEGRE && posicio.getFila() + 2 < N_FILES && m_tauler[posicio.getFila() + 2][posicio.getColumna() - 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							pos.setFila(posicio.getFila() + 2);
							pos.setColumna(posicio.getColumna() - 2);
							posicions[nPosicions++] = pos;
						}
					}
				}
			}
			else
			{
				if (m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getTipusFitxa() == TIPUS_EMPTY)
				{
					pos.setFila(posicio.getFila() + 1);
					pos.setColumna(posicio.getColumna() + 1);
					posicions[nPosicions++] = pos;
				}
				else
				{
					if (m_tauler[posicio.getFila() + 1][posicio.getColumna() + 1].getColorFitxa() == COLOR_NEGRE && posicio.getFila() + 2 < N_FILES && m_tauler[posicio.getFila() + 2][posicio.getColumna() + 2].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() + 2);
						pos.setColumna(posicio.getColumna() + 2);
						posicions[nPosicions++] = pos;
					}
				}
			}
		}
		break;

	case COLOR_NEGRE:
		if (posicio.getFila() < N_FILES - 1)
		{
			if (posicio.getFila() > 0)
			{
				if (posicio.getColumna() > 0)
				{
					if (m_tauler[posicio.getFila() - 1][posicio.getColumna() - 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() - 1);
						pos.setColumna(posicio.getColumna() - 1);
						posicions[nPosicions++] = pos;
					}
					else
						if (m_tauler[posicio.getFila() - 1][posicio.getColumna() - 1].getColorFitxa() == COLOR_BLANC && posicio.getColumna() > 1 && posicio.getFila() > 1 && m_tauler[posicio.getFila() - 2][posicio.getColumna() - 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							pos.setFila(posicio.getFila() - 2);
							pos.setColumna(posicio.getColumna() - 2);
							posicions[nPosicions++] = pos;
						}
				}
				if (posicio.getColumna() < N_COLUMNES - 1)
				{
					if (m_tauler[posicio.getFila() - 1][posicio.getColumna() + 1].getTipusFitxa() == TIPUS_EMPTY)
					{
						pos.setFila(posicio.getFila() - 1);
						pos.setColumna(posicio.getColumna() + 1);
						posicions[nPosicions++] = pos;
					}
					else
						if (m_tauler[posicio.getFila() - 1][posicio.getColumna() + 1].getColorFitxa() == COLOR_BLANC && posicio.getColumna() < N_COLUMNES - 2 && posicio.getFila() > 1 && m_tauler[posicio.getFila() - 2][posicio.getColumna() + 2].getTipusFitxa() == TIPUS_EMPTY)
						{
							pos.setFila(posicio.getFila() - 2);
							pos.setColumna(posicio.getColumna() + 2);
							posicions[nPosicions++] = pos;
						}
				}
			}
			break;

	default:
		break;
		}
	}
}

void Tauler::posicionsValidesDama(const Posicio& posicio, ColorFitxa color,
	vector<Posicio>& posicions,
	const vector<Posicio>& posicionsVisitades)
{
	posicions.clear();
	Posicio pos;

	int desplacaments[4][2] =
	{
		{ 1, 1 }, { 1, -1 },
		{ -1, 1 }, { -1, -1 }
	};

	int f0 = posicio.getFila();
	int c0 = posicio.getColumna();

	for (int i = 0; i < 4; ++i)
	{
		int dr = desplacaments[i][0];
		int dc = desplacaments[i][1];

		int f1 = f0 + dr;
		int c1 = c0 + dc;

		if (f1 >= 0 && f1 < N_FILES && c1 >= 0 && c1 < N_COLUMNES)
		{
			if (m_tauler[f1][c1].getTipusFitxa() == TIPUS_EMPTY)
			{
				pos.setFila(f1);
				pos.setColumna(c1);

				bool visitada = false;
				for (size_t j = 0; j < posicionsVisitades.size(); ++j)
				{
					if (posicionsVisitades[j] == pos)
					{
						visitada = true;
						break;
					}
				}

				if (!visitada)
					posicions.push_back(pos);
			}
			else if (m_tauler[f1][c1].getColorFitxa() != color)
			{
				int f2 = f1 + dr;
				int c2 = c1 + dc;

				if (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES &&
					m_tauler[f2][c2].getTipusFitxa() == TIPUS_EMPTY)
				{
					pos.setFila(f2);
					pos.setColumna(c2);

					bool visitada = false;
					for (size_t j = 0; j < posicionsVisitades.size(); ++j)
					{
						if (posicionsVisitades[j] == pos)
						{
							visitada = true;
							break;
						}
					}
					if (!visitada)
						posicions.push_back(pos);
				}
			}
		}
	}
}

bool Tauler::mataFitxa(const Posicio& inicial, const Posicio & final, Posicio& morta)
{
	int despFila = final.getFila() - inicial.getFila();
	int despColumna = final.getColumna() - inicial.getColumna();

	if (abs(despFila) == 2 && abs(despColumna) == 2)
	{
		int filaMitja = inicial.getFila() + despFila / 2;
		int colMitja = inicial.getColumna() + despColumna / 2;
		morta = Posicio(filaMitja, colMitja);
		return true;
	}
	return false;
}