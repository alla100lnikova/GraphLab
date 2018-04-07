#include "VirtStruct.h"

VirtStruct::~VirtStruct()
{
	Lref t = Head;
	while (t != Tail)
	{
		Free2Graph(&(*t).Trail); t = (*t).Next;
	}
	Free1Graph(&Head, &Tail);
	delete Tail;
}

Lref VirtStruct::SearchGraph(string w)
{
	Lref h;

	h = Head; (*Tail).Key = w;
	while ((*h).Key != w) h = (*h).Next;
	if (h == Tail)
	{
		Tail = new (Leader); (*h).Count = (*h).Count1 = 0;
		(*h).Trail = (*h).Pred = NULL; (*h).Next = Tail;
	}
	return h;
}

Lref VirtStruct::Search(string w)
{
	Lref h = Head;
	(*Tail).Key = w; //Поиск "с барьером".
	while ((*h).Key != w) h = (*h).Next;
	if (h == Tail)
		h = NULL;
	return h;
}

void VirtStruct::MakeGraph(map<string, vector<CEdge*>> Graph)
{
	string x, y;
	Lref p, q = nullptr;
	Tref t, r;
	Boolean Res;

	for (auto edgeList : Graph)
	{
		x = edgeList.first;
		p = SearchGraph(x);
		for (auto edge : edgeList.second)
		{
			y = edge->GetEndNode()->GetLabel();
			//существует ли в графе дуга (x,y)?
			q = SearchGraph(y);
			r = (*p).Trail; Res = FALSE;
			while ((r != NULL) && (!Res))
			{
				if ((*r).Id == q) Res = TRUE;
				else r = (*r).Next;
			}
			if (!Res)
			{
				t = new (Trailer); (*t).Id = q;
				(*t).Next = (*p).Trail; (*p).Trail = t; (*q).Count++;
				t = new (Trailer); (*t).Id = p;
				(*t).Next = (*q).Pred;  (*q).Pred = t; (*p).Count1++;
			}
		}

		if (edgeList.second.size() == 0)
		{
			t = new (Trailer); (*t).Id = p;
		}
	}
}

void VirtStruct::PrintGraph()
{
	Lref p;
	Tref q;

	p = Head;
	while (p != Tail)
	{
		cout << (*p).Key << "(("; q = (*p).Trail;
		while (q != NULL)
		{
			cout << (*(*q).Id).Key << " "; q = (*q).Next;
		}
		cout << ")(";
		q = p->Pred;
		while (q != NULL)
		{
			cout << (*(*q).Id).Key << " "; q = q->Next;
		}
		cout << "))";
		p = (*p).Next; cout << " ";
	}
}

void VirtStruct::Free1Graph(Lref *Head, Lref *Tail)
{
	if (*Head != *Tail)
	{
		Free1Graph(&(**Head).Next, Tail); delete *Head; *Head = NULL;
	}
}

void VirtStruct::Free2Graph(Tref *X)
{
	if (*X != NULL)
	{
		Free2Graph(&(**X).Next); delete *X; *X = NULL;
	}
}
