#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "Posicio.hpp"
#include <vector>

class Moviment
{
public:
    Moviment() = default;
    Moviment(const Posicio& posicioInicial);
    Moviment(const Posicio& inici, const Posicio& fi);

    Posicio getInici() const;
    Posicio getFinal() const;
    int getnPosicions() const { return m_posicions.size(); }
    int getnFitxesEliminades() const { return m_fitxesEliminades.size(); }
    bool esCaptura() const { return !m_fitxesEliminades.empty(); }
    void getPosicions(std::vector<Posicio>& posicions) const;
    void getEliminades(std::vector<Posicio>& posicions) const;

    void afegirPosicio(const Posicio& posicio);
    void afegirFitxaEliminada(const Posicio& posicio);

private:
    vector<Posicio> m_posicions;
    vector<Posicio> m_fitxesEliminades;
};

#endif