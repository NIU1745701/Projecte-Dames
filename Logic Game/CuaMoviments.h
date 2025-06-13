#include "NodeMoviment.h"
#include "Moviment.h"
#include <vector>
#include <fstream>
using namespace std;

class CuaMoviments
{
public:
	CuaMoviments() { m_nMoviments = 0; m_primer = nullptr; m_ultim = nullptr; }
	CuaMoviments(const CuaMoviments& cua);
	~CuaMoviments();

	void esborra();
	void afegeixElement(const Moviment& mov);
	void eliminaFinal();
	void eliminaInici();
	Moviment getInici() const;
	Moviment getFinal() const;
	int getnMoviments() const;
	bool estaVuida() const;
	void llegeixFitxer(const string& nomFitxer);
	void escriuFitxer(const string& nomFitxer);
	void getMoviments(vector<Moviment>& vector) const;
	Moviment getMoviment(int i) const;


private:
	int m_nMoviments;
	NodeMoviment* m_primer;
	NodeMoviment* m_ultim;
};