#include "CuaMoviments.h"

CuaMoviments::CuaMoviments(const CuaMoviments& cua)
{
	m_nMoviments = cua.m_nMoviments;
	
	if (cua.m_primer == nullptr)
	{
		m_primer = nullptr;
		m_ultim = nullptr;
	}
	else
	{
		m_primer = new NodeMoviment(*cua.m_primer);
		NodeMoviment* orig = cua.m_primer;
		NodeMoviment* copia = m_primer;

		while (orig->getSeguent())
		{
			orig = orig->getSeguent();
			NodeMoviment* nodoNuevo = new NodeMoviment(*orig);
			copia->setSeguent(nodoNuevo);
			nodoNuevo->setAnterior(copia);
			copia = nodoNuevo;
		}


		m_ultim = copia;
	}
}


CuaMoviments::~CuaMoviments()
{
	NodeMoviment* iterador = m_primer;
	while (iterador != nullptr)
	{
		NodeMoviment* seguent = iterador->getSeguent();
		delete iterador;
		iterador = seguent;
	}
	m_primer = nullptr;
	m_ultim = nullptr;
}

void CuaMoviments::esborra()
{
	if (m_primer != nullptr)
	{
		delete m_primer;
		m_primer = nullptr;
		m_ultim = nullptr;
	}

	m_nMoviments = 0;
}

void CuaMoviments::afegeixElement(const Moviment& mov)
{
	if (m_ultim != nullptr)
	{
		m_ultim->setSeguent(new NodeMoviment(m_ultim, mov));
		m_ultim = m_ultim->getSeguent();
	}
	else
	{
		m_ultim = new NodeMoviment(mov);
		m_primer = m_ultim;
	}
	
	m_nMoviments++;
}

void CuaMoviments::eliminaInici()
{
	if (m_primer != nullptr)
	{
		NodeMoviment* aux;
		aux = m_primer->getSeguent();
		delete m_primer;
		m_primer = aux;
		m_nMoviments--;

		if (m_primer == nullptr)
		{
			m_ultim = nullptr;
		}
	}
}

Moviment CuaMoviments::getInici() const
{
	if (m_primer != nullptr)
	{
		return m_primer->getValor();
	}
	else
	{
		Moviment mov;
		return mov;
	}
}

Moviment CuaMoviments::getFinal() const
{
	if (m_ultim != nullptr)
	{
		return m_ultim->getValor();
	}
	else
	{
		Moviment mov;
		return mov;
	}
}

void CuaMoviments::eliminaFinal()
{
	if (m_ultim != nullptr)
	{
		NodeMoviment* aux;
		aux = m_ultim->getAnterior();
		delete m_ultim;
		m_ultim = aux;
		m_nMoviments--;

		if (m_ultim == nullptr)
		{
			m_primer = nullptr;
		}
		else
		{
			m_ultim->setSeguent(nullptr);
		}
	}
}

int CuaMoviments::getnMoviments() const
{
	return m_nMoviments;
}

bool CuaMoviments::estaVuida() const
{
	return m_nMoviments == 0;
}

void CuaMoviments::llegeixFitxer(const string& nomFitxer)
{
	esborra();
	
	int nMoviments;
	int nPos;
	int nEliminades;
	Posicio pos;
	
	ifstream fitxer;
	fitxer.open(nomFitxer);

	fitxer >> nMoviments;

	int j;
	int i = 0;

	while(i<nMoviments && !fitxer.eof())
	{
		Moviment mov;

		fitxer >> nPos;

		for (j = 0; j < nPos; j++)
		{
			fitxer >> pos;
			mov.afegirPosicio(pos);
		}

		fitxer >> nEliminades;

		for (j = 0; j < nEliminades; j++)
		{
			fitxer >> pos;
			mov.afegirFitxaEliminada(pos);
		}

		afegeixElement(mov);

		i++;
	}

	fitxer.close();
}

void CuaMoviments::escriuFitxer(const string& nomFitxer)
{
	ofstream fitxer;
	fitxer.open(nomFitxer);

	vector<Moviment> m_moviments;
	getMoviments(m_moviments);
	vector<Posicio> posicions;
	vector<Posicio> eliminades;
	int nPosicions;
	int nEliminades;
	string pos;

	fitxer << m_nMoviments << endl;
	
	int j;
	for (int i = 0; i < m_nMoviments; i++)
	{
		m_moviments[i].getPosicions(posicions);
		m_moviments[i].getEliminades(eliminades);
		nPosicions = m_moviments[i].getnPosicions();
		nEliminades = m_moviments[i].getnFitxesEliminades();

		fitxer << nPosicions << " ";

		for (j = 0; j < nPosicions; j++)
		{
			posicions[j].ToString(pos);
			fitxer << pos << " ";
		}

		fitxer << nEliminades;

		if (nEliminades > 0)
		{
			fitxer << " ";
			
			for (j = 0; j < nEliminades - 1; j++)
			{
				eliminades[j].ToString(pos);
				fitxer << pos << " ";
			}

			eliminades[j].ToString(pos);
			fitxer << pos;
		}

		fitxer << endl;
	}

	fitxer.close();
}

void CuaMoviments::getMoviments(vector<Moviment>& vector) const
{
	vector.clear();
	
	if (m_primer != nullptr)
	{
		NodeMoviment* aux = m_primer;

		for (int i = 0; i < m_nMoviments; i++)
		{
			vector.push_back(aux->getValor());
			aux = aux->getSeguent();
		}
	}
}

Moviment CuaMoviments::getMoviment(int i) const
{
	Moviment mov;
	
	if (i >= 0 && i < m_nMoviments)
	{
		if (m_primer != nullptr)
		{
			NodeMoviment* aux = m_primer;

			for (int j = 0; j < i; j++)
			{
				aux = aux->getSeguent();
			}

			mov = aux->getValor();
		}
	}
	
	return mov;
}