#include "CNode.h"

CNode::CNode(string Label) 
	  : m_Label(Label)
{ 
}

CNode::~CNode()
{
}

string CNode::GetLabel()
{
	return m_Label;
}
