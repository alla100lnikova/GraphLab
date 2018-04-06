#pragma once
#include "CNode.h"

class CEdge
{
private:
	CNode* m_NodeEnd;
	double m_Weight;
public:
	CEdge(CNode* NodeEnd, double Weight);
	CEdge();
	~CEdge();
	double GetWeight();
	CNode* GetEndNode() { return m_NodeEnd; }
};

