#include <fstream>
#include<string>
#include "Fitxa.h"
#include "Moviment.h"

using namespace std;

#define N_FILES 8
#define N_COLUMNES 8

class Tauler
{
public:
	Tauler() {}
	void inicialitza(const string& nomFitxer);

	void actualitzaMovimentsValids();
	void getPosicionsPossibles(const Posicio& origen,
		int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

	void afegeixFitxa(const Fitxa& fitxa);

	void inicialitzaEmpty();

	void bufarFitxa(const Posicio& origenMoviment, const Moviment& movimentFet, ColorFitxa tornActual);
	void getPosicionsSimples(const Posicio& origen, ColorFitxa color, TipusFitxa tipus, int& nPosicions, Posicio posicions[]) const;
	bool esCaptura(const Posicio& origen, const Posicio& desti) const;
	Posicio getFitxaCapturada(const Posicio& origen, const Posicio& desti) const;
	bool esPosicioValida(int fila, int columna) const;

	int getNumDamesCapturades(const Moviment& moviment) const;
	Moviment getMillorMovimentDeFitxa(int fila, int col) const;

	//Funcions auxiliars actualitzarMovimentsValids:
	void trobaMovimentsValids(Fitxa& fitxa);
	void trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments);
	void trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments);
	void posicionsValidesNormal(const Posicio& posicio, ColorFitxa color, Posicio posicions[], int& nPosicions);
	void posicionsValidesDama(const Posicio& posicio, ColorFitxa color, Posicio posicions[], int& nPosicions);
	bool mataFitxa(const Posicio& inicial, const Posicio & final, Posicio& morta);
};