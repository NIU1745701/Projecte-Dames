#include <fstream>
#include "Fitxa.h"
#include "Moviment.h"

using namespace std;

#define N_FILES 8
#define N_COLUMNES 8
#define MAX_MOVIMENTS 20

class Tauler
{
public:
	Tauler() {}
	void inicialitza(const string& nomFitxer);

	void actualitzaMovimentsValids();
	void getPosicionsPossibles(const Posicio& origen,int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;
	void bufarFitxa(ColorFitxa tornActual);
	void getPosicionsSimples(const Posicio& origen, ColorFitxa color, TipusFitxa tipus, int& nPosicions, Posicio posicions[]) const;
	bool esCaptura(const Posicio& origen, const Posicio& desti) const;
	Posicio getFitxaCapturada(const Posicio& origen, const Posicio& desti) const;
	bool Tauler::esPosicioValida(int fila, int columna) const;
private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

	void afegeixFitxa(const Fitxa& fitxa, const Posicio& posicio);

	//Funcions auxiliars actualitzarMovimentsValids:
	void trobaMovimentsValids(Fitxa& fitxa);
	void trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments);
	void trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, Moviment moviments[], int& nMoviments);
};