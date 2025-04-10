#include "Posicio.h"

#define MAX_FITXES_ELIMINADES 11
#define MAX_POSICIONS 12

class Moviment
{
public:
	Moviment() { m_nPosicions = 0; m_nFitxesEliminades = 0; }
	Moviment(int fila0, int columna0) { m_nPosicions = fila0;
	m_nFitxesEliminades = columna0; }

	void AfegirPosicio(const Posicio posicio);
	void AfegirFitxaEliminada(const Posicio posicio);
	int getnPosicions() const { return m_nPosicions; }
	int getnFitxesEliminades() const { return m_nFitxesEliminades; }
	void getInici(int& fila, int& columna) const;
	void getFinal(int& fila, int& columna) const;

private:
	Posicio m_posicions[MAX_POSICIONS];
	int m_nPosicions;

	Posicio m_fitxesEliminades[MAX_FITXES_ELIMINADES];
	int m_nFitxesEliminades;
};
