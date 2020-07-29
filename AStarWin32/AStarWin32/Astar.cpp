// AStar.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "Astar.h"

using namespace std;
int AStar::AStarStart(Vector2DI start, Vector2DI end)
{	
	Initialze(start, end);	
	
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

void AStar::Initialze(Vector2DI start, Vector2DI end)
{
	openlist.clear();
	closelist.clear();
	path.clear();
	start_ = start;
	end_ = end;


	//auto startNode = std::make_shared<Node>(start_);
	//shared_ptr<Node> startNode(new Node(start_));
	//printf("%x\n", &startNode);
	//printf("%x\n", startNode.get());
	//openlist.emplace(map<int, Node*>::value_type(0, startNode.get()));

	MakeRoute();
}

Node* AStar::MakeChildNode(int x, int y, Node* currentNode)
{
	if (const auto& node = GetNode(x, y))
	{
		node->Parent = currentNode;

		//현재 노드 기준으로, 비용을 다시 계산한다.
		int h = abs(end_.Y - node->Position.Y) + abs(end_.X - node->Position.X);
		int g = abs(node->Position.Y - currentNode->Position.Y) + abs(node->Position.X - currentNode->Position.X);
		node->G = g;
		node->H = h;
		node->F = g + h;

		return node;
	}
	return nullptr;
}
void AStar::MakeRoute()
{
	nodeList_.clear();
	for (int y = 0; y < WorldManager::MAP_MAX_Y; ++y)
	{
		for (int x = 0; x < WorldManager::MAP_MAX_X; ++x)
		{
			//Node* ptr = new Node();
			//std::shared_ptr<Node> node(new Node());
			auto node = std::make_shared<Node>();
			node->Index = (WorldManager::MAP_MAX_Y * y) + x;
			node->ObjectType = (WorldManager::EObjectType)WorldManager::Get()->mapData[y][x];
			node->Position = std::move(Vector2DI(x, y));
			int h = (end_.Y - node->Position.Y) + (end_.X - node->Position.X);
			int g = (node->Position.Y - start_.Y) + (node->Position.X - start_.X);
			node->G = g;
			node->H = h;
			node->F = g + h;
			nodeList_.emplace(make_pair(node->Index, node));
		}
	}
}
Node* AStar::GetNode(int x, int y)
{
	if (x < 0 || y < 0)
		return nullptr;

	int index = WorldManager::MAP_MAX_Y * y + x;
	if (nodeList_.find(index) == nodeList_.end())
	{
		return nullptr;
	}
	return nodeList_[index].get();
}
struct OpenlistSortFunctor
{
	bool operator()(Node* lhs, Node* rhs)
	{
		if (lhs->F < rhs->F)
		{
			return true;
		}
		if (lhs->F > rhs->F)
		{
			return false;
		}
		return false;
	}
};
void AStar::SortOpenlist()
{
	openlist.sort([](Node* lhs, Node* rhs)
	{
		if (lhs->F < rhs->F)
		{
			return true;
		}
		if (lhs->F > rhs->F)
		{
			return false;
		}
		return false;
	});
	//list는 자신의 sort버전이 있다.
	//std::sort(openlist.begin(), openlist.end(), [](Node* lhs, Node* rhs)
	//{
	//    if (lhs->F < rhs->F)
	//    {
	//        return true;
	//    }
	//    if (lhs->F > rhs->F)
	//    {
	//        return false;
	//    }
	//    return false;
	//});
}


void AStar::FindPath()
{
	if (openlist.size() == 0)
	{
		cout << "Can`t find path" << endl;
		return;
	}
	
	TraceInfo debugInfo;

	//오픈리스트중 가장 비용(f)가 적은 것을 선택한다.
	auto currentNode = openlist.front();
	if (currentNode)
	{
		debugInfo.Current = currentNode;

		path.push_back(currentNode);
		for (auto& pathNode : path)
		{
			debugInfo.path.push_back(pathNode);
		}

		if (currentNode->Position == end_)
		{
			cout << "Find Path" << endl;
			PrintPath();
			return;
		}
		//선택된 노드는 close에 넣는다.
		closelist.push_back(currentNode);
		openlist.remove(currentNode);

		for (auto closeNode : closelist)
		{
			debugInfo.closelist.push_back(closeNode);
		}


		//8방향으로, 장애물이 아니면서, close리스트에 없는 노드를 검색하여, openlist에 넣는다.
		Node* dir[8];
		dir[0] = MakeChildNode(currentNode->Position.X - 1, currentNode->Position.Y, currentNode);
		dir[1] = MakeChildNode(currentNode->Position.X - 1, currentNode->Position.Y - 1, currentNode);
		dir[2] = MakeChildNode(currentNode->Position.X, currentNode->Position.Y - 1, currentNode);
		dir[3] = MakeChildNode(currentNode->Position.X + 1, currentNode->Position.Y - 1, currentNode);
		dir[4] = MakeChildNode(currentNode->Position.X + 1, currentNode->Position.Y, currentNode);
		dir[5] = MakeChildNode(currentNode->Position.X + 1, currentNode->Position.Y + 1, currentNode);
		dir[6] = MakeChildNode(currentNode->Position.X, currentNode->Position.Y + 1, currentNode);
		dir[7] = MakeChildNode(currentNode->Position.X - 1, currentNode->Position.Y + 1, currentNode);
		for (int i = 0; i < 8; ++i)
		{
			if (dir[i])
			{
				if (dir[i]->ObjectType == WorldManager::Wall)
				{
					continue;
				}

				if (std::find(closelist.begin(), closelist.end(), dir[i]) != closelist.end())
				{
					continue;;
				}

				//이미 open에 있어도 추가하지 않는다. 정렬해봐야 같은 것 길찾게 됨으로
				if (std::find(openlist.begin(), openlist.end(), dir[i]) != openlist.end())
				{
					continue;;
				}
				
				//연결된 경로가 아니면 대상에서 패스해야 한다. -알고리즘상 없을텐데 만들다보니 추가했다. 현재는 의미없을듯
				//이때 첫 노드는 부모 정보가 없음으로, null체크한다.
				if(dir[i]->Parent != currentNode)// && currentNode->Parent != nullptr)
				{
					continue; 
				};


				openlist.push_back(dir[i]);
			}
		}
		SortOpenlist();

		for (auto openNode : openlist)
		{
			debugInfo.openlist.push_back(openNode);
		}
		TraceManager::Get()->traceInfo.push_back(debugInfo);

		FindPath();
	}
}
void AStar::PrintPath()
{
	for (auto pathNode : path)
	{
		WorldManager::Get()->mapData[pathNode->Position.Y][pathNode->Position.X] = 2;
		//cout << pathNode->Position.X << ":" << pathNode->Position.Y << endl;
		for (int y = 0; y < WorldManager::MAP_MAX_Y; ++y)
		{
			for (int x = 0; x < WorldManager::MAP_MAX_X; ++x)
			{
				cout << WorldManager::Get()->mapData[y][x];
			}
			cout << endl;
		}
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << endl;
	}
	cout << endl;
}

void AStar::PrintMap()
{
	for (int y = 0; y < WorldManager::MAP_MAX_Y; ++y)
	{
		for (int x = 0; x < WorldManager::MAP_MAX_X; ++x)
		{
			cout << WorldManager::Get()->mapData[y][x];
		}
		cout << endl;
	}
	cout << endl;
}

//int main()
//{
//	AStar aster;
//	aster.AStarStart(Vector2DI(0, 0), Vector2DI(4, 4));
//	//aster.PrintMap();
//	//aster.PrintPath();
//	return 0;
//}