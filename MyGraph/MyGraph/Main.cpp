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
	cout << "\nКакое представление будем импортировать?\n";
	cout << "1. Список смежности\n";
	cout << "2. Матрица смежности\n";
	cout << "3. Матрица инцидентности\n";
	cout << "4. Список дуг\n";
	cout << "Выбор: ";
}

void OperQuestion(bool IsStart)
{
	cout << "\nВыберите операцию (16 - показать список операций):\n";
	if (IsStart)
	{
		cout << "1. Добавить вершину\n";
		cout << "2. Удалить вершину\n";
		cout << "3. Добавить ребро\n";
		cout << "4. Удалить ребро\n";
		cout << "5. Изменить ориентированность графа\n";
		cout << "6. Вывести инварианты\n";
		cout << "7. Вывести вершины в порядке обхода в глубину\n";
		cout << "8. Напечатать граф\n";
		cout << "9. Проверить наличие ребра\n";
		cout << "10. Проверить, является ли граф деревом\n";
		cout << "11. Узнать степень вершины\n";
		cout << "12. Получить список смежных вершин для заданной вершины\n";
		cout << "13. Проверить наличие вершины\n";
		cout << "14. Импорт нового графа\n";
		cout << "15. Узнать ориентированность графа\n";
		cout << "0. Выйти\n";
	}
	cout << "Выбор: ";
}

CGraphBase ImportDialog(bool IsStart = false)
{
	if (IsStart)
		ImportQuestion();
	else
		cout << "\nКакое представление будем импортировать?\nВыбор: ";

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
					cout << "\nВведите метку вершины: ";
					cin >> Label;
					Graph.AddNode(Label);
					Graph.PrintGraph();
					break;
				case oDelNode:
					cout << "\nВведите метку вершины: ";
					cin >> Label;
					Graph.DeleteNode(Label);
					Graph.PrintGraph();
					break;
				case oAddEdge:
					cout << "\nВведите через пробел метку вершины - откуда, метку вершины - куда и вес: ";
					cin >> Label >> LabelOut >> Weight;
					Graph.AddEdge(Label, LabelOut, Weight);
					Graph.SetInfo();
					Graph.PrintGraph();
					break;
				case oDelEdge:
					cout << "\nВведите через пробел метку вершины - откуда и метку вершины - куда: ";
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
					cout << "\nВведите через пробел метку вершины - откуда и метку вершины - куда: ";
					cin >> Label >> LabelOut;
					if (Graph.IsThereEdge(Label, LabelOut))
						cout << "Ребро существует.\n";
					else cout << "Ребра не существует.\n";
					break;
				case oIsTree:
					if (Graph.IsTree())
						cout << "Граф является деревом.\n";
					else cout << "Граф не дерево.\n";
					break;
				case oGetNodeDegree:
					cout << "\nВведите метку вершины: ";
					cin >> Label;
					cout << Graph.GetNodeDegree(Label);
					break;
				case oGetAdjNodeList:
				{
					cout << "\nВведите метку вершины: ";
					cin >> Label;
					vector<CEdge*> AdjList = Graph.AdjacentNodeList(Label);
					if (AdjList.size() != 0)
						cout << "Список смежных вершин с " << Label << ": ";
					else
						cout << "Смежных вершин нет.";
					for each (auto var in AdjList)
					{
						cout << var->GetEndNode()->GetLabel() << " ";
					}
				}
				break;
				case oIsThereNode:
				{
					cout << "\nВведите метку вершины: ";
					cin >> Label;
					if (Graph.IsThereNode(Label))
						cout << "Такая вершина есть.\n";
					else cout << "Вершины нет.\n";
				}
				break;
				case oImport:
					Graph = ImportDialog();
					Graph.PrintGraph();
					break;
				case oIsOriented:
					if (Graph.IsOriented())
						cout << "Граф ориентированный.\n";
					else cout << "Граф не ориентированный.\n";
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