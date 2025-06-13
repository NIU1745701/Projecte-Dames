#include "Fitxa.h"
#include "C:\Users\rasub\source\repos\Dames_llibreriaGrafica\0. C++ Code\Logic Game\GraphicManager.h"


Fitxa::Fitxa(Posicio posicioInicial, TipusFitxa tipus, ColorFitxa color)
    : m_posicio(posicioInicial), m_tipus(tipus), m_color(color), m_nMovimentsValids(0) {}

Fitxa::Fitxa(TipusFitxa tipus, ColorFitxa color)
    : m_tipus(tipus), m_color(color), m_nMovimentsValids(0) {}

string Fitxa::toString() const
{
    {
        if (m_tipus == TIPUS_EMPTY)
        {
            return "_";
        }

        if (m_tipus == TIPUS_NORMAL)
        {
            if (m_color == COLOR_BLANC)
            {
                return "O";
            }
            else
            {
                return "X";
            }
        }

        if (m_tipus == TIPUS_DAMA)
        {
            if (m_color == COLOR_BLANC)
            {
                return "D";
            }
            else
            {
                return "R";
            }
        }
        return "_";
    }
}

void Fitxa::getMovimentsValids(vector<Moviment>& moviments) const
{
    moviments = m_movimentsValids;
}

void Fitxa::setMovimentsValids(const vector<Moviment>& moviments)
{
    m_movimentsValids = moviments;
    m_nMovimentsValids = static_cast<int>(moviments.size());
}

istream& operator>>(istream& in, Fitxa& fitxa)
{
    char tipusChar;
    string posStr;

    if (in >> tipusChar >> posStr)
    {
        TipusFitxa tipus;
        ColorFitxa color;

        switch (tipusChar)
        {
        case 'O': tipus = TIPUS_NORMAL; color = COLOR_BLANC; break;
        case 'X': tipus = TIPUS_NORMAL; color = COLOR_NEGRE; break;
        case 'D': tipus = TIPUS_DAMA;   color = COLOR_BLANC; break;
        case 'R': tipus = TIPUS_DAMA;   color = COLOR_NEGRE; break;
        default:
            in.setstate(ios::failbit);
            return in;
        }

        Posicio pos;
        pos.fromString(posStr);

        fitxa.setTipus(tipus);
        fitxa.setColor(color);
        fitxa.setPosicio(pos);
    }

    return in;
}

void Fitxa::visualitza(int posX, int posY) const
{
    if (m_tipus != TIPUS_EMPTY)
    {
        IMAGE_NAME grafic = GRAFIC_NUM_MAX;

        switch (m_tipus)
        {
        case TIPUS_NORMAL:
            grafic = (m_color == COLOR_BLANC) ? GRAFIC_FITXA_BLANCA : GRAFIC_FITXA_NEGRA;
            break;
        case TIPUS_DAMA:
            grafic = (m_color == COLOR_BLANC) ? GRAFIC_DAMA_BLANCA : GRAFIC_DAMA_NEGRA;
            break;
        default:
            break;
        }

        GraphicManager::getInstance()->drawSprite(grafic, posX, posY);
    }    
}