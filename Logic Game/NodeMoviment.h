#include "Moviment.h"

class NodeMoviment
{
public:
	NodeMoviment() { m_seguent = nullptr; m_anterior = nullptr; }
	NodeMoviment(const NodeMoviment& node);
	NodeMoviment(const Moviment& mov) { m_seguent = nullptr; m_anterior = nullptr; m_moviment = mov; }
	NodeMoviment(NodeMoviment* anterior) { m_seguent = nullptr; m_anterior = anterior; }
	NodeMoviment(NodeMoviment* anterior, const Moviment& mov) { m_seguent = nullptr; m_anterior = anterior; m_moviment = mov; }

	void setSeguent(NodeMoviment* seguent) { m_seguent = seguent; }
	void setAnterior(NodeMoviment* anterior) { m_anterior = anterior; }
	void setValor(const Moviment& mov) { m_moviment = mov; }

	Moviment getValor() const { return m_moviment; }
	NodeMoviment* getSeguent() const { return m_seguent; }
	NodeMoviment* getAnterior() const { return m_anterior; }

private:
	Moviment m_moviment;
	NodeMoviment* m_seguent;
	NodeMoviment* m_anterior;
};