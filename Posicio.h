#include <iostream>
using namespace std;

#define N_FILES 7
#define N_COLUMNES 7

class Posicio
{
public:
	Posicio() { m_fila = 0; m_columna = 0; }
	Posicio(const string& posicio);

	int getFila() const { return m_fila; }
	int getColumna() const { return m_columna; }
	void setFila(int fila) { m_fila = fila; }
	void setColumna(int columna) { m_columna = columna; }
	void setPosicio(int fila, int columna) { m_fila = fila; m_columna = columna; }

	bool operator==(const Posicio& posicio) const;
	void stringToPosicio(const string& posicio);
	void posicioToString(string& posicio) const;
	
private:
	int m_fila;
	int m_columna;
};
