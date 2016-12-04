#pragma once

class cNode;

class MoveMap
{

	int NodeNum;

public:
	MoveMap();
	~MoveMap();

	cNode* Head;
	cNode* CurDestNode;
	cNode* CurSourNode;
	cNode* Tail;

	void init();
	void addNode(D3DXVECTOR3 Vec);
	void Render(cNode* NextNode);
	void renderNodeRoute();
	cNode* NextNode();
	cNode* PreNode();
	void setCurNodeNext();
	cNode* GetCurNode();


};

