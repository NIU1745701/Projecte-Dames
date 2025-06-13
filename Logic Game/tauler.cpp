#include "tauler.hpp"

Tauler::Tauler()
{
	m_tauler = new Fitxa * [N_FILES];

	for (int i = 0; i < N_FILES; i++)
	{
		m_tauler[i] = new Fitxa[N_COLUMNES];
	}
}

Tauler::Tauler(const Tauler& t)
{
	m_tauler = new Fitxa * [N_FILES];

	for (int i = 0; i < N_FILES; ++i)
	{
		m_tauler[i] = new Fitxa[N_COLUMNES];
		for (int j = 0; j < N_COLUMNES; ++j)
		{
			m_tauler[i][j] = t.m_tauler[i][j];
		}
	}
}

Tauler::~Tauler()
{
	for (int i = 0; i < N_FILES; i++)
	{
		delete[] m_tauler[i];
	}

	delete[] m_tauler;
}

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
	tauler += "   a b c d e f g h";
	return tauler;
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti, vector<Posicio>& bufada)
{
	const Fitxa* fitxaOrigenPtr = getFitxa(origen);
	if (!fitxaOrigenPtr || fitxaOrigenPtr->getTipusFitxa() == TIPUS_EMPTY)
	{
		return false;
	}
	ColorFitxa tornActual = fitxaOrigenPtr->getColorFitxa();
	actualitzaMovimentsValids();
	vector<Moviment> movimentsCandidats;
	fitxaOrigenPtr->getMovimentsValids(movimentsCandidats);
	Moviment movimentIntentat;
	bool movimentTrobats = false;
	for (size_t k = 0; k < movimentsCandidats.size(); ++k)
	{
		const Moviment& mov = movimentsCandidats[k];
		if (mov.getFinal() == desti)
		{
			movimentIntentat = mov;
			movimentTrobats = true;
			break;
		}
	}
	if (!movimentTrobats) return false;
	int maxCapturesGlobals = 0;
	int maxDamesCapturades = 0;
	for (int i = 0; i < N_FILES; ++i)
	{
		for (int j = 0; j < N_COLUMNES; ++j)
		{
			const Fitxa* fitxa = getFitxa(Posicio(i, j));
			if (fitxa && fitxa->getTipusFitxa() != TIPUS_EMPTY && fitxa->getColorFitxa() == tornActual)
			{
				vector<Moviment> moviments;
				fitxa->getMovimentsValids(moviments);
				for (size_t k = 0; k < moviments.size(); ++k)
				{
					const Moviment& mov = moviments[k];
					if (mov.getnFitxesEliminades() > maxCapturesGlobals)
					{
						maxCapturesGlobals = mov.getnFitxesEliminades();
					}
				}
			}
		}
	}
	if (maxCapturesGlobals > 0)
	{
		for (int i = 0; i < N_FILES; ++i)
		{
			for (int j = 0; j < N_COLUMNES; ++j)
			{
				const Fitxa* fitxa = getFitxa(Posicio(i, j));
				if (fitxa && fitxa->getTipusFitxa() != TIPUS_EMPTY && fitxa->getColorFitxa() == tornActual)
				{
					vector<Moviment> moviments;
					fitxa->getMovimentsValids(moviments);
					for (size_t k = 0; k < moviments.size(); ++k)
					{
						const Moviment& mov = moviments[k];
						if (mov.getnFitxesEliminades() == maxCapturesGlobals)
						{
							int damesEnMov = getNumDamesCapturades(mov);
							if (damesEnMov > maxDamesCapturades)
							{
								maxDamesCapturades = damesEnMov;
							}
						}
					}
				}
			}
		}
	}
	int capturesFetes = movimentIntentat.getnFitxesEliminades();
	bool bufadaPropia = false;

	if (maxCapturesGlobals > 0)
	{
		bool movimentLegal = (capturesFetes == maxCapturesGlobals) && (getNumDamesCapturades(movimentIntentat) >= maxDamesCapturades);
		if (!movimentLegal)
		{
			for (int i = 0; i < N_FILES; ++i)
			{
				for (int j = 0; j < N_COLUMNES; ++j)
				{
					const Fitxa* fitxa = getFitxa(Posicio(i, j));
					if (fitxa && fitxa->getTipusFitxa() != TIPUS_EMPTY && fitxa->getColorFitxa() == tornActual)
					{
						vector<Moviment> moviments;
						fitxa->getMovimentsValids(moviments);
						for (size_t k = 0; k < moviments.size(); ++k)
						{
							const Moviment& mov = moviments[k];
							if (mov.getnFitxesEliminades() == maxCapturesGlobals && getNumDamesCapturades(mov) == maxDamesCapturades)
							{
								if (!(origen == mov.getInici()))
								{
									m_tauler[i][j] = Fitxa(Posicio(i, j), TIPUS_EMPTY, COLOR_BLANC);
									Posicio posBufada(i, j);
									bufada.push_back(posBufada);
								}
								else
								{
									bufadaPropia = true;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (capturesFetes > 0) return false;
	}
	vector<Posicio> eliminades;
	movimentIntentat.getEliminades(eliminades);
	for (size_t k = 0; k < eliminades.size(); ++k)
	{
		const Posicio& posEliminada = eliminades[k];
		m_tauler[posEliminada.getFila()][posEliminada.getColumna()] = Fitxa(posEliminada, TIPUS_EMPTY, COLOR_BLANC);
	}
	Fitxa fitxaMoguda = *fitxaOrigenPtr;
	fitxaMoguda.setPosicio(desti);
	if (fitxaMoguda.getTipusFitxa() == TIPUS_NORMAL)
	{
		if ((tornActual == COLOR_BLANC && desti.getFila() == N_FILES - 1) || (tornActual == COLOR_NEGRE && desti.getFila() == 0))
		{
			fitxaMoguda.convertirDama();
		}
	}
	m_tauler[desti.getFila()][desti.getColumna()] = fitxaMoguda;
	m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(origen, TIPUS_EMPTY, COLOR_BLANC);

	if (bufadaPropia)
	{
		m_tauler[desti.getFila()][desti.getColumna()] = Fitxa(desti, TIPUS_EMPTY, COLOR_BLANC);
		bufada.push_back(desti);
	}
	return true;
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

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], vector<Moviment>& movimentsPosibles)
{
	if (m_tauler[origen.getFila()][origen.getColumna()].getTipusFitxa() != TIPUS_EMPTY)
	{
		actualitzaMovimentsValids();

		vector<Moviment> moviments;
		m_tauler[origen.getFila()][origen.getColumna()].getMovimentsValids(moviments);

		movimentsPosibles = moviments;

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
		break;

	case TIPUS_DAMA:
		trobaMovimentsValidsDama(posicio, color, moviments);
		break;

	default:
		break;
	}

	fitxa.setMovimentsValids(moviments);
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

void trobaCadenesDeCapturaNormalRec(const Tauler* tauler, const Posicio& origen, ColorFitxa color, int direccioAvanc, Moviment movActual, vector<Moviment>& movimentsResultat)
{
	int direccions[2][2] = { {direccioAvanc, 1}, {direccioAvanc, -1} };

	for (int i = 0; i < 2; i++)
	{
		Posicio intermedia(origen.getFila() + direccions[i][0], origen.getColumna() + direccions[i][1]);
		Posicio desti(origen.getFila() + 2 * direccions[i][0], origen.getColumna() + 2 * direccions[i][1]);

		if (tauler->esPosicioValida(desti.getFila(), desti.getColumna()) && tauler->esPosicioValida(intermedia.getFila(), intermedia.getColumna()))
		{
			const Fitxa* fitxaIntermedia = tauler->getFitxa(intermedia);
			const Fitxa* fitxaDesti = tauler->getFitxa(desti);

			vector<Posicio> eliminades;
			movActual.getEliminades(eliminades);
			bool jaEliminada = false;
			for (size_t k = 0; k < eliminades.size(); ++k)
			{
				const Posicio& posEliminada = eliminades[k];
				if (posEliminada == intermedia)
				{
					jaEliminada = true;
					break;
				}
			}
			if (!jaEliminada && fitxaIntermedia->getTipusFitxa() != TIPUS_EMPTY && fitxaIntermedia->getColorFitxa() != color && fitxaDesti->getTipusFitxa() == TIPUS_EMPTY)
			{
				Moviment nouMov = movActual;
				nouMov.afegirPosicio(desti);
				nouMov.afegirFitxaEliminada(intermedia);
				movimentsResultat.push_back(nouMov);
				trobaCadenesDeCapturaNormalRec(tauler, desti, color, direccioAvanc, nouMov, movimentsResultat);
			}
		}
	}
}

void trobaCadenesDeCapturaDamaRec(const Tauler* tauler, const Posicio& origen, ColorFitxa color, Moviment movActual, vector<Moviment>& movimentsResultat)
{
	int direccions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	for (int i = 0; i < 4; i++)
	{
		int df = direccions[i][0];
		int dc = direccions[i][1];
		for (int k = 1; tauler->esPosicioValida(origen.getFila() + k * df, origen.getColumna() + k * dc); k++)
		{
			Posicio posIntermedia(origen.getFila() + k * df, origen.getColumna() + k * dc);
			const Fitxa* fitxaIntermedia = tauler->getFitxa(posIntermedia);

			if (fitxaIntermedia->getTipusFitxa() != TIPUS_EMPTY)
			{
				vector<Posicio> eliminades;
				movActual.getEliminades(eliminades);
				bool jaEliminada = false;
				for (size_t j = 0; j < eliminades.size(); ++j)
				{
					const Posicio& posEliminada = eliminades[j];
					if (posEliminada == posIntermedia)
					{
						jaEliminada = true;
						break;
					}
				}

				if (!jaEliminada && fitxaIntermedia->getColorFitxa() != color)
				{
					Posicio posDesti(posIntermedia.getFila() + df, posIntermedia.getColumna() + dc);
					if (tauler->esPosicioValida(posDesti.getFila(), posDesti.getColumna()) && tauler->getFitxa(posDesti)->getTipusFitxa() == TIPUS_EMPTY)
					{
						Moviment nouMov = movActual;
						nouMov.afegirPosicio(posDesti);
						nouMov.afegirFitxaEliminada(posIntermedia);
						movimentsResultat.push_back(nouMov);
						trobaCadenesDeCapturaDamaRec(tauler, posDesti, color, nouMov, movimentsResultat);
					}
				}
				break;
			}
		}
	}
}

void Tauler::trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments)
{
	moviments.clear();
	vector<Moviment> movimentsCaptura;
	int direccioAvanc;
	if (color == COLOR_BLANC)
	{
		direccioAvanc = 1;
	}
	else
	{
		direccioAvanc = -1;
	}
	trobaCadenesDeCapturaNormalRec(this, posicio, color, direccioAvanc, Moviment(posicio), movimentsCaptura);
	if (!movimentsCaptura.empty())
	{
		moviments = movimentsCaptura;
		return;
	}
	Posicio destiEsq(posicio.getFila() + direccioAvanc, posicio.getColumna() - 1);
	if (esPosicioValida(destiEsq.getFila(), destiEsq.getColumna()) && getFitxa(destiEsq)->getTipusFitxa() == TIPUS_EMPTY)
	{
		moviments.push_back(Moviment(posicio, destiEsq));
	}
	Posicio destiDret(posicio.getFila() + direccioAvanc, posicio.getColumna() + 1);
	if (esPosicioValida(destiDret.getFila(), destiDret.getColumna()) && getFitxa(destiDret)->getTipusFitxa() == TIPUS_EMPTY)
	{
		moviments.push_back(Moviment(posicio, destiDret));
	}
}

void Tauler::trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments)
{
	moviments.clear();
	vector<Moviment> movimentsCaptura;

	trobaCadenesDeCapturaDamaRec(this, posicio, color, Moviment(posicio), movimentsCaptura);

	if (!movimentsCaptura.empty())
	{
		moviments = movimentsCaptura;
		return;
	}

	int direccions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	for (int i = 0; i < 4; i++)
	{
		int df = direccions[i][0];
		int dc = direccions[i][1];

		for (int k = 1; ; k++)
		{
			Posicio desti(posicio.getFila() + k * df, posicio.getColumna() + k * dc);
			if (esPosicioValida(desti.getFila(), desti.getColumna()) && getFitxa(desti)->getTipusFitxa() == TIPUS_EMPTY)
			{
				moviments.push_back(Moviment(posicio, desti));
			}
			else
			{
				break;
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

void Tauler::visualitza() const
{
	int j;
	int x = 0;
	int y = 0;
	for (int i = 0; i < N_FILES; i++)
	{
		for (j = 0; j < N_COLUMNES; j++)
		{
			if (m_tauler[i][j].getTipusFitxa() != TIPUS_EMPTY)
			{
				toCoordenades(j, i, x, y);
				m_tauler[i][j].visualitza(x, y);
			}
		}
	}
}

void Tauler::toCoordenades(int cX, int cY, int& x, int& y) const
{
	x = (cX * COSTAT_CASELLA) + CASELLA_INICIAL_X;
	y = ((7 - cY) * COSTAT_CASELLA) + CASELLA_INICIAL_Y;
}

const Fitxa* Tauler::getFitxa(const Posicio& pos) const
{
	int fila = pos.getFila();
	int col = pos.getColumna();
	if (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES) {
		return &m_tauler[fila][col];
	}
	return nullptr;
}

void Tauler::mouFitxaReplay(const Moviment& mov)
{
	Posicio inicial = mov.getInici();
	Posicio final = mov.getFinal();

	Fitxa fitxa = m_tauler[inicial.getFila()][inicial.getColumna()];
	Fitxa empty;

	if ((final.getFila() == 7 && fitxa.getColorFitxa() == COLOR_BLANC) ||
		(final.getFila() == 0 && fitxa.getColorFitxa() == COLOR_NEGRE))
	{
		fitxa.setTipus(TIPUS_DAMA);
	}

	m_tauler[inicial.getFila()][inicial.getColumna()] = empty;
	m_tauler[final.getFila()][final.getColumna()] = fitxa;

	vector<Posicio> eliminades;
	mov.getEliminades(eliminades);

	for (int i = 0; i < eliminades.size(); i++)
	{
		m_tauler[eliminades[i].getFila()][eliminades[i].getColumna()] = empty;
	}
}