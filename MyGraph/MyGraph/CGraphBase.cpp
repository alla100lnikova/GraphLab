#include "CGraphBase.h"
#include <algorithm>
#include<unordered_set>

CGraphBase::CGraphBase()
	:m_NodesNumber(0)
	, m_bIsOriented(true)
{
}

CGraphBase::~CGraphBase()
{
}

void CGraphBase::AddNode(string Label)
{
	if (m_Graph.find(Label) != m_Graph.end())
	{
		cout << "Вершина уже существует!";
		return;
	}
	m_Graph.insert({ Label, {} });

	SetInfo();
}

void CGraphBase::DeleteNode(string Node)
{
	if (m_Graph.find(Node) == m_Graph.end())
	{
		cout << "Вершины не существует!\n";
		return;
	}

	for (auto edge : m_Graph[Node])
	{
		delete edge;
	}
	m_Graph.erase(Node);

	for (auto& nodes : m_Graph)
	{
		for (auto node : nodes.second)
		{
			if (node->GetEndNode()->GetLabel() == Node)
			{
				nodes.second.erase(remove(nodes.second.begin(), nodes.second.end(), node), nodes.second.end());
				break;
			}
		}
	}

	SetInfo();
}

void CGraphBase::AddEdge(string NodeBegin, string NodeEnd, double Weight)
{
	AddEdge(m_Graph, NodeBegin, NodeEnd, Weight);
	if (!IsOriented())
		AddEdge(m_Graph, NodeEnd, NodeBegin, Weight);
}

void CGraphBase::AddEdge(map<string, vector<CEdge*>>& Graph, string NodeBegin, string NodeEnd, double Weight)
{
	if (Graph.find(NodeBegin) == Graph.end())
	{
		AddNode(NodeBegin);
	}

	if (Graph.find(NodeEnd) == Graph.end())
	{
		AddNode(NodeEnd);
	}

	Graph[NodeBegin].push_back(new CEdge(new CNode(NodeEnd), Weight));
}

void CGraphBase::DeleteEdgeIn(string NodeBegin, string NodeEnd)
{
	if (m_Graph.find(NodeBegin) == m_Graph.end())
	{
		cout << "Вершины не существует!\n";
		return;
	}
	for (auto node : m_Graph[NodeBegin])
	{
		if (node->GetEndNode()->GetLabel() == NodeEnd)
		{
			m_Graph[NodeBegin].erase(remove(m_Graph[NodeBegin].begin(), m_Graph[NodeBegin].end(), node), m_Graph[NodeBegin].end());
			delete node;
			break;
		}
	}
}

void CGraphBase::DeleteEdge(string NodeBegin, string NodeEnd)
{
	DeleteEdgeIn(NodeBegin, NodeEnd);
	if (!IsOriented())
		DeleteEdgeIn(NodeEnd, NodeBegin);
	SetInfo();
}

double CGraphBase::IsThereEdge(string NodeBegin, string NodeEnd)
{
	if (m_Graph.find(NodeBegin) == m_Graph.end())
	{
		return 0.0;
	}

	for (auto node : m_Graph[NodeBegin])
	{
		if (node->GetEndNode()->GetLabel() == NodeEnd) return node->GetWeight();
	}

	return 0.0;
}

//Поиск вершины == возврату списка смежности, иначе что должен возвращать поиск?
vector<CEdge*> CGraphBase::AdjacentNodeList(string NodeBegin)
{
	if (m_Graph.find(NodeBegin) == m_Graph.end())
	{
		cout << "Такой вершины в графе нет!\n";
		return vector<CEdge*>();
	}

	return m_Graph[NodeBegin];
}

bool CGraphBase::IsTree()
{
	if(IsOriented()) return false;
	if (m_EdgeNumber == m_NodesNumber - 1 && m_ConnectedComponentNumber == 1) return true;
	return false;
}

int CGraphBase::CalcEdgeNumber()
{
	int Number = 0;
	for (auto node : m_Graph)
	{
		Number += node.second.size();
	}

	return IsOriented() ? Number : Number / 2;
}

void CGraphBase::FillCenters(vector<double> rad, double min)
{
	map<int, string> NodeNumbers = NumberToNode();
	Centers.clear();
	int Count = m_Graph.size();

	if (min <= INT_MAX - 1)
	{
		for (int i = 0; i < Count; i++)
		{
			if (rad[i] == min)
				Centers.push_back(NodeNumbers[i]);
		}
	}
}

int CGraphBase::CalcDegree()
{
	int Max = 0;
	for (auto nodes : m_Graph)
	{
		int NodeDegree = nodes.second.size();
		if (NodeDegree > Max)
		{
			Max = NodeDegree;
		}
	}
	return Max;
}

double CGraphBase::CalcDiameter()
{
	return CalcRadiusOrDiameter(true);
}

double CGraphBase::CalcRadius()
{
	return CalcRadiusOrDiameter();
}

int CGraphBase::CalcCyclicNumber()
{
	return m_EdgeNumber - m_NodesNumber + m_ConnectedComponentNumber;
}

int CGraphBase::CalcConnectedComponentNumber()
{
	vector<int> Used;
	int Components = 0;
	map<string, int> NodeNumbers = NodeToNumber();
	map<string, vector<CEdge*>> Graph = m_Graph;
	if (IsOriented()) AddOrientEdges(Graph);

	for (int i = 0, Count = Graph.size(); i < Count; i++) Used.push_back(0);

	for (auto node : m_Graph)
	{
		if (!Used[NodeNumbers[node.first]])
		{
			vector<string> Cycle;
			Components++;
			DFS(Graph, node.first, Used, NodeNumbers, Cycle);
		}
	}

	return Components;
}

CEdge* CGraphBase::ImportEdge(string Label, double Weight)
{
	if (Weight > 0)
	{
		return new CEdge(new CNode(Label), Weight);
	}
	return nullptr;
}

void CGraphBase::PrintGraph()
{
	SetInfo();
	VirtStruct A;
	A.MakeGraph(m_Graph);
	A.PrintGraph();
	cout << "\n";
}

void CGraphBase::GetInfo()
{
	cout << "Степень: " << m_Degree << "\n";
	cout << "Циклическое число: " << m_CyclicNumber << "\n";
	cout << "Количество компонент связности: " << m_ConnectedComponentNumber << "\n";
	if (m_Diameter == INT_MAX)
	{
		cout << "Диаметр: бесконечный\n";
	}
	else
	{
		cout << "Диаметр: " << m_Diameter << "\n";
	}

	if(m_Radius == INT_MAX)
	{
		cout << "Радиус: бесконечный\n";
	}
	else
	{
		cout << "Радиус: " << m_Radius << "\n";
	}
	cout << "Количество вершин: " << m_NodesNumber << "\n";
	cout << "Количество рёбер: " << m_EdgeNumber << "\n";
	cout << "Граф является деревом: " << (IsTree() ? "да" : "нет") << "\n";

	cout << "Центры: ";
	for (auto var : Centers)
	{
		cout << var << " ";
	}
}

int CGraphBase::GetNodeDegree(string Node)
{
	if (AdjacentNodeList(Node).size() != 0) return m_Graph[Node].size();
	return 0;
}


void CGraphBase::EditOrient()
{
	m_bIsOriented = !m_bIsOriented;
	if (IsOriented()) return;

	AddOrientEdges(m_Graph);
	SetInfo();
}

void CGraphBase::AddOrientEdges(map<string, vector<CEdge*>>& Graph)
{
	for (auto node : Graph)
	{
		for (auto edge : node.second)
		{
			if (!IsThereEdge(edge->GetEndNode()->GetLabel(), node.first))
				AddEdge(Graph, edge->GetEndNode()->GetLabel(), node.first, edge->GetWeight());
		}
	}
}

void CGraphBase::DeleteGraph()
{
	for (auto node : m_Graph)
	{
		for (auto edge : node.second)
		{
			delete edge;
		}
	}
}

void CGraphBase::DFS()
{
	vector<int> Used;
	for (int i = 0, Count = m_Graph.size(); i < Count; i++) Used.push_back(0);
	vector<string> Cycle;
	for(auto node : m_Graph)
		DFS(m_Graph, (*m_Graph.begin()).first, Used, NodeToNumber(), Cycle, true);
}

void CGraphBase::AbsCenter()
{
	int Count = m_Graph.size();
	double **d, //матрица минимальных путей между вершинами
		**a; //матрица дуг между вершинами
	vector<Edge> rebs; // для упрощения я пихаю все ребра в один вектор

	a = GraphToIntMatrix(false);
	d = DistanceMatrix(false);

	for (int i = 0; i < Count; i++)
	{
		for (int j = 0; j < Count; j++)
		{
			if (i == j) a[i][j] = 0;
			if (i == j) d[i][j] = 0;
		}
	}

	//заполняю список ребер
	map<int, string> Name = NumberToNode();
	for (int i = 0; i < Count; i++)
	{
		for (int j = i + 1; j < Count; j++)
		{
			if (d[i][j] <= INT_MAX - 1)
			{
				rebs.push_back({ i,j });
			}
		}
	}

	//"если радиус бесконечен" или можно заменить на "если компонент связности больше 2"
	if (m_ConnectedComponentNumber < 2)
	{
		double conts = 10.0; //я как бы все ребра умножаю на 10, чтобы идти по ребру не с шагом 0.1, а с шагом 1, просто потом делю на conts в выводе
		vector<Center> Centers; //центров может быть несколько
		Center min_dot_on_edge; //центр найденный на конкретном ребре
		Center s; //а это уже реальный абс центр
		s.k = INT_MAX;
		bool flag = false;
		min_dot_on_edge.k = INT_MAX;
		for (Edge edge : rebs) //сначала идем по ребру
		{
			Center max_dist_dot_ver;
			if (a[edge.begin][edge.end] != 0 && a[edge.begin][edge.end] != INT_MAX)
			{
				double e = conts * a[edge.begin][edge.end]; //длина ребра увеличенная в 10 раз
				for (int j = 0; j <= e; j += 1) // беру точку на ребре 
				{
					max_dist_dot_ver.k = INT_MIN;

					for (int i = 0; i < Count; i++) // и ищу расстояние от точки до вершин 
					{
						double k;
						k = min((j / conts) + d[edge.begin][i], // от точки до вершины можно дойти через левую границу ребра
							(a[edge.begin][edge.end] - (j / conts)) + d[edge.end][i]); // и через правую
						if (k > max_dist_dot_ver.k) // запоминаем расстояние до максимально удаленной вершины
						{
							max_dist_dot_ver.k = k;
							max_dist_dot_ver.f = j;
							max_dist_dot_ver.reb = edge;
						}
					}
					//если максимальное расстояние этой точки на этом ребре меньше, чем у других точек на этом ребре, запоминаем ее
					if (max_dist_dot_ver.k < min_dot_on_edge.k)
					{
						min_dot_on_edge = max_dist_dot_ver;
					}

					if (!flag &&
						max_dist_dot_ver.k == min_dot_on_edge.k)
					{
						flag = true;
						Centers.push_back(max_dist_dot_ver);
					}
					if (max_dist_dot_ver.k == s.k)
					{
						Centers.push_back(max_dist_dot_ver);
					}
				}
				if (min_dot_on_edge.k < s.k) //теперь среди точек на всех ребрах берем ту, что максимальное расстояние минимально
				{
					s = min_dot_on_edge;
					Centers.clear();
					Centers.push_back(s);
				}
			}
		}
		unordered_set<int> CentersSet; //мы нашли несколько центров, они могут повторятся в вершинах, поэтому кладем центры во множество, чтобы не повторялись
		for (Center s : Centers)
		{
			if (s.f == 0) { //если центр - это левая граница ребра
				if (!CentersSet.count(s.reb.begin))
				{ //если уже есть во множестве - не выводим
					cout << "Абсолютный центр: " << Name[s.reb.begin] << endl;
					CentersSet.insert(s.reb.begin);
					cout << "Радиус = " << s.k << endl;
				}
			}
			else
				if (s.f == a[s.reb.begin][s.reb.end] * conts)
				{ //если центр - это правая граница ребра
					if (!CentersSet.count(s.reb.end))
					{
						cout << "Абсолютный центр: " << Name[s.reb.end] << endl;
						CentersSet.insert(s.reb.end);
						cout << "Радиус = " << s.k << endl;
					}
				}
				else
				{
					cout << "Абсолютный центр - это точка на ребре : (" << Name[s.reb.begin] << ", " << Name[s.reb.end] << ") значение: " << s.f / conts << endl;
					cout << "Радиус = " << s.k << endl;
				}
		}
	}
	else cout << "Нельзя найти центр в несвязном графе " << endl;

	for (int i = 0; i < Count; i++)
	{
		delete d[i];
		delete a[i];
	}
	delete[]d;
	delete[]a;
}

void CGraphBase::DFS(map<string, vector<CEdge*>> Graph,
	string Node, vector<int>& used,
	map<string, int> NodeNumbers,
	vector<string>& Cycle, bool NeedPrint)
{

	if (used[NodeNumbers[Node]]) return;
	used[NodeNumbers[Node]] = 1;
	//Cycle.push_back(Node);
	if (NeedPrint) cout << Node << " ";
	for (auto node : Graph[Node])
	{
		if (!used[NodeNumbers[node->GetEndNode()->GetLabel()]])
		{
			DFS(Graph, node->GetEndNode()->GetLabel(), used, NodeNumbers, Cycle, NeedPrint);
		}
	}
}

void CGraphBase::SetInfo()
{
	m_NodesNumber = m_Graph.size();
	m_EdgeNumber = CalcEdgeNumber();
	m_NodesNumber = m_Graph.size();
	m_Degree = CalcDegree();
	m_Diameter = CalcDiameter();
	m_Radius = CalcRadius();
	m_ConnectedComponentNumber = CalcConnectedComponentNumber();
	m_CyclicNumber = CalcCyclicNumber();
}

double CGraphBase::CalcRadiusOrDiameter(bool IsDiameter)
{
	int NodeCount = m_Graph.size();
	double** edge = DistanceMatrix(IsDiameter);

	double Result = IsDiameter ? INT_MIN : INT_MAX;
	double maxI = INT_MIN;
	vector<double> rad;
	for (int i = 0; i < NodeCount; i++)
	{
		maxI = INT_MIN;
		for (int j = 0; j < NodeCount; j++)
		{
			if (i != j)
				if (edge[i][j] > maxI) maxI = edge[i][j];
		}
		rad.push_back(maxI);
		if (IsDiameter ? Result < maxI : Result > maxI)
		{
			Result = maxI;
		}
	}

	if (!IsDiameter)
	{
		FillCenters(rad, Result);
	}

	for (int i = 0; i < NodeCount; i++)
	{
		delete[] edge[i];
	}
	delete[] edge;
	return Result;
}

double** CGraphBase::GraphToIntMatrix(bool IsDiameter)
{
	map<string, int> NodeNumbers = NodeToNumber();
	int NodeCount = m_Graph.size(), i = 0;

	double** edges = new double *[NodeCount];
	for (int i = 0; i < NodeCount; i++)
	{
		edges[i] = new double[NodeCount];
		for (int j = 0; j < NodeCount; j++) edges[i][j] = INT_MAX;
	}

	for (auto node : m_Graph)
	{
		for (auto edge : node.second)
		{
			edges[NodeNumbers[node.first]][NodeNumbers[edge->GetEndNode()->GetLabel()]] = edge->GetWeight();
		}
	}

	return edges;
}

double ** CGraphBase::DistanceMatrix(bool IsDiameter)
{
	int NodeCount = m_Graph.size();
	double** edge = GraphToIntMatrix(IsDiameter);

	for (int k = 0; k < NodeCount; k++)
		for (int i = 0; i < NodeCount; i++)
			for (int j = 0; j < NodeCount; j++)
				if (edge[i][k] < INT_MAX && edge[k][j] < INT_MAX && i != j)
					edge[i][j] = min(edge[i][j], edge[i][k] + edge[k][j]);

	//if (IsDiameter)
	{
		for (int i = 0; i < NodeCount; i++)
		{
			for (int j = 0; j < NodeCount; j++)
			{
				if (i == j)
					edge[i][j] = 0;
			}
		}
	}
	return edge;
}

map<int, string> CGraphBase::NumberToNode()
{
	map<int, string> NodeNumbers;
	int i = 0;

	for (auto node : m_Graph)
	{
		NodeNumbers.insert({ i++, node.first });
	}
	return NodeNumbers;
}

map<string, int> CGraphBase::NodeToNumber()
{
	map<string, int> NodeNumbers;
	int i = 0;

	for (auto node : m_Graph)
	{
		NodeNumbers.insert({ node.first, i++ });
	}
	return NodeNumbers;
}

void CGraphBase::ImportGraph()
{
	ifstream Fin("InputAdjacencyList.txt");
	int Number = 0, AdjNodesCount;
	string Label, AdjNode;
	vector<CEdge*> AdjEdges;
	double Weight;
	while (!Fin.eof())
	{
		Fin >> Label >> AdjNodesCount;
		for (int i = 0; i < AdjNodesCount; i++)
		{
			Fin >> AdjNode >> Weight;
			CEdge* Edge = new CEdge(new CNode(AdjNode), Weight);
			AdjEdges.push_back(Edge);
		}
		m_Graph.insert({ Label, AdjEdges });
		AdjEdges.clear();
		m_NodesNumber++;
	}
	Fin.close();
	SetInfo();
}


void CGraphBase::Define()
{
	Count = m_Graph.size();
	used.resize(Count);
	blossom.resize(Count);
	match.resize(Count);
	p.resize(Count);
	base.resize(Count);
	q.resize(Count);
}

int CGraphBase::lca(int a, int b)
{
	vector<bool> _used;
	_used.assign(Count, false);
	// поднимаемся от вершины a до корня, помечая все чётные вершины
	for (;;) {
		a = base[a];
		_used[a] = true;
		if (match[a] == -1)  break; // дошли до корня
		a = p[match[a]];
	}
	// поднимаемся от вершины b, пока не найдём помеченную вершину
	for (;;) {
		b = base[b];
		if (_used[b])  return b;
		b = p[match[b]];
	}
}

void CGraphBase::MarkPath(int v, int b, int children)
{
	while (base[v] != b)
	{
		blossom[base[v]] = blossom[base[match[v]]] = true;
		p[v] = children;
		children = match[v];
		v = p[match[v]];
	}
}

int CGraphBase::FindPath(int root)
{
	fill(used.begin(), used.end(), false);
	fill(p.begin(), p.end(), -1);
	for (int i = 0; i<Count; ++i)
		base[i] = i;

	used[root] = true;
	int qh = 0, qt = 0;
	q[qt++] = root;
	while (qh < qt)
	{
		int v = q[qh++];
		for (size_t i = 0; i<g[v].size(); ++i)
		{
			int to = g[v][i];
			if (base[v] == base[to] || match[v] == to)  continue;
			if (to == root || match[to] != -1 && p[match[to]] != -1)
			{
				int curbase = lca(v, to);
				fill(blossom.begin(), blossom.end(), 0);
				MarkPath(v, curbase, to);
				MarkPath(to, curbase, v);
				for (int i = 0; i<Count; ++i)
					if (blossom[base[i]])
					{
						base[i] = curbase;
						if (!used[i])
						{
							used[i] = true;
							q[qt++] = i;
						}
					}
			}
			else if (p[to] == -1)
			{
				p[to] = v;
				if (match[to] == -1)
					return to;
				to = match[to];
				used[to] = true;
				q[qt++] = to;
			}
		}
	}
	return -1;
}

vector<int> CGraphBase::Start()
{
	fill(match.begin(), match.end(), -1);
	for (int i = 0; i<Count; i++)
		if (match[i] == -1)
		{
			int v = FindPath(i);
			while (v != -1)
			{
				int pv = p[v], ppv = match[pv];
				match[v] = pv, match[pv] = v;
				v = ppv;
			}
		}
	return match;
}

vector<vector<int>> CGraphBase::Transform()
{
	vector<vector<int>> g;
	map<string, int> Num = NodeToNumber();
	g.resize(Count);
	int i = 0;
	for (auto node : m_Graph)
	{
		for (auto edge : node.second)
		{
			g[i].push_back(Num[edge->GetEndNode()->GetLabel()]);
		}
		i++;
	}
	return g;
}

void CGraphBase::Matching()
{
	Define();
	g = Transform();
	vector<int> edges = Start();
	map<int, string> Name = NumberToNode();
	unordered_set<int> exclude;
	cout << " Ребра, вошедшие в наибольшее паросочетание:\n";
	for (int i = 0; i < Count; i++)
		if (edges[i] != -1 && exclude.count(i) == 0 && exclude.count(edges[i]) == 0) {
			cout << " " << Name[i] << '-' << Name[edges[i]] << endl;
			exclude.insert(i);
			exclude.insert(edges[i]);
		}
}
