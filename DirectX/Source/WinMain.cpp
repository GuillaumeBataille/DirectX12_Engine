#include "Precompiled_header.h"

/*----------------------------------------------*/
/*   Author : mylgasia - Guillaume Bataille     */
/*   Licence : MIT Licence                      */
/*----------------------------------------------*/

/*------ Global Variables */
#pragma region GlobalVariables

WCHAR WindowClass[MAX_NAME_STRING];
WCHAR WindowTitle[MAX_NAME_STRING];
INT WindowWidth;
INT WindowHeight;
HICON Icon;

#pragma endregion

/*------ Declarations */
#pragma region Declarations

VOID InitWinMainVar();
VOID CreateWindowClass();
VOID InitWindow();
VOID MessageListener();
LRESULT CALLBACK WindowProcess(HWND w_inst, UINT w_msg, WPARAM w_param, LPARAM l_param);
#pragma endregion

/*------ Main program */
#pragma region Main
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	InitWinMainVar();
	CreateWindowClass();
	InitWindow();
	MessageListener();	
	return 0;
};

/* A window processing system */
LRESULT CALLBACK WindowProcess(HWND w_inst, UINT w_msg, WPARAM w_param, LPARAM l_param)
{
	std::cout << "Message type : " << w_msg << std::endl;
	switch (w_msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(w_inst, w_msg, w_param, l_param);
}

#pragma endregion

/*------Functions */
#pragma region Functions

/* Initiate variables to handle main*/
VOID InitWinMainVar()
{
	LoadString(HInstance(), IDS_GAMENAME, WindowClass, MAX_NAME_STRING);
	LoadString(HInstance(), IDS_WINDOWNAME, WindowTitle, MAX_NAME_STRING);
	WindowWidth = WIDTH;
	WindowHeight = HEIGHT;
	Icon = LoadIcon(HInstance(), MAKEINTRESOURCE(101));
}

/* Create a window instance */
VOID CreateWindowClass()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

	wcex.hIcon = Icon;
	wcex.hIconSm = Icon;

	wcex.lpszMenuName = WindowClass;

	wcex.hInstance = HInstance();

	wcex.lpfnWndProc = WindowProcess;

	wcex.lpszClassName = WindowClass;

	RegisterClassEx(&wcex);
}

/* Generate a window */
VOID InitWindow()
{
	HWND hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, WindowWidth, WindowHeight, nullptr, nullptr, HInstance(), nullptr);
	if (!hWnd)
	{
		MessageBox(0, L"Failed to Create Window!", 0, 0);
		PostQuitMessage(0);
	}
	ShowWindow(hWnd, SW_SHOW);
}

/* A listener loop on messages */
VOID MessageListener()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

#pragma endregion