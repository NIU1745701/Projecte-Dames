#include "posicio.hpp"
#ifndef MOVIMENT_H
#define MOVIMENT_H
#define MAX_FITXES_ELIMINADES 11
#define MAX_POSICIONS 12

class Moviment
{
public:
	Moviment() { m_nPosicions = 0; m_nFitxesEliminades = 0; }
	Moviment(int fila0, int columna0);
	Moviment(const Posicio& posicioInicial);

	void afegirPosicio(const Posicio& posicio);
	void afegirFitxaEliminada(const Posicio& posicio);

	int getnPosicions() const { return m_nPosicions; }
	int getnFitxesEliminades() const { return m_nFitxesEliminades; }
	void setInici(const Posicio& inici);
	void setFinal(const Posicio & final);
	void setNFitxesEliminades(int n);
	void getInici(int& fila, int& columna) const;
	Posicio getInici() const;
	void getFinal(int& fila, int& columna) const;
	Posicio getFinal() const;
	bool esCaptura() const { return m_nFitxesEliminades > 0; }

	void getPosicions(Posicio posicions[]) const;
	void getEliminades(Posicio posicions[]) const;


private:
	Posicio m_posicions[MAX_POSICIONS];
	int m_nPosicions;

	Posicio m_fitxesEliminades[MAX_FITXES_ELIMINADES];
	int m_nFitxesEliminades;
};
#endif