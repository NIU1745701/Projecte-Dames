#ifndef FITXA_H
#define FITXA_H
#include "posicio.hpp"
#include "Moviment.h"
#include <fstream>
#include <string>
#define MAX_MOVIMENTS 12

typedef enum
{
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
	COLOR_NEGRE,
	COLOR_BLANC,
} ColorFitxa;

class Fitxa
{
public:
	Fitxa() { m_nMovimentsValids = 0; m_color = COLOR_BLANC; m_tipus = TIPUS_EMPTY; }
	Fitxa(Posicio posicioInicial, TipusFitxa tipus, ColorFitxa color);
	Fitxa(TipusFitxa tipus, ColorFitxa color);

	Posicio getPosicio() const { return m_posicio; }
	TipusFitxa getTipusFitxa() const { return m_tipus; }
	ColorFitxa getColorFitxa() const { return m_color; }
	int getNMovimentsValids() const { return m_nMovimentsValids; }
	void getMovimentsValids(Moviment moviments[]) const;
	string toString() const;
	void setPosicio(const Posicio& posicio) { m_posicio = posicio; }
	void setPosicio(int fila, int columna) { m_posicio.setFila(fila); m_posicio.setColumna(columna); }
	void setTipus(const TipusFitxa& tipus) { m_tipus = tipus; }
	void setColor(const ColorFitxa& color) { m_color = color; }
	void convertirDama() { m_tipus = TIPUS_DAMA; }
	void setMovimentsValids(const Moviment movimentsValids[], int nMoviments);


private:
	Posicio m_posicio;
	TipusFitxa m_tipus;
	ColorFitxa m_color;

	Moviment m_movimentsValids[MAX_MOVIMENTS];
	int m_nMovimentsValids;
};

istream& operator>>(istream& input, Fitxa& fitxa);

#endif