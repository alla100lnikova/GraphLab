#include <iostream> 
#include <fstream>
#include "GraphView.h"
#include <Windows.h>
using namespace std;

enum eInputType
{
	itAdjList = 1,
	itAdjMatr = 2,
	itIncMatr = 3,
	itArcList = 4
};

enum eOperation
{
	oExit = 0,
	oAddNode = 1,
	oDelNode = 2,
	oAddEdge = 3,
	oDelEdge = 4,
	oEditOrient = 5,
	oGetInfo = 6,
	oDfs = 7,
	oPrintGraph = 8,
	oIsThereEdge = 9,
	oIsTree = 10,
	oGetNodeDegree = 11,
	oGetAdjNodeList = 12,
	oIsThereNode = 13,
	oImport = 14,
	oIsOriented = 15,
	oPrintOpList = 16
};

CGraphBase Graph;

void ImportQuestion()
{
	cout << "\n����� ������������� ����� �������������?\n";
	cout << "1. ������ ���������\n";
	cout << "2. ������� ���������\n";
	cout << "3. ������� �������������\n";
	cout << "4. ������ ���\n";
	cout << "�����: ";
}

void OperQuestion(bool IsStart)
{
	cout << "\n�������� �������� (16 - �������� ������ ��������):\n";
	if (IsStart)
	{
		cout << "1. �������� �������\n";
		cout << "2. ������� �������\n";
		cout << "3. �������� �����\n";
		cout << "4. ������� �����\n";
		cout << "5. �������� ����������������� �����\n";
		cout << "6. ������� ����������\n";
		cout << "7. ������� ������� � ������� ������ � �������\n";
		cout << "8. ���������� ����\n";
		cout << "9. ��������� ������� �����\n";
		cout << "10. ���������, �������� �� ���� �������\n";
		cout << "11. ������ ������� �������\n";
		cout << "12. �������� ������ ������� ������ ��� �������� �������\n";
		cout << "13. ��������� ������� �������\n";
		cout << "14. ������ ������ �����\n";
		cout << "15. ������ ����������������� �����\n";
		cout << "0. �����\n";
	}
	cout << "�����: ";
}

CGraphBase ImportDialog(bool IsStart = false)
{
	if (IsStart)
		ImportQuestion();
	else
		cout << "\n����� ������������� ����� �������������?\n�����: ";

	string Imp;
	cin >> Imp;
	eInputType Inp = (eInputType)atoi(Imp.c_str());

	switch (Inp)
	{
		case itAdjList:
		{
			CGraphBase Graph1;
			Graph1.ImportGraph();
			return Graph1;
		}
		case itAdjMatr:
		{
			CGraphAdjMatrix Graph2;
			Graph2.ImportGraph();
			return Graph2;
		}
		case itArcList:
		{
			CGraphArcList Graph3;
			Graph3.ImportGraph();
			return Graph3;
		}
		case itIncMatr:
		{
			CGraphIncidenceMatrix Graph4;
			Graph4.ImportGraph();
			return Graph4;
		}
		default:
			return ImportDialog();
	}
}

void GetCharacters(CGraphBase Graph)
{
	Graph.GetInfo();

	if (!Graph.IsOriented())
	{
		Graph.AbsCenter();
		Graph.Matching();
	}
}

void DoTest()
{
	try
	{
		Graph = ImportDialog(true);
		eOperation Op;
		bool IsStart = true;
		while (true)
		{
			OperQuestion(IsStart);
			IsStart = false;
			string Oper;
			cin >> Oper;
			Op = (eOperation) atoi(Oper.c_str());
			string Label, LabelOut;
			double Weight;
			switch (Op)
			{
				case oExit:
					return;
				case oAddNode:
					cout << "\n������� ����� �������: ";
					cin >> Label;
					Graph.AddNode(Label);
					Graph.PrintGraph();
					break;
				case oDelNode:
					cout << "\n������� ����� �������: ";
					cin >> Label;
					Graph.DeleteNode(Label);
					Graph.PrintGraph();
					break;
				case oAddEdge:
					cout << "\n������� ����� ������ ����� ������� - ������, ����� ������� - ���� � ���: ";
					cin >> Label >> LabelOut >> Weight;
					Graph.AddEdge(Label, LabelOut, Weight);
					Graph.SetInfo();
					Graph.PrintGraph();
					break;
				case oDelEdge:
					cout << "\n������� ����� ������ ����� ������� - ������ � ����� ������� - ����: ";
					cin >> Label >> LabelOut;
					Graph.DeleteEdge(Label, LabelOut);
					Graph.PrintGraph();
					break;
				case oEditOrient:
					Graph.EditOrient();
					Graph.PrintGraph();
					break;
				case oGetInfo:
					GetCharacters(Graph);
					break;
				case oDfs:
					Graph.DFS();
					break;
				case oPrintGraph:
					Graph.PrintGraph();
					break;
				case oIsThereEdge:
					cout << "\n������� ����� ������ ����� ������� - ������ � ����� ������� - ����: ";
					cin >> Label >> LabelOut;
					if (Graph.IsThereEdge(Label, LabelOut))
						cout << "����� ����������.\n";
					else cout << "����� �� ����������.\n";
					break;
				case oIsTree:
					if (Graph.IsTree())
						cout << "���� �������� �������.\n";
					else cout << "���� �� ������.\n";
					break;
				case oGetNodeDegree:
					cout << "\n������� ����� �������: ";
					cin >> Label;
					cout << Graph.GetNodeDegree(Label);
					break;
				case oGetAdjNodeList:
				{
					cout << "\n������� ����� �������: ";
					cin >> Label;
					vector<CEdge*> AdjList = Graph.AdjacentNodeList(Label);
					if (AdjList.size() != 0)
						cout << "������ ������� ������ � " << Label << ": ";
					else
						cout << "������� ������ ���.";
					for each (auto var in AdjList)
					{
						cout << var->GetEndNode()->GetLabel() << " ";
					}
				}
				break;
				case oIsThereNode:
				{
					cout << "\n������� ����� �������: ";
					cin >> Label;
					if (Graph.IsThereNode(Label))
						cout << "����� ������� ����.\n";
					else cout << "������� ���.\n";
				}
				break;
				case oImport:
					Graph = ImportDialog();
					Graph.PrintGraph();
					break;
				case oIsOriented:
					if (Graph.IsOriented())
						cout << "���� ���������������.\n";
					else cout << "���� �� ���������������.\n";
					break;
				case oPrintOpList:
					OperQuestion(true);
					break;
				default:
					break;
			}
		}

		Graph.DeleteGraph();
	}
	catch(int e)
	{
		Graph.DeleteGraph();
		throw 0;
	}

}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		DoTest();
	}
	catch (int e)
	{
		DoTest();
	}
}