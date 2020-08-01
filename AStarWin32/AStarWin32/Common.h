#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <assert.h>
#include <algorithm>
#include <future>

using namespace std;
//-----------------------------------------------------------------------------------------------
class Node;
class WorldManager
{
public:
	static const int MAP_MAX_X = 5;
	static const int MAP_MAX_Y = 5;
	const int mapData[MAP_MAX_Y][MAP_MAX_X] =
	{
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	};
	enum EObjectType
	{
		Road = 0,
		Wall = 1,
		Closed = 2,

	};

	static WorldManager* Get()
	{
		static WorldManager instance_;
		return &instance_;
	}
};
//-----------------------------------------------------------------------------------------------
template<typename T>
class Vector2D
{
public:
	T X;
	T Y;
	Vector2D() :X(T()), Y(T()) {};
	Vector2D(int x, int y) :X(x), Y(y) {}
	Vector2D(const Vector2D& other)
	{
		X = other.X;
		Y = other.Y;
	}
	//������ �� ������ �³� üũ�غ��� - move�����ڿ� const�� �ٿ��� �ϴ°�
	Vector2D(Vector2D&& other)
	{
		X = std::move(other.X);   //std::move�� ���� ����ϴ°�? rvalue�� ��������� ����ȯ �ϱ� ���� ���. �̵������� �ƴ����� �������� ����.
		Y = std::move(other.Y);   //������, �̵�������, ��������� ���ο��� std::move�� �� ������, �ѱ涧 std::move�� �����ʿ䰡 ����.
	}
	Vector2D& operator=(const Vector2D& other)
	{
		this->X = other.X;
		this->Y = other.Y;
		return *this;
	}
	//������ �� ������ �³� üũ�غ��� - move ���� �����ڿ� const �ٿ��� �ϴ°�(x) -> �׷� ����� ó����.
	Vector2D& operator=(Vector2D&& other)
	{
		this->X = std::move(other.X);
		this->Y = std::move(other.Y);
		return *this;
	}

	bool operator==(const Vector2D& other)
	{
		return this->X == other.X && this->Y == other.Y;
	}
};
using Vector2DI = Vector2D<int>;
//-----------------------------------------------------------------------------------------------
class Node
{
public:
	int Index = 0;
	//Node* Parent;
	Node* Parent = nullptr;
	WorldManager::EObjectType ObjectType = WorldManager::Road;
	Vector2DI Position = {};
	int F = 0;

	int G = 0;	//���� ��갪 F�� �־ ������, ����뿡 ���Ƽ� G�� H�߰�
	int H = 0;

	int State = 0;	//AStar�ܿ� �˰��򿡼�, �ΰ����� �뵵�� ���

	Node() = default;
	~Node()
	{}
	
	explicit Node(int f)
	{
		F = f;
	}
	
	explicit Node(Vector2DI pos) :Position(pos)
	{}
	
	Node(const Node& lvalue)
	{
		Index = lvalue.Index;
		Parent = lvalue.Parent;
		ObjectType = lvalue.ObjectType;
	}
	Node(Node&& rvalue)
	{
		Index = std::move(rvalue.Index);
		Parent = std::move(rvalue.Parent);
		ObjectType = std::move(rvalue.ObjectType);
	}
};


//-----------------------------------------------------------------------------------------------
struct TraceInfo
{
public:
	Node* Current;
	list<Node*> openlist;
	list<Node*> closelist;//�� �񱳸� �Ҷ� weak_ptr�� == �񱳰� �ȵǰ� , shared_ptr�� ��½��Ѿ� ������ ���� ������ �ƴϴ�.
	list<Node*> path;//�����̳ʿ� ���� �ְ�, �񱳰˻��� std::find���� �˰������� ã������ raw pointer�� shared_ptr�� �������
};


class TraceManager
{
public:
	std::vector<TraceInfo> traceInfo;

	static TraceManager* Get()
	{
		static TraceManager instance_;
		return &instance_;
	}
};