#include "stdafx.h"
#include "MoveMap.h"
#include "cNode.h"


MoveMap::MoveMap()
{
}


MoveMap::~MoveMap()
{
}

void MoveMap::init()
{
	Head = new cNode({ 1,0,1 });
	Tail = Head;
	CurDestNode = Head;
	Head->Pre = NULL;
	
	addNode({ 10, 0, 0 });
	addNode({ 20, 0, 10 });
	addNode({ 30, 0, 20 });
	addNode({ 40, 0, 15 });
	addNode({ 50, 0, 20 });
	addNode({ 50, 0, 40 });
	addNode({ 50, 0, 30 });
	addNode({ 35, 0, 70 });
	addNode({ 90, 0, 90 });
	addNode({ 150, 0, 90 });
	addNode({ 150, 0, 150 });
	addNode({ 90, 0, 150 });

	cNode* Node = Head;
	for (; Node->Next != NULL; Node = Node->Next)
	{
		Node->SetNextRotate(Node->Next->pos.GetWorldPosition());
	}




}

cNode* MoveMap::PreNode()
{
	return CurDestNode->Pre;
}

void MoveMap::setCurNodeNext()
{
	if(CurDestNode->Next != NULL)
	CurDestNode = CurDestNode->Next;


}

cNode * MoveMap::GetCurNode()
{
	return CurDestNode;
}

cNode* MoveMap::NextNode()
{
	return CurDestNode->Next;
}

void MoveMap::addNode(D3DXVECTOR3 Vec)
{
	Tail->Pre = Tail;
	Tail->Next = new cNode(Vec);
	Tail = Tail->Next;
}

void MoveMap::Render(cNode* NextNode)
{
	GIZMO_MGR->Circle(NextNode->pos.GetLocalPosition(), 3.0f);

	if (NextNode->Next == NULL)
	{
		return;
	}

	GIZMO_MGR->Line(NextNode->pos.GetLocalPosition(), NextNode->Next->pos.GetLocalPosition(), 0x110000);

	Render(NextNode->Next);
}

void MoveMap::renderNodeRoute()
{
	Render(Head);
}
