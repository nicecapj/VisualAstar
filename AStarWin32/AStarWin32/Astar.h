// AStar.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
//잘못 이해하고 있던 것 수정
//무조건 시작점에서의 비용을 계산하는 것이 아니라, 현재 노드에서의 비용을 계산하는 것이다.
//현재노드에서의 비용(시작점인 경우, 시작점에서의 비용이 된다)
//현재노드에서 목표까지의 비용
//
//알고리즘
//open리스트에서 가장 낮은 비용의 노드를 선택한다.
//현재 노드가 목적 노드면 길찾기 완료
//현재 노드를 closelist에 넣는다.
//8방향으로 확장한다(방향은 최적화를 위해 4방향 등으로 줄일수 있다.)
//- 확장할때 현재노드로부터의 비용, 현재노드로부터 목표점까지의 비용을 계산하여 f = g(현재부터 비용) + h(목표까지 비용)를 만든다
//open리스트에 없고, close리스트에 없고, 막힌곳이 아니면, 부모를 현재 노드로 설정하고, openlist에 넣는다.
//비용순으로 정렬한다(이 부분도 컨테이너나 정렬 로직을 통해 최적화 가능하다)


//close리스트에 넣는다.
#pragma once
#include "stdafx.h"
#include "Common.h"
//-----------------------------------------------------------------------------------------------
class AStar
{
    public:
        int AStarStart(Vector2DI start, Vector2DI end);


        //console display용
        void PrintPath();
        void PrintMap();

        //win32 display용
        list<Node*> GetOpenList()
        {
            return openlist;
        }
        list<Node*> GetcloseList()
        {
            return closelist;
        }
        list<Node*> GetPath()
        {
            return path;
        }
        auto& GetMapNode()
        {
            return nodeList_;
        }
protected:
		void Initialze(Vector2DI start, Vector2DI end);
    
		virtual void FindPath();
        void SortOpenlist();
        void MakeRoute();
        Node* GetNode(int x, int y);
        Node* MakeChildNode(int x, int y, Node* currentNode);

        std::map<int, shared_ptr<Node>> nodeList_;
        Vector2DI start_;
        Vector2DI end_;
        list<Node*> openlist;
        list<Node*> closelist;//값 비교를 할때 weak_ptr은 == 비교가 안되고 , shared_ptr로 상승시켜야 함으로 좋은 선택이 아니다.
        list<Node*> path;//컨테이너에 값이 있고, 비교검색을 std::find등의 알고리즘으로 찾을때는 raw pointer나 shared_ptr을 사용하자
};