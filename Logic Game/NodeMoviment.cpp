#include "NodeMoviment.h"

NodeMoviment::NodeMoviment(const NodeMoviment& node)
{
	m_moviment = node.m_moviment;

	m_seguent = nullptr;
	m_anterior = nullptr;
}