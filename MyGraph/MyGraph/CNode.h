#pragma once
#include <algorithm>
#include <string>
using namespace std;

class CNode
{
private:
	string m_Label;

public:
	CNode(string Label);
	~CNode();
	string GetLabel();
};

