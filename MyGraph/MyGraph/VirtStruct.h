#pragma once
#include <iostream>
#include <vector>
#include"CGraphBase.h"
#include<string>
#define TRUE 1
#define FALSE 0
using namespace std;

typedef int Boolean;
typedef struct L *Lref;
typedef struct T *Tref;
typedef struct L
{
	string Key;
	int Count;
	int Count1;
	Tref Pred;
	Tref Trail;
	Lref Next;

} Leader;
typedef struct T
{
	Lref Id;
	Tref Next;
} Trailer;

class VirtStruct {
private:

	Lref Head;
	Lref Tail;

	Lref SearchGraph(string);
	Lref Search(string);
public:
	VirtStruct() {
		Head = Tail = new (Leader);
	}
	~VirtStruct(); 
	void MakeGraph(map<string, vector<CEdge*>> Graph);
	void PrintGraph();
	void Free1Graph(Lref *, Lref *);
	void Free2Graph(Tref *);
};
