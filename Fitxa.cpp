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

void Fitxa::actualitzaMovimentsValids(const Posicio& posicioInicial, const Tauler& tauler)
{
    m_nMovimentsValids = 0;

    Moviment movimentsPendents[50];
    int nPendents = 0;

    Moviment movimentInicial(posicioInicial.getFila(), posicioInicial.getColumna());
    movimentsPendents[nPendents++] = movimentInicial;

    while (nPendents > 0)
    {
        Moviment movimentActual = movimentsPendents[--nPendents]; // Sacamos el último
        Posicio posActual = movimentActual.getFinal();

        Posicio posValides[4]; // máximo 4 direcciones (diagonales)
        int nPosValides = 0;
        tauler.getPosicionsPossibles(posActual, nPosValides, posValides);

        if (nPosValides == 0) 
        {
            if (movimentActual.getNPosicions() > 1)
            {
                m_moviments[m_nMovimentsValids++] = movimentActual;
            }
        }
        else 
        {
            for (int i = 0; i < nPosValides; i++) 
            {
                Moviment nouMoviment = movimentActual; // duplicamos el actual
                nouMoviment.afegirPosicio(posValides[i]);

                // Si la nova posició implica una captura, afegir la fitxa menjada
                if (tauler.esCaptura(posActual, posValides[i])) 
                {
                    Posicio menjada = tauler.getFitxaCapturada(posActual, posValides[i]);
                    nouMoviment.AfegirFitxaEliminada(menjada);
                }
                movimentsPendents[nPendents++] = nouMoviment;
            }
        }
    }

    // Si no hay capturas múltiples, también añadimos movimientos simples
    if (m_numMoviments == 0) 
    {
        Posicio posValides[4];
        int nPosValides = 0;
        tauler.getPosicionsSimples(posicioInicial, m_color, m_tipus, nPosValides, posValides);

        for (int i = 0; i < nPosValides; i++)
        {
            Moviment m(posicioInicial);
            m.afegirPosicio(posValides[i]);
            m_moviments[m_numMoviments++] = m;
        }
    }
}

