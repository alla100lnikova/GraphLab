#include "CEdge.h"

CEdge::CEdge(CNode* NodeEnd, double Weight)
	:m_Weight(Weight),
	m_NodeEnd(NodeEnd)
{
}

CEdge::CEdge()
{
}

CEdge::~CEdge()
{
	delete m_NodeEnd;
}

double CEdge::GetWeight()
{
	return m_Weight;
}
