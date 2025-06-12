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
	for (size_t i = 0; i < moviments.size(); ++i)
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

	vector<Posicio> fitxesEliminades;
	movimentFet.getEliminades(fitxesEliminades);
	for (size_t i = 0; i < fitxesEliminades.size(); ++i)
	{
		Fitxa fitxaEmpty(fitxesEliminades[i], TIPUS_EMPTY, COLOR_BLANC);
		afegeixFitxa(fitxaEmpty);
	}

	Fitxa novaFitxa = fitxa;
	novaFitxa.setPosicio(desti);
	if (fitxa.getTipusFitxa() == TIPUS_NORMAL)
	{
		bool promocioBlanca = (fitxa.getColorFitxa() == COLOR_BLANC && desti.getFila() == N_FILES - 1);
		bool promocioNegra = (fitxa.getColorFitxa() == COLOR_NEGRE && desti.getFila() == 0);
		if (promocioBlanca || promocioNegra)
			novaFitxa.convertirDama();
	}
	m_tauler[desti.getFila()][desti.getColumna()] = novaFitxa;
	m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

	actualitzaMovimentsValids();
	bufarFitxa(origen, desti, movimentFet, fitxa.getColorFitxa());

	return true;
}

void Tauler::bufarFitxa(const Posicio& origenMoviment,
	const Posicio& destiMoviment,
	const Moviment& movimentFet,
	ColorFitxa tornActual)
{
	int capturesFetes = movimentFet.getnFitxesEliminades();
	int damesFetes = getNumDamesCapturades(movimentFet);

	Moviment millorMovimentDeLaFitxa = getMillorMovimentDeFitxa(origenMoviment.getFila(), origenMoviment.getColumna());
	int maxCapturesDeLaFitxa = millorMovimentDeLaFitxa.getnFitxesEliminades();
	int maxDamesDeLaFitxa = getNumDamesCapturades(millorMovimentDeLaFitxa);

	if (capturesFetes < maxCapturesDeLaFitxa || (capturesFetes == maxCapturesDeLaFitxa && damesFetes < maxDamesDeLaFitxa))
	{
		afegeixFitxa(Fitxa(destiMoviment, TIPUS_EMPTY, COLOR_BLANC));
		return;
	}

	if (capturesFetes == 0)
	{
		for (int i = 0; i < N_FILES; ++i)
		{
			for (int j = 0; j < N_COLUMNES; ++j)
			{
				Posicio posActual(i, j);
				if (posActual == origenMoviment) continue;
				const Fitxa& fitxaActual = m_tauler[i][j];
				if (fitxaActual.getTipusFitxa() != TIPUS_EMPTY && fitxaActual.getColorFitxa() == tornActual)
				{
					Moviment millorMovimentLocal = getMillorMovimentDeFitxa(i, j);
					if (millorMovimentLocal.esCaptura())
					{
						afegeixFitxa(Fitxa(posActual, TIPUS_EMPTY, COLOR_BLANC));
					}
				}
			}
		}
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
	bool stopEliminades;
	bool fitxaEliminada;
	while (!movimentsPendents.empty())
	{
		stopEliminades = false;
		fitxaEliminada = false;
		movimentActual = movimentsPendents.front();
		movimentsPendents.erase(movimentsPendents.begin());
		posicioActual = movimentActual.getFinal();
		posicionsValidesNormal(posicioActual, color, posicionsValides.data(), incrementPendents);
		while (incrementPendents > 0 && !stopEliminades)
		{
			for (int i = 1; i < incrementPendents; i++)
			{
				Moviment movimentPendent = movimentActual;
				movimentPendent.afegirPosicio(posicionsValides[i]);
				if (mataFitxa(posicioActual, posicionsValides[i], eliminada))
				{
					movimentPendent.afegirFitxaEliminada(eliminada);
					movimentsPendents.push_back(movimentPendent);
				}

				moviments.push_back(movimentPendent);
			}

			movimentActual.afegirPosicio(posicionsValides[0]);
			if (mataFitxa(posicioActual, posicionsValides[0], eliminada))
			{
				movimentActual.afegirFitxaEliminada(eliminada);
				fitxaEliminada = true;
			}
			else
			{
				stopEliminades = true;
			}

			posicioActual = posicionsValides[0];
			posicionsValidesNormal(posicioActual, color, posicionsValides.data(), incrementPendents);
			if (fitxaEliminada)
			{
				if (!mataFitxa(posicioActual, posicionsValides[0], eliminada))
				{
					stopEliminades = true;
				}
			}

			if (!(movimentActual.getFinal() == movimentActual.getInici()))
			{
				moviments.push_back(movimentActual);
			}
		}
	}
}

void Tauler::trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments)
{
	moviments.clear();
	vector<Moviment> movimentsSimples;
	vector<Moviment> movimentsCaptura;

	vector<Moviment> pendents;
	Moviment movInicial(posicio);
	pendents.push_back(movInicial);

	vector<Posicio> fitxesJaCapturadesEnAquestCamí;

	int direccions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

	while (!pendents.empty())
	{
		Moviment movActual = pendents.back();
		pendents.pop_back();
		Posicio posActual = movActual.getFinal();
		movActual.getEliminades(fitxesJaCapturadesEnAquestCamí);

		bool haTrobatNovaCaptura = false;

		for (int i = 0; i < 4; ++i)
		{
			Posicio posFitxaAcapturar(-1, -1);
			int df = direccions[i][0];
			int dc = direccions[i][1];

			for (int d = 1; d < N_FILES; ++d)
			{
				int nf = posActual.getFila() + d * df;
				int nc = posActual.getColumna() + d * dc;

				if (!esPosicioValida(nf, nc)) break;

				Fitxa fitxaActual = m_tauler[nf][nc];

				if (posFitxaAcapturar.getFila() == -1)
				{
					if (fitxaActual.getTipusFitxa() == TIPUS_EMPTY)
					{
						if (movActual.getnFitxesEliminades() == 0)
						{
							Moviment movSimple = movActual;
							movSimple.afegirPosicio(Posicio(nf, nc));
							movimentsSimples.push_back(movSimple);
						}
					}
					else if (fitxaActual.getColorFitxa() != color)
					{
						posFitxaAcapturar.setPosicio(nf, nc);
					}
					else
					{
						break;
					}
				}
				else
				{
					if (fitxaActual.getTipusFitxa() == TIPUS_EMPTY)
					{
						bool jaCapturada = false;
						for (size_t j = 0; j < fitxesJaCapturadesEnAquestCamí.size(); ++j)
						{
							const Posicio& eliminada = fitxesJaCapturadesEnAquestCamí[j];
							if (eliminada == posFitxaAcapturar)
							{
								jaCapturada = true;
								break;
							}
						}
						if (!jaCapturada)
						{
							haTrobatNovaCaptura = true;
							Moviment nouMoviment = movActual;
							nouMoviment.afegirPosicio(Posicio(nf, nc));
							nouMoviment.afegirFitxaEliminada(posFitxaAcapturar);
							movimentsCaptura.push_back(nouMoviment);
							pendents.push_back(nouMoviment);
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	if (!movimentsCaptura.empty())
	{
		moviments = movimentsCaptura;
	}
	else
	{
		moviments = movimentsSimples;
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
	vector<Posicio>& posicions, int& incrementPendents,
	const vector<Posicio>& posicionsVisitades)
{
	incrementPendents = 0;
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
				{
					posicions.push_back(pos);
					incrementPendents++;
				}
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
					{
						posicions.push_back(pos);
						incrementPendents++;
					}
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