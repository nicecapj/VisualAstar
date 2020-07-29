#include "stdafx.h"
#include "Jps.h"

int Jps::JpsStart(Vector2DI start, Vector2DI end)
{
	AStar::Initialze(start, end);
	

	auto startNode = GetNode(start.X, start.Y);
	assert(startNode);
	if (startNode)
	{
		openlist.push_front(startNode);
		//path.push_back(startNode);
	}

	FindPath();
	return 0;
}

bool Jps::CheckR(Node* currentNode, int x, int y)
{
	int checkX = x + 1;
	int checkY = y;

	auto nextNode = GetNode(checkX, checkY);
	if (!nextNode)
		return false;
	
	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y && nextNode->State == WorldManager::Road)
	{
		auto wallNode = GetNode(checkX, checkY - 1);
		auto forceNeighborNode = GetNode(checkX +1, checkY - 1);
		if (!wallNode || !forceNeighborNode)
			return false;

		//위쪽 강제 이웃 체크
		if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
		{
			nextNode->Parent = currentNode;		
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			wallNode->State = WorldManager::ForceNeighbor;
			return true;
		}

		//아래쪽 강제 이웃 체크
		wallNode = GetNode(checkX, checkY + 1);
		forceNeighborNode = GetNode(checkX + 1, checkY + 1);
		if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			wallNode->State = WorldManager::ForceNeighbor;
			return true;
		}
		
		//목적지에 도달한 경우		
		if (nextNode->Position.X == end_.X && nextNode->Position.Y == end_.Y)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			wallNode->State = WorldManager::ForceNeighbor;

			//isFindPath_ = true;
			return true;
		}

		//해당 방향으로 계속 이동하면서 체크
		++checkX;		
	}
	
	return false;
}

bool Jps::CheckL(Node* currentNode, int x, int y)
{
	int checkX = x - 1;
	int checkY = y;

	auto nextNode = GetNode(checkX, checkY);
	if (!nextNode)
		return false;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y && nextNode->State == WorldManager::Road)
	{
		auto wallNode = GetNode(checkX, checkY - 1);
		auto forceNeighborNode = GetNode(checkX - 1, checkY - 1);
		if (!wallNode || !forceNeighborNode)
			return false;

		//위쪽 강제 이웃 체크
		if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			wallNode->State = WorldManager::ForceNeighbor;
			return true;
		}

		//아래쪽 강제 이웃 체크
		wallNode = GetNode(checkX, checkY + 1);
		forceNeighborNode = GetNode(checkX - 1, checkY + 1);
		if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			wallNode->State = WorldManager::ForceNeighbor;
			return true;
		}

		//목적지에 도달한 경우		
		if (nextNode->Position.X == end_.X && nextNode->Position.Y == end_.Y)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			wallNode->State = WorldManager::ForceNeighbor;

			//isFindPath_ = true;
			return true;
		}

		//해당 방향으로 계속 이동하면서 체크
		++checkX;
	}

	return false;
}

bool Jps::CheckUR(Node* currentNode, int x, int y)
{
	return false;
}

bool Jps::CheckUL(Node* currentNode, int x, int y)
{
	return false;
}

bool Jps::CheckDR(Node* currentNode, int x, int y)
{
	return false;
}

bool Jps::CheckDL(Node* currentNode, int x, int y)
{
	return false;
}

bool Jps::CheckU(Node* currentNode, int x, int y)
{
	return false;
}

bool Jps::CheckD(Node* currentNode, int x, int y)
{
	return false;
}

void Jps::FindPath()
{
	while (!openlist.empty())
	{
		auto currentNode = openlist.front();
		if (currentNode)
		{
			//상하좌우			
			CheckU(currentNode, currentNode->Position.X, currentNode->Position.Y);
			CheckD(currentNode, currentNode->Position.X, currentNode->Position.Y);
			CheckL(currentNode, currentNode->Position.X, currentNode->Position.Y);
			CheckR(currentNode, currentNode->Position.X, currentNode->Position.Y);
			//우상/우하/좌상/좌하
			CheckUR(currentNode, currentNode->Position.X, currentNode->Position.Y);
			CheckUL(currentNode, currentNode->Position.X, currentNode->Position.Y);
			CheckDR(currentNode, currentNode->Position.X, currentNode->Position.Y);
			CheckDL(currentNode, currentNode->Position.X, currentNode->Position.Y);
		}
	}
}