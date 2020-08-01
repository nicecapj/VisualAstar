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
	//선언이 이 형식이 맞나 체크해보자 - move생성자에 const를 붙여야 하는가
	Vector2D(Vector2D&& other)
	{
		X = std::move(other.X);   //std::move는 언제 사용하는가? rvalue를 명시적으로 형변환 하기 위해 사용. 이동결정은 아니지만 가독성에 좋다.
		Y = std::move(other.Y);   //생성자, 이동생성자, 복사생성자 내부에서 std::move가 되 있으면, 넘길때 std::move를 해줄필요가 없다.
	}
	Vector2D& operator=(const Vector2D& other)
	{
		this->X = other.X;
		this->Y = other.Y;
		return *this;
	}
	//선언이 이 형식이 맞나 체크해보자 - move 대입 연산자에 const 붙여야 하는가(x) -> 그럼 복사로 처리됨.
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

	int G = 0;	//최종 계산값 F만 있어도 되지만, 디버깅에 좋아서 G와 H추가
	int H = 0;

	int State = 0;	//AStar외에 알고리즘에서, 부가적인 용도로 사용

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
	list<Node*> closelist;//값 비교를 할때 weak_ptr은 == 비교가 안되고 , shared_ptr로 상승시켜야 함으로 좋은 선택이 아니다.
	list<Node*> path;//컨테이너에 값이 있고, 비교검색을 std::find등의 알고리즘으로 찾을때는 raw pointer나 shared_ptr을 사용하자
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