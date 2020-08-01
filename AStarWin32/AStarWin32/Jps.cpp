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

bool Jps::CheckU(Node* currentNode, int x, int y)
{
	int checkX = x;
	int checkY = y - 1;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		//위쪽 강제 이웃 체크
		auto wallNode = GetNode(checkX - 1, checkY);
		auto forceNeighborNode = GetNode(checkX - 1, checkY - 1);
		if (wallNode && forceNeighborNode)
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//아래쪽 강제 이웃 체크
		wallNode = GetNode(checkX + 1, checkY);
		forceNeighborNode = GetNode(checkX + 1, checkY - 1);
		if (wallNode && forceNeighborNode)
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//목적지에 도달한 경우		
		if (nextNode->Position.X == end_.X && nextNode->Position.Y == end_.Y)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			nextNode->State = WorldManager::Closed;
			openlist.push_back(nextNode);

			//isFindPath_ = true;
			return true;
		}

		//해당 방향으로 계속 이동하면서 체크
		--checkY;
	}

	return false;
}

bool Jps::CheckD(Node* currentNode, int x, int y)
{
	int checkX = x;
	int checkY = y + 1;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		//위쪽 강제 이웃 체크
		auto wallNode = GetNode(checkX - 1, checkY);
		auto forceNeighborNode = GetNode(checkX - 1, checkY + 1);
		if (wallNode && forceNeighborNode)
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}
			
		//아래쪽 강제 이웃 체크
		wallNode = GetNode(checkX + 1, checkY);
		forceNeighborNode = GetNode(checkX + 1, checkY + 1);
		if (wallNode && forceNeighborNode)
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				wallNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//목적지에 도달한 경우		
		if (nextNode->Position.X == end_.X && nextNode->Position.Y == end_.Y)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			nextNode->State = WorldManager::Closed;
			openlist.push_back(nextNode);
			//isFindPath_ = true;
			return true;
		}

		//해당 방향으로 계속 이동하면서 체크
		++checkY;
	}

	return false;
}

bool Jps::CheckL(Node* currentNode, int x, int y)
{
	int checkX = x - 1;
	int checkY = y;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		//위쪽 강제 이웃 체크
		auto wallNode = GetNode(checkX, checkY - 1);
		auto forceNeighborNode = GetNode(checkX - 1, checkY - 1);
		if (wallNode && nextNode)
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//아래쪽 강제 이웃 체크
		wallNode = GetNode(checkX, checkY + 1);
		forceNeighborNode = GetNode(checkX - 1, checkY + 1);
		if (wallNode && nextNode)
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//목적지에 도달한 경우		
		if (nextNode->Position.X == end_.X && nextNode->Position.Y == end_.Y)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			nextNode->State = WorldManager::Closed;
			openlist.push_back(nextNode);

			//isFindPath_ = true;
			return true;
		}

		//해당 방향으로 계속 이동하면서 체크
		--checkX;
	}

	return false;
}

bool Jps::CheckR(Node* currentNode, int x, int y)
{
	int checkX = x + 1;
	int checkY = y;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		auto wallNode = GetNode(checkX, checkY - 1);
		auto forceNeighborNode = GetNode(checkX + 1, checkY - 1);
		if (wallNode && nextNode)
		{
			//위쪽 강제 이웃 체크
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//아래쪽 강제 이웃 체크
		wallNode = GetNode(checkX, checkY + 1);
		forceNeighborNode = GetNode(checkX + 1, checkY + 1);
		if (wallNode && nextNode) 
		{
			if (wallNode->State == WorldManager::Wall && forceNeighborNode->State == WorldManager::Road)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
				return true;
			}
		}

		//목적지에 도달한 경우		
		if (nextNode->Position.X == end_.X && nextNode->Position.Y == end_.Y)
		{
			nextNode->Parent = currentNode;
			nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
			nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
			nextNode->F = nextNode->G + nextNode->H;
			nextNode->State = WorldManager::Closed;
			openlist.push_back(nextNode);
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
	int checkX = x + 1;
	int checkY = y - 1;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		//위, 오른쪽 체크
		if (CheckU(nextNode, checkX, checkY) || CheckR(nextNode, checkX, checkY))
		{
			if (nextNode->State != WorldManager::Closed || nextNode->Parent == nullptr)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
			}
		}
		//
		else if (checkX != 0 && checkY != 0)
		{
			auto wallNode = GetNode(checkX + 1, checkY - 1);
			if (wallNode && wallNode->State == WorldManager::Wall)
			{
				if (nextNode->State != WorldManager::Closed)
				{
					nextNode->Parent = currentNode;
					nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
					nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
					nextNode->F = nextNode->G + nextNode->H;
					nextNode->State = WorldManager::Closed;
					openlist.push_back(nextNode);
				}
			}
		}

		//해당 방향으로 계속 이동하면서 체크
		++checkX;
		--checkY;
	}

	return false;
}

bool Jps::CheckUL(Node* currentNode, int x, int y)
{
	int checkX = x - 1;
	int checkY = y - 1;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		if (CheckU(nextNode, checkX, checkY) || CheckL(nextNode, checkX, checkY))
		{
			if (nextNode->State != WorldManager::Closed || nextNode->Parent == nullptr)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
			}
		}
		else if (checkX != 0 && checkY != 0)
		{
			auto wallNode = GetNode(checkX - 1, checkY - 1);
			if (wallNode && wallNode->State == WorldManager::Wall)
			{
				if (nextNode->State != WorldManager::Closed)
				{
					nextNode->Parent = currentNode;
					nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
					nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
					nextNode->F = nextNode->G + nextNode->H;
					nextNode->State = WorldManager::Closed;
					openlist.push_back(nextNode);
				}
			}
		}

		//해당 방향으로 계속 이동하면서 체크
		--checkX;
		--checkY;
	}

	return false;
}

bool Jps::CheckDR(Node* currentNode, int x, int y)
{
	int checkX = x + 1;
	int checkY = y + 1;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		//위, 오른쪽 체크
		if (CheckD(nextNode, checkX, checkY) || CheckR(nextNode, checkX, checkY))
		{
			if (nextNode->State != WorldManager::Closed || nextNode->Parent == nullptr)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
			}
		}
		//
		else if (checkX != 0 && checkY != 0)
		{
			auto wallNode = GetNode(checkX + 1, checkY + 1);
			if (wallNode && wallNode->State == WorldManager::Wall)
			{
				if (nextNode->State != WorldManager::Closed)
				{
					nextNode->Parent = currentNode;
					nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
					nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
					nextNode->F = nextNode->G + nextNode->H;
					nextNode->State = WorldManager::Closed;
					openlist.push_back(nextNode);
				}
			}
		}

		//해당 방향으로 계속 이동하면서 체크
		++checkX;
		++checkY;
	}

	return false;
}

bool Jps::CheckDL(Node* currentNode, int x, int y)
{
	int checkX = x - 1;
	int checkY = y + 1;

	//벽이면 중지한다(== 벽이 아니면 진행한다)
	while (checkX < WorldManager::MAP_MAX_Y && checkY < WorldManager::MAP_MAX_Y)
	{
		auto nextNode = GetNode(checkX, checkY);
		if (!nextNode)
			return false;

		if (nextNode->State != WorldManager::Road)
			return false;

		//위, 오른쪽 체크
		if (CheckD(nextNode, checkX, checkY) || CheckL(nextNode, checkX, checkY))
		{
			if (nextNode->State != WorldManager::Closed || nextNode->Parent == nullptr)
			{
				nextNode->Parent = currentNode;
				nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
				nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
				nextNode->F = nextNode->G + nextNode->H;
				nextNode->State = WorldManager::Closed;
				openlist.push_back(nextNode);
			}
		}
		//
		else if (checkX != 0 && checkY != 0)
		{
			auto wallNode = GetNode(checkX - 1, checkY + 1);
			if (wallNode && wallNode->State == WorldManager::Wall)
			{
				if (nextNode->State != WorldManager::Closed)
				{
					nextNode->Parent = currentNode;
					nextNode->G = (nextNode->Position.Y - currentNode->Position.Y) + (nextNode->Position.X - currentNode->Position.X) + currentNode->G;
					nextNode->H = (end_.Y - currentNode->Position.Y) + (end_.X - currentNode->Position.X);
					nextNode->F = nextNode->G + nextNode->H;
					nextNode->State = WorldManager::Closed;
					openlist.push_back(nextNode);
				}
			}
		}

		//해당 방향으로 계속 이동하면서 체크
		--checkX;
		++checkY;
	}

	return false;
}


void Jps::FindPath()
{
	while (!openlist.empty())
	{
		auto currentNode = openlist.front();
		if (currentNode)
		{
			if (currentNode->Position == end_)
			{
				cout << "Find Path" << endl;
				PrintPath();
				return;
			}

			openlist.pop_front();

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

			SortOpenlist();
		}
	}
}