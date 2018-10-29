#include "GameWindow.h"
#include <cassert>

// 静的メンバ変数の実体を宣言
GameWindow * GameWindow::s_instance = nullptr;

GameWindow::GameWindow()
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::Startup(uint32_t clientWidth, uint32_t clientHeight)
{
	assert(s_instance == nullptr);
	s_instance = new GameWindow();
	s_instance->Create(clientWidth, clientHeight);
}

void GameWindow::Shutdown()
{
	assert(s_instance != nullptr);
	delete s_instance;
	s_instance = nullptr;
}


HWND GameWindow::GetSafeHWnd() const
{
	return m_hWnd;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}

	return (0L);
}


void GameWindow::Create(uint32_t clientWidth, uint32_t clientHeight)
{
	WNDCLASS wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(nullptr);
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "GP1年ゲームウィンドウ";
	if (!RegisterClass(&wcex))
	{
		assert(0);
		return;
	}

	RECT rect;
	rect.left = 0;
	rect.right = clientWidth;
	rect.top = 0;
	rect.bottom = clientHeight;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	const uint32_t windowW = rect.right - rect.left;
	const uint32_t windowH = rect.bottom - rect.top;

	HWND hWnd;
	hWnd = CreateWindow(
		wcex.lpszClassName,
		"オセロ",
		WS_OVERLAPPEDWINDOW,
		200,		// 初期座標　横
		100,		// 初期座標　縦
		windowW,	// 初期の大きさ　縦
		windowH,	// 初期の大きさ　横
		nullptr,
		nullptr,
		wcex.hInstance,
		nullptr);

	if (!hWnd)
	{
		MessageBox(nullptr, "ウィンドウの作成に失敗しました", "エラー", MB_OK);
		return;
	}

	m_hWnd = hWnd;
}
