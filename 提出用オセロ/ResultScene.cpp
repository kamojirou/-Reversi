#include "ResultScene.h"
#include "Graphics.h"
#include "GameWindow.h"
#include "PlayScene.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Othello.h"
#include "GlobalData.h"
#include <tchar.h>
#include"CollisionDetection.h"
enum class ResultSceneTextureID
{
	BaseID=3000,
	WinBackGround,
	LoseBackGround,
	Cracker,
	Cracker2,
	Paper,
	Win,
	Lose,
	Mystone,
	Comstone,
	Retrybutton,
	RetrybuttonAfter,
	Rain,
	Sorrow,
	Exitbutton,
	ExitbuttonAfter,
	Number
};


ResultScene::ResultScene(const char * name)
	: Scene(name)
{
	m_retrybutton = { 420,570,525,575};
	m_exitbutton = {750,900,525,575};
}

void ResultScene::RenderNumbers(Graphics * g, int x, int y, int value)
{
	TextureManager * texmgr = TextureManager::GetInstance();

	//桁ごとに分ける
	int digit[16];
	int count = 0;

	while (value>0)
	{
		digit[count] = value % 10;
		value /= 10;
		count++;
	}

	for (int i = 0; i < count; i++)
	{
		g->DrawSprite((float)i * 70 + x, (float)y, 70, 100, m_numberTexture[digit[count - 1 - i]], 0, 0,65, 100, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}


void ResultScene::OnCreate()
{
		//画像の読み込み
		TextureManager*texmgr = TextureManager::GetInstance();
		texmgr->RegisterTexture("Assets/win_background.png", (int)ResultSceneTextureID::WinBackGround);
		texmgr->RegisterTexture("Assets/lose_background.png", (int)ResultSceneTextureID::LoseBackGround);
		texmgr->RegisterTexture("Assets/cracker.png", (int)ResultSceneTextureID::Cracker);
		texmgr->RegisterTexture("Assets/cracker2.png", (int)ResultSceneTextureID::Cracker2);
		texmgr->RegisterTexture("Assets/paper_fall2_02.png", (int)ResultSceneTextureID::Paper);
		texmgr->RegisterTexture("Assets/win.png", (int)ResultSceneTextureID::Win);
		texmgr->RegisterTexture("Assets/lose.png", (int)ResultSceneTextureID::Lose);
		texmgr->RegisterTexture("Assets/mystone.png", (int)ResultSceneTextureID::Mystone);
		texmgr->RegisterTexture("Assets/comstone.png", (int)ResultSceneTextureID::Comstone);
		texmgr->RegisterTexture("Assets/retryburtton.png", (int)ResultSceneTextureID::Retrybutton);
		texmgr->RegisterTexture("Assets/a.retry.png", (int)ResultSceneTextureID::RetrybuttonAfter);
		texmgr->RegisterTexture("Assets/rain.png", (int)ResultSceneTextureID::Rain);
		texmgr->RegisterTexture("Assets/sorrow.png", (int)ResultSceneTextureID::Sorrow);
		texmgr->RegisterTexture("Assets/exitbutton.jpg", (int)ResultSceneTextureID::Exitbutton);
		texmgr->RegisterTexture("Assets/a.exitbutton.jpg", (int)ResultSceneTextureID::ExitbuttonAfter);
		
		for (int i = 0; i < 10; i++)
		{
			char path[256];
			sprintf_s(path, "Assets/%d.png", i);
			m_numberTexture[i] = texmgr->RegisterTexture(path, (int)ResultSceneTextureID::Number);
		}


}

void ResultScene::OnStart()
{
	m_resultMenuIndex = -1;
	m_prevLButtonPressed = false;
}

void ResultScene::OnUpdate(float deltaTime)
{
	HWND hWnd = GameWindow::GetInstance()->GetSafeHWnd();

	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(hWnd, &cursor);

	int ox = cursor.x - 320;
	int oy = cursor.y - 50;
	int mx = ox / 76;
	int my = oy / 76;

	/*配置用
	char text[256];
	sprintf(text, "(%4d,%4d) (%4d,%4d) (%4d,%4d)", cursor.x, cursor.y, ox, oy, mx, my);
	SetWindowText(hWnd, text);*/


	//シーン移動の判断
	m_resultMenuIndex = -1;

	Point cursorpoint{ cursor.x,cursor.y };

	if (PointInRect(m_retrybutton, cursorpoint))
	{
		m_resultMenuIndex = 0;
	}

	if (PointInRect(m_exitbutton, cursorpoint))
	{
		m_resultMenuIndex = 1;
	}

	bool currLButtonPressed = (GetKeyState(VK_LBUTTON) & 0x8000) == 0x8000;
	if (!m_prevLButtonPressed && currLButtonPressed)
	{
		switch (m_resultMenuIndex)
		{
		case 0:
			SceneManager::GetInstance()->ReserveNextScene("プレイ");
			break;
		case 1:
			DestroyWindow(hWnd);
			break;
		}
	}
	m_prevLButtonPressed = currLButtonPressed;

}

void ResultScene::OnRender()
{
	Graphics * g = Graphics::GetInstance();
	TextureManager*texmgr = TextureManager::GetInstance();

	//背景画像
	if (GlobalData::GetInstance()->GetResult() == Result::Win)
	{
		//背景
		g->DrawSprite(0, 0, 1280, 720, texmgr->FindTexture((int)ResultSceneTextureID::WinBackGround), 0, 0, 1280, 720, D3DCOLOR_ARGB(125, 255, 255, 255));
		//クラッカー
		g->DrawSprite(0, 300, 248 * 2, 203 * 2, texmgr->FindTexture((int)ResultSceneTextureID::Cracker), 0, 0, 248, 203, D3DCOLOR_ARGB(255, 255, 255, 255));
		g->DrawSprite(790, 300, 248 * 2, 203 * 2, texmgr->FindTexture((int)ResultSceneTextureID::Cracker2), 0, 0, 248, 203, D3DCOLOR_ARGB(255, 255, 255, 255));
		//紙吹雪
		g->DrawSprite(0, 0, 1280, 720, texmgr->FindTexture((int)ResultSceneTextureID::Paper), 0, 0, 800, 450, D3DCOLOR_ARGB(155, 255, 255, 255));
		//フィルタ
		g->FillRect(260, 120, 1050 - 260, 600 - 120, D3DCOLOR_ARGB(150, 125, 0, 0));
		//勝利文字
		g->DrawSprite(285, 135, 735, 117, texmgr->FindTexture((int)ResultSceneTextureID::Win), 0, 0, 735, 117, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		//背景
		g->DrawSprite(0, 0, 1280, 720, texmgr->FindTexture((int)ResultSceneTextureID::LoseBackGround), 0, 0, 1280, 720, D3DCOLOR_ARGB(255, 255, 255, 255));
		//悲しみ
		g->DrawSprite(0, 250, 512, 512, texmgr->FindTexture((int)ResultSceneTextureID::Sorrow), 0, 0, 512, 512, D3DCOLOR_ARGB(255, 255, 255, 255));
		//フィルタ
		g->FillRect(260, 120, 1050 - 260, 600 - 120, D3DCOLOR_ARGB(150, 185, 206, 235));
		//敗北文字
		g->DrawSprite(285, 135, 760, 117, texmgr->FindTexture((int)ResultSceneTextureID::Lose), 0, 0, 760, 117, D3DCOLOR_ARGB(155, 255, 255, 255));
	}

	//自分の石の数
	g->DrawSprite(380, 330, 347, 50, texmgr->FindTexture((int)ResultSceneTextureID::Mystone), 0, 0, 347, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//comの石の数
	g->DrawSprite(410, 440, 318, 50, texmgr->FindTexture((int)ResultSceneTextureID::Comstone) , 0, 0, 318, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//リトライボタンアフター
	if (m_resultMenuIndex == 0)
		g->DrawSprite(m_retrybutton.left, m_retrybutton.top, m_retrybutton.GetWidth(), m_retrybutton.GetHeight(), texmgr->FindTexture((int)ResultSceneTextureID::RetrybuttonAfter), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//リトライボタン
	else
		g->DrawSprite(m_retrybutton.left, m_retrybutton.top, m_retrybutton.GetWidth(), m_retrybutton.GetHeight(), texmgr->FindTexture((int)ResultSceneTextureID::Retrybutton), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//出口ボタンアフター
	if (m_resultMenuIndex == 1)
		g->DrawSprite(m_exitbutton.left, m_exitbutton.top, m_exitbutton.GetWidth(), m_exitbutton.GetHeight(), texmgr->FindTexture((int)ResultSceneTextureID::ExitbuttonAfter), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//出口ボタン
	else
		g->DrawSprite(m_exitbutton.left, m_exitbutton.top, m_exitbutton.GetWidth(), m_exitbutton.GetHeight(), texmgr->FindTexture((int)ResultSceneTextureID::Exitbutton), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));

	//石の数の描画
	if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK)
	{
		RenderNumbers(g, 825, 295, GlobalData::GetInstance()->GetBlackStone());
		RenderNumbers(g, 825, 405, GlobalData::GetInstance()->GetWhiteStone());
	}
	else
	{
		RenderNumbers(g, 825, 295, GlobalData::GetInstance()->GetWhiteStone());
		RenderNumbers(g, 825, 405, GlobalData::GetInstance()->GetBlackStone());
	}
}

void ResultScene::OnStop()
{
}

void ResultScene::OnDestroy()
{
}

