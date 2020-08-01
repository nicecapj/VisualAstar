// AStarWin32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AStarWin32.h"
#include "Astar.h"
#include "Jps.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASTARWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTARWIN32));

    MSG msg;
	
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTARWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASTARWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

void DrawAStarPath(AStar &astar, HDC hdc)
{
	HBRUSH oldBrush = nullptr;
	HBRUSH wallBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH roadBrush = CreateSolidBrush(RGB(0, 255, 0));

	const int RECT_SIZE = 100;
	//map
	for (auto mapNode : astar.GetMapNode())
	{
		int y = mapNode.first / WorldManager::MAP_MAX_Y;
		int x = mapNode.first % WorldManager::MAP_MAX_Y;

		if (mapNode.second->ObjectType == WorldManager::EObjectType::Wall)
		{
			oldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, wallBrush));
			Rectangle(hdc, x*RECT_SIZE, y * RECT_SIZE, x * RECT_SIZE + RECT_SIZE, y * RECT_SIZE + RECT_SIZE);
			SelectObject(hdc, oldBrush);
		}
		else
		{
			oldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, roadBrush));
			Rectangle(hdc, x*RECT_SIZE, y * RECT_SIZE, x * RECT_SIZE + RECT_SIZE, y * RECT_SIZE + RECT_SIZE);
			SelectObject(hdc, oldBrush);
		}

		char buff[256];
		_itoa_s(mapNode.second->Index, buff, 10);
		wstring strID = s2ws(buff);
		strID += TEXT("] ");
		_itoa_s(mapNode.second->G, buff, 10);
		wstring strG = s2ws(buff);
		strG += TEXT("+");
		strID += strG;
		_itoa_s(mapNode.second->H, buff, 10);
		wstring strH = s2ws(buff);
		strH += TEXT("=");
		strID += strH;
		_itoa_s(mapNode.second->F, buff, 10);
		wstring strF = s2ws(buff);
		strID += strF;


		TextOut(hdc, x*RECT_SIZE, y*RECT_SIZE, strID.c_str(), static_cast<int>(strID.size()));
		//TextOut(hdc, x*RECT_SIZE+20, y*RECT_SIZE, strG.c_str(), strG.size());
		//TextOut(hdc, x*RECT_SIZE+40, y*RECT_SIZE, strH.c_str(), strH.size());
		//TextOut(hdc, x*RECT_SIZE+60, y*RECT_SIZE, strF.c_str(), strF.size());
	}

	int index = 0;
	char buff[256];
	for (auto node : astar.GetPath())
	{
		int y = node->Index / WorldManager::MAP_MAX_Y;
		int x = node->Index % WorldManager::MAP_MAX_Y;
		Ellipse(hdc, x*RECT_SIZE + 20, y * RECT_SIZE + 20, x * RECT_SIZE + RECT_SIZE - 20, y * RECT_SIZE + RECT_SIZE - 20);

		++index;
		_itoa_s(index, buff, 10);
		wstring str = s2ws(buff);
		TextOut(hdc, x*RECT_SIZE + 40, y * RECT_SIZE + 40, str.c_str(), static_cast<int>(str.size()));
	}

	int count = 0;
	for (auto node : TraceManager::Get()->traceInfo)
	{
		_itoa_s(node.Current->Index, buff, 10);
		wstring id = s2ws(buff);
		id += TEXT("]");

		wstring path;
		for (auto pathnode : node.path)
		{
			_itoa_s(pathnode->Index, buff, 10);
			wstring str = s2ws(buff);
			path += str;
			path += TEXT(" ");
		}

		wstring open;
		for (auto opennode : node.openlist)
		{
			_itoa_s(opennode->Index, buff, 10);
			wstring str = s2ws(buff);
			open += str;
			open += TEXT(" ");
		}

		wstring close;
		for (auto closenode : node.closelist)
		{
			_itoa_s(closenode->Index, buff, 10);
			wstring str = s2ws(buff);
			close += str;
			close += TEXT(" ");
		}

		int col = count++ * 80;
		TextOut(hdc, 800, 10 + col, id.c_str(), static_cast<int>(id.size()));
		TextOut(hdc, 800, 30 + col, open.c_str(), static_cast<int>(open.size()));
		TextOut(hdc, 800, 50 + col, close.c_str(), static_cast<int>(close.size()));
		//TextOut(hdc, 800, 110 + col, path.c_str(), path.size());
	}

	DeleteObject(wallBrush);
	DeleteObject(roadBrush);
}

void DrawJPSPath(AStar &jps, HDC hdc)
{	
	const int drawOffsetY = 510;

	HBRUSH oldBrush = nullptr;
	HBRUSH wallBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH roadBrush = CreateSolidBrush(RGB(0, 255, 0));

	const int RECT_SIZE = 100;
	//map
	for (auto mapNode : jps.GetMapNode())
	{
		int y = mapNode.first / WorldManager::MAP_MAX_Y;
		int x = mapNode.first % WorldManager::MAP_MAX_Y;

		if (mapNode.second->ObjectType == WorldManager::EObjectType::Wall)
		{
			oldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, wallBrush));
			Rectangle(hdc, x*RECT_SIZE, y * RECT_SIZE + drawOffsetY, x * RECT_SIZE + RECT_SIZE, y * RECT_SIZE + RECT_SIZE + drawOffsetY);
			SelectObject(hdc, oldBrush);
		}
		else
		{
			oldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, roadBrush));
			Rectangle(hdc, x*RECT_SIZE, y * RECT_SIZE + drawOffsetY, x * RECT_SIZE + RECT_SIZE, y * RECT_SIZE + RECT_SIZE + drawOffsetY);
			SelectObject(hdc, oldBrush);
		}

		char buff[256];
		_itoa_s(mapNode.second->Index, buff, 10);
		wstring strID = s2ws(buff);
		strID += TEXT("] ");
		_itoa_s(mapNode.second->G, buff, 10);
		wstring strG = s2ws(buff);
		strG += TEXT("+");
		strID += strG;
		_itoa_s(mapNode.second->H, buff, 10);
		wstring strH = s2ws(buff);
		strH += TEXT("=");
		strID += strH;
		_itoa_s(mapNode.second->F, buff, 10);
		wstring strF = s2ws(buff);
		strID += strF;

		TextOut(hdc, x*RECT_SIZE, y*RECT_SIZE + drawOffsetY, strID.c_str(), static_cast<int>(strID.size()));
	}

	int index = 0;
	for (auto node : jps.GetPath())
	{
		int y = node->Index / WorldManager::MAP_MAX_Y;
		int x = node->Index % WorldManager::MAP_MAX_Y;
		Ellipse(hdc, x*RECT_SIZE + 20, y * RECT_SIZE + 20 + drawOffsetY, x * RECT_SIZE + RECT_SIZE - 20, y * RECT_SIZE + RECT_SIZE - 20 + drawOffsetY);

		++index;
		
	}

	DeleteObject(wallBrush);
	DeleteObject(roadBrush);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

			AStar astar;
			astar.AStarStart(Vector2DI(0, 0), Vector2DI(4, 4));
			DrawAStarPath(astar, hdc);

			Jps jps;
			jps.JpsStart(Vector2DI(0, 0), Vector2DI(4, 4));
			DrawJPSPath(jps, hdc);			

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
