#include "GraphView.h"

void CGraphAdjMatrix::ImportGraph()
{
	ifstream Fin("InputAdjacencyMatrix.txt");
	int Number = 0;
	string Label;
	vector<CEdge*> AdjEdges;
	double Weight;
	Fin >> m_NodesNumber;
	vector<string> LabelList;

	for (int i = 0; i < m_NodesNumber; i++)
	{
		Fin >> Label;
		LabelList.push_back(Label);
	}

	for (int i = 0; i < m_NodesNumber; i++)
	{
		for (int j = 0; j < m_NodesNumber; j++)
		{
			Fin >> Weight;
			if (CEdge* Edge = ImportEdge(LabelList[j], Weight))
			{
				AdjEdges.push_back(Edge);
			}
		}
		m_Graph.insert({ LabelList[i], AdjEdges });
		AdjEdges.clear();
	}
	Fin.close();
	SetInfo();
}

void CGraphArcList::ImportGraph()
{
	ifstream Fin("InputArcList.txt");
	double Weight;
	Fin >> m_EdgeNumber;
	string LabelIn, LabelOut;

	for (int i = 0; i < m_EdgeNumber; i++)
	{
		Fin >> LabelIn >> LabelOut >> Weight;
		if (m_Graph.find(LabelIn) == m_Graph.end())
		{
			m_Graph.insert({ LabelIn, {} });
		}
		m_Graph[LabelIn].push_back(ImportEdge(LabelOut, Weight));

		if (m_Graph.find(LabelOut) == m_Graph.end())
		{
			m_Graph.insert({ LabelOut,{} });
		}
	}
	Fin.close();
	SetInfo();
}

void CGraphIncidenceMatrix::ImportGraph()
{
	ifstream Fin("InputIncidenceMatrix.txt");
	string Label;
	double Weight;
	Fin >> m_NodesNumber >> m_EdgeNumber;
	vector<string> LabelList;

	for (int i = 0; i < m_NodesNumber; i++)
	{
		Fin >> Label;
		LabelList.push_back(Label);
		m_Graph.insert({ Label, {} });
	}

	for (int i = 0; i < m_EdgeNumber; i++)
	{
		CEdge* tmpOut = nullptr;
		int tmpIn = -1;
		bool IsNode = false;
		for (int j = 0; j < m_NodesNumber; j++)
		{
			Fin >> Weight;
			if (!IsNode)
			{
				if (Weight < 0)
				{
					tmpOut = ImportEdge(LabelList[j], -Weight);
					if (tmpIn >= 0)
					{
						m_Graph[LabelList[tmpIn]].push_back(tmpOut);
						IsNode = true;
					}
				}
				else
				{
					if (Weight > 0)
					{
						if (tmpOut)
						{
							m_Graph[LabelList[j]].push_back(tmpOut);
							IsNode = true;
						}
						else
							tmpIn = j;
					}
				}
			}
		}
	}
	Fin.close();
	SetInfo();
}