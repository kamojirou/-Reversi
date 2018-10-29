#include <tchar.h>
#include<time.h>
#include<stdio.h>

// �V�X�e��
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
	//�V�[���̓o�^
	sceneManager->RegisterScene(new TitleScene("�^�C�g��"));
	sceneManager->RegisterScene(new PlayScene("�v���C"));
	sceneManager->RegisterScene(new ResultScene("���U���g"));
	sceneManager->RunFirstScene("�^�C�g��");
}

//�`�揈��
void RenderGame()
{
	IDirect3DDevice9 * d3dDevice = Graphics::GetInstance()->GetD3DDevice();
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 149, 237), 1.0, 0);
	d3dDevice->BeginScene();

	//�V�[�����Ƃ̕`�揈��
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
	// �Q�[���p�E�B���h�E�̍쐬
	GameWindow::Startup(1280, 720);

	// �O���t�B�b�N�X�T�u�V�X�e���̏�����
	HWND hWnd = GameWindow::GetInstance()->GetSafeHWnd();
	
	GlobalData::Startup();
	Graphics::Startup(hWnd, 1280, 720);
	SceneManager::Startup();
	TextureManager::Startup();

	InitializeGame();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
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
			// �Q�[���̕`�揈��
			RenderGame();

			// �Q�[���̍X�V����
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
