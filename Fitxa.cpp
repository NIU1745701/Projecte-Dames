#include "Fitxa.h"

Fitxa::Fitxa(Posicio posicioInicial, TipusFitxa tipus, ColorFitxa color)
{
	m_posicio = posicioInicial;
	m_tipus = tipus;
	m_color = color;
	m_nMovimentsValids = 0;
}

Fitxa::Fitxa(TipusFitxa tipus, ColorFitxa color)
{
	m_tipus = tipus;
	m_color = color;
	m_nMovimentsValids = 0;
}

void Fitxa::movimentsValids(Moviment movimentsValids[], int nMoviments) const
{

}

