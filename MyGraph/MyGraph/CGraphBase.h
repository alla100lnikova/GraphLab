#pragma once
#include "CNode.h"
#include "CEdge.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "VirtStruct.h"
using namespace std;

struct Edge //завела чисто для абс цетра такую структуру ребра
{
	int begin;
	int end;
};

struct Center //ну и сам центр типо так выглядит
{
	Edge reb; //на каком ребре
	int f; //расстояние от левой границы ребра, храню как целую величину, чтобы в цикле идти по ней без погрешности
	float k; //это расстояние до максимально удаленной от этого центра вершины 
};

class CGraphBase
{
private:
	vector <int> match, p, base, q;
	vector <bool> used, blossom;
	vector <vector<int>> g;
	int Count;

protected:
	int m_NodesNumber;
	int m_EdgeNumber;
	int m_Degree;
	double m_Diameter;
	double m_Radius;
	int m_CyclicNumber;
	int m_ConnectedComponentNumber;
	bool m_bIsOriented;
	map<string, vector<CEdge*>> m_Graph;

public:
	vector<vector<string>> Cycles;
	vector<string> Centers;

private:
	void Define();
	vector<vector<int>> Transform();
	vector<int> Start();
	int FindPath(int root);
	void MarkPath(int v, int b, int children);
	int lca(int a, int b);
	int CalcEdgeNumber();
	void FillCenters(vector<double>, double min);
	double CalcRadiusOrDiameter(bool IsDiameter = false);
	double ** GraphToIntMatrix(bool IsDiameter);
	double ** DistanceMatrix(bool IsDiameter);
	map<int, string> NumberToNode();
	map<string, int> NodeToNumber();
	void AddEdge(map<string, vector<CEdge*>>& Graph, string NodeBegin, string NodeEnd, double Weight);
	void AddOrientEdges(map<string, vector<CEdge*>>& Graph);
	void DFS(map<string, vector<CEdge*>> Graph, string Node, vector<int>& used, map<string, int> NodeNumbers, vector<string>& Cycle, bool NeedPrint = false);

protected:
	int CalcDegree();
	double CalcDiameter();
	double CalcRadius();
	int CalcCyclicNumber();
	int CalcConnectedComponentNumber();

public:
	CGraphBase();
	~CGraphBase();
	//ВАЖНО! После вызова позвать сетинфо и принт
	void AddNode(string Label);
	void DeleteNode(string Node);
	void AddEdge(string NodeBegin, string NodeEnd, double Weight);
	void DeleteEdge(string NodeBegin, string NodeEnd);
	double IsThereEdge(string NodeBegin, string NodeEnd);
	vector<CEdge*> AdjacentNodeList(string NodeBegin);
	bool IsTree();
	void PrintGraph();
	void GetInfo();
	bool IsOriented() { return m_bIsOriented; }
	int GetNodeDegree(string Node);
	void EditOrient();
	void DeleteGraph();
	void DFS();
	void AbsCenter();
	void Matching();
	void SetInfo();

public:
	virtual void ImportGraph();
	virtual CEdge* ImportEdge(string Label, double Weight);
};