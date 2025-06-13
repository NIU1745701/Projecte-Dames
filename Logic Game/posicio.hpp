#ifndef POSICIO_HPP
#define POSICIO_HPP

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Posicio
{
public:
    Posicio() : m_fila(0), m_columna(0) {}
    Posicio(const string& posicio);
    Posicio(int fila, int columna) : m_fila(fila), m_columna(columna) {}

    int getFila() const { return m_fila; }
    int getColumna() const { return m_columna; }
    void setFila(int fila) { m_fila = fila; }
    void setColumna(int columna) { m_columna = columna; }
    void setPosicio(int fila, int columna) { m_fila = fila; m_columna = columna; }

    bool operator==(const Posicio& posicio) const;
    void fromString(const string& posicio);
    void ToString(string& posicio) const;

private:
    int m_fila;
    int m_columna;
};

ifstream& operator>>(ifstream& fitxer, Posicio& posicio);
ostream& operator<<(ostream& os, const Posicio& pos);

#endif 
