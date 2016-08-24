#include "stdafx.h"


int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//������ â ��� ����
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;	//Window Class Styles
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//������ â������� ���
	if ( !RegisterClassEx(&wcex) )
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed!", WINDOW_TITLE_CAPTION, MB_OK);

		return 1;
	}
	
	//������ ������ ����
	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, WINDOW_CLASS_NAME, WINDOW_TITLE_CAPTION,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU,//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		0, 0, RESOLUTION_X, RESOLUTION_Y, NULL, NULL, hInstance, NULL);
	if ( !hWnd )
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", WINDOW_TITLE_CAPTION, MB_OK);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	UpdateWindow(hWnd);

	
	GameManager::Get().Initialize(hWnd);

	MSG message = MSG();
	while ( message.message != WM_QUIT )
	{
		if ( PeekMessageW(&message, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			//Ÿ�Ӱ�Ÿ�� ���
			static DWORD prevTime = timeGetTime();
			DWORD nowTime = timeGetTime();

			double tickSecond = ( nowTime - prevTime ) * 0.001f;

			GameManager::Get().Loop(tickSecond);

			prevTime = nowTime;
		}
	}

	GameManager::Get().Destroy();

	return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch ( message )
	{
		case WM_CREATE:
			{
				RECT wantRect = { 0, 0, RESOLUTION_X, RESOLUTION_Y };
				AdjustWindowRect(&wantRect, WS_CAPTION | WS_SYSMENU, false); //������ ũ�⸦ ���( ���ϴ� Ŭ���̾�Ʈ ����, ������ ��Ÿ��, �޴� ���ԵǴ���)
				MoveWindow(hWnd, 0, 0, wantRect.right - wantRect.left, wantRect.bottom - wantRect.top, true);	//������ ũ�� ����(�������� �ٽ� �׸�����)
			}
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
			GameManager::Get().GameInputProc(hWnd, message, wParam, lParam);
			break;
		case WM_KEYDOWN:
			if ( wParam == VK_ESCAPE )
			{
				::DestroyWindow(hWnd);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}