#include <tchar.h>
#include<time.h>
#include<stdio.h>

// システム
#include "GameWindow.h"
#include "Graphics.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "Othello.h"
#include "TextureManager.h"
#include"TitleScene.h"
#include"PlayScene.h"
#include"ResultScene.h"
#include"GlobalData.h"

void InitializeGame()
{
	auto sceneManager = SceneManager::GetInstance();
	//シーンの登録
	sceneManager->RegisterScene(new TitleScene("タイトル"));
	sceneManager->RegisterScene(new PlayScene("プレイ"));
	sceneManager->RegisterScene(new ResultScene("リザルト"));
	sceneManager->RunFirstScene("タイトル");
}

//描画処理
void RenderGame()
{
	IDirect3DDevice9 * d3dDevice = Graphics::GetInstance()->GetD3DDevice();
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 149, 237), 1.0, 0);
	d3dDevice->BeginScene();

	//シーンごとの描画処理
	SceneManager::GetInstance()->RenderCurrentScene();

	d3dDevice->EndScene();
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}


void UpdateGame()
{
	SceneManager::GetInstance()->UpdateCurrentScene(1.0f / 60);
	SceneManager::GetInstance()->TransitScene();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	// ゲーム用ウィンドウの作成
	GameWindow::Startup(1280, 720);

	// グラフィックスサブシステムの初期化
	HWND hWnd = GameWindow::GetInstance()->GetSafeHWnd();
	
	GlobalData::Startup();
	Graphics::Startup(hWnd, 1280, 720);
	SceneManager::Startup();
	TextureManager::Startup();

	InitializeGame();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// ゲームの描画処理
			RenderGame();

			// ゲームの更新処理
			UpdateGame();
		}
	}
    TextureManager::Shutdown();
    SceneManager::Shutdown();
	Graphics::Shutdown();	
	GlobalData::Shutdown();
	GameWindow::Shutdown();

	return (msg.wParam);
}
