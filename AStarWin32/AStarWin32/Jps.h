#pragma once
#include "Common.h"
#include "Astar.h"

class Jps : public AStar
{
public:
	int JpsStart(Vector2DI start, Vector2DI end);

	bool CheckU(Node* currentNode, int x, int y);
	bool CheckD(Node* currentNode, int x, int y);
	bool CheckR(Node* currentNode, int x, int y);
	bool CheckL(Node* currentNode, int x, int y);

	bool CheckUR(Node* currentNode, int x, int y);
	bool CheckUL(Node* currentNode, int x, int y);
	bool CheckDR(Node* currentNode, int x, int y);
	bool CheckDL(Node* currentNode, int x, int y);
private:
	virtual void FindPath() override;
};

