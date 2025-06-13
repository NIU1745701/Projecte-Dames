#ifndef TAULER_HPP
#define TAULER_HPP

#include "Fitxa.h"
#include "Moviment.h"
#include <vector>
#include "info_joc.hpp"

#define N_FILES 8
#define N_COLUMNES 8

#define COSTAT_CASELLA 62

class Tauler
{
public:
    Tauler();
    Tauler(const Tauler& t);
    ~Tauler();
    void inicialitza(const std::string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], vector<Moviment>& movimentsPosibles);
    bool mouFitxa(const Posicio& origen, const Posicio& desti, vector<Posicio>& bufada);
    std::string toString() const;
    bool esPosicioValida(int fila, int columna) const;
    void visualitza() const;
    const Fitxa* getFitxa(const Posicio& pos) const;
    void mouFitxaReplay(const Moviment& mov);

private:
    Fitxa** m_tauler;
    void afegeixFitxa(const Fitxa& fitxa);
    void inicialitzaEmpty();
    void getPosicionsSimples(const Posicio& origen, ColorFitxa color, TipusFitxa tipus, int& nPosicions, Posicio posicions[]) const;
    bool esCaptura(const Posicio& origen, const Posicio& desti) const;
    Posicio getFitxaCapturada(const Posicio& origen, const Posicio& desti) const;
    int getNumDamesCapturades(const Moviment& moviment) const;
    Moviment getMillorMovimentDeFitxa(int fila, int col) const;
    void trobaMovimentsValids(Fitxa& fitxa);
    void trobaMovimentsValidsNormal(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments);
    void trobaMovimentsValidsDama(const Posicio& posicio, ColorFitxa color, vector<Moviment>& moviments);
    void posicionsValidesNormal(const Posicio& posicio, ColorFitxa color, Posicio posicions[], int& nPosicions);
    bool mataFitxa(const Posicio& inicial, const Posicio & final, Posicio& morta);
    void toCoordenades(int cX, int cY, int& x, int& y) const;
};

#endif
