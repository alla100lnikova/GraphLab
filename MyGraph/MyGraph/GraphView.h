#pragma once
#include "CGraphBase.h"

class CGraphAdjMatrix : public CGraphBase
{
public:
	void ImportGraph();
};

class CGraphIncidenceMatrix : public CGraphBase
{
public:
	void ImportGraph();
};

class CGraphArcList : public CGraphBase
{
public:
	void ImportGraph();
};