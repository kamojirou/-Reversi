#include "PlayScene.h"
#include "Graphics.h"
#include "GameWindow.h"
#include "Othello.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "GlobalData.h"
#include <tchar.h>

enum class PlaySceneTextureID
{
	BaseID = 2000,
	TexBoard,
	TexWhite,
	TexBlack,
	MyTurnBlack,
	MyTurnWhite,
	MyTurn,
	Enemy,
	EnemyTurn,
	BackGround,
	Number
};

PlayScene::PlayScene(const char * name)
	: Scene(name)
{
}

void PlayScene::OnCreate()
{
	//画像の読み込み
	TextureManager * texmgr = TextureManager::GetInstance();
	//盤面
	texmgr->RegisterTexture("Assets/board.png", (int)PlaySceneTextureID::TexBoard);
	//白石
	texmgr->RegisterTexture("Assets/white.png", (int)PlaySceneTextureID::TexWhite);
	//黒石
	texmgr->RegisterTexture("Assets/black.png", (int)PlaySceneTextureID::TexBlack);
	//自分の色表示(黒)
	texmgr->RegisterTexture("Assets/turn_black.png", (int)PlaySceneTextureID::MyTurnBlack);
	//自分の色表示(白)
	texmgr->RegisterTexture("Assets/turn_white.png", (int)PlaySceneTextureID::MyTurnWhite);
	//ターン表示(自分)
	texmgr->RegisterTexture("Assets/myturn.png", (int)PlaySceneTextureID::MyTurn);
	//ＣＯＭ表示
	texmgr->RegisterTexture("Assets/enemy.png", (int)PlaySceneTextureID::Enemy);
	//ターン表示(相手)
	texmgr->RegisterTexture("Assets/enemyturn .png", (int)PlaySceneTextureID::EnemyTurn);
	//背景
	texmgr->RegisterTexture("Assets/background.png", (int)PlaySceneTextureID::BackGround);

	//数字0から9
	for (int i = 0; i < 10; i++)
	{
		char path[256];
		sprintf_s(path, "Assets/%d.png", i);
		m_numberTexture[i] = texmgr->RegisterTexture(path, (int)PlaySceneTextureID::Number);
	}

	m_othello = new Othello();
}

void PlayScene::RenderNumbers(Graphics * g, int x, int y, int value)
{
	TextureManager * texmgr = TextureManager::GetInstance();

	//桁ごとに分ける
	int digit[16];
	//何桁か記憶させる
	int count = 0;

	while (value>0)
	{
		digit[count] = value % 10;
		value /= 10;
		count++;
	}
	for (int i = 0; i < count; i++)
	{
		g->DrawSprite(i * 70.0f + x, (float)y, 70, 100,m_numberTexture[digit[count - 1 - i]], 0, 0, 65, 100, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}


void PlayScene::OnStart()
{
	m_othello->ResetBoard();
	m_othello->ColorDecide();
	m_othello->CountStones();
	m_othello->PlayStart();
}


void PlayScene::OnUpdate(float deltaTime)
{
	HWND hWnd = GameWindow::GetInstance()->GetSafeHWnd();

	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(hWnd, &cursor);
	//カーソルの現在位置に関する変数
	int ox = cursor.x - 320;
	int oy = cursor.y - 50;
	int mx = ox / 76;
	int my = oy / 76;

	/*配置用
	char text[256];
	sprintf(text, "(%4d,%4d) (%4d,%4d) (%4d,%4d)", cursor.x, cursor.y, ox, oy, mx, my);
	SetWindowText(hWnd, text);*/

	//ターンの進行によって分岐
	switch (m_othello->GetState())
	{
	case OthelloState::PlayStart:
	{
		m_othello->TurnStart();
	}
	break;

	case OthelloState::TurnStart:
	{
		// ターン変更時に一度だけ来る。
		// 石を置くことができる場所を全て探す。
		m_othello->SearchPutable(m_othello->turn);
		m_othello->Playing();
	}
	break;

	case OthelloState::Playing:
	{
		if (m_othello->turn == GlobalData::GetInstance()->GetMyColor())
		{
			//プレイヤーの思考場面
			m_othello->ThinkTimeForPlayer(ox, oy, mx, my);
		}
		else
		{
			//ＣＯＭの思考場面
			m_othello->ThinkTimeForCom();
			//ＣＯＭの思考時間(変更可)
			Sleep(500);
		}

		//勝敗を判定
		m_othello->CountStones();
		if (m_othello->blackstones + m_othello->whitestones >= 64)
		{
			Sleep(1000);
			if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK &&
				m_othello->blackstones > m_othello->whitestones)
			{
				//自分が黒で勝利
				GlobalData::GetInstance()->SetResult(Result::Win);
			}
			else if (GlobalData::GetInstance()->GetMyColor() == Othello::WHITE &&
				m_othello->whitestones > m_othello->blackstones)
			{
				//自分が白で勝利
				GlobalData::GetInstance()->SetResult(Result::Win);
			}
			else if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK &&
				m_othello->blackstones < m_othello->whitestones)
			{
				//自分が黒で敗北
				GlobalData::GetInstance()->SetResult(Result::Lose);
			}
			else
			{
				//自分が白で勝利
				GlobalData::GetInstance()->SetResult(Result::Lose);
			}
			//黒石と白石の数を次のシーンに受け渡すための代入
			GlobalData::GetInstance()->SetBlackStone(m_othello->blackstones);
			GlobalData::GetInstance()->SetWhiteStone(m_othello->whitestones);
			m_othello->PlayEnd();
		}
	}
	break;

	case OthelloState::TurnEnd:
	{
		// 盤面にあるお互いの石の数の表示を更新
		m_othello->TurnStart();
		break;
	}
	case OthelloState::PlayEnd:
	{
		//リザルト画面に移動
		SceneManager::GetInstance()->ReserveNextScene("リザルト");
	}
	break;
	}
}


void PlayScene::OnRender()
{
	Graphics * g = Graphics::GetInstance();
	TextureManager*texmgr = TextureManager::GetInstance();

	//背景画像
	g->DrawSprite(0, 0, 1280, 720, texmgr->FindTexture((int)PlaySceneTextureID::BackGround), 0, 0, 1280, 720, D3DCOLOR_ARGB(255, 255, 255, 255));
	//盤面
	g->DrawSprite(320, 50, 613, 613, texmgr->FindTexture((int)PlaySceneTextureID::TexBoard), 0, 0, 613, 613, D3DCOLOR_ARGB(255, 255, 255, 255));
	//「自分」
	if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK)
		g->DrawSprite(955, 435, 300, 50, texmgr->FindTexture((int)PlaySceneTextureID::MyTurnBlack), 0, 0, 300, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		g->DrawSprite(955, 435, 300, 50, texmgr->FindTexture((int)PlaySceneTextureID::MyTurnWhite), 0, 0, 300, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//「ＣＯＭ」
	g->DrawSprite(0, 50, 100, 50, texmgr->FindTexture((int)PlaySceneTextureID::Enemy), 0, 0, 100, 50, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_othello->turn == GlobalData::GetInstance()->GetMyColor())
	{
		//自分のターン表示
		g->DrawSprite(943, 460, 300, 50, texmgr->FindTexture((int)PlaySceneTextureID::MyTurn), 0, 0, 300, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		//敵のターン表示
		g->DrawSprite(0, 25, 250, 50, texmgr->FindTexture((int)PlaySceneTextureID::EnemyTurn), 0, 0, 250, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (m_othello->board[y][x] == Othello::WHITE)
				//白石
				g->DrawSprite(x * 77.0f + 325, y * 77.0f + 56, STONE, STONE, texmgr->FindTexture((int)PlaySceneTextureID::TexWhite), 0, 0, 64, 64, D3DCOLOR_ARGB(255, 255, 255, 255));
			else if (m_othello->board[y][x] == Othello::BLACK)
				//黒石
				g->DrawSprite(x * 77.0f + 325, y * 77.0f + 56, STONE, STONE, texmgr->FindTexture((int)PlaySceneTextureID::TexBlack), 0, 0, 64, 64, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// 置くことができる場所(ガイド)の表示

	for (int i = 0; i < m_othello->count; i++)
	{
		g->FillRect(m_othello->putable[i].x*76.5f + 322, m_othello->putable[i].y*76.5f + 52, 75, 75, D3DCOLOR_ARGB(200, 255, 255, 0));
	}

	//現在置いてある黒と白の石をそれぞれ表示
	if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK)
	{
		//自分が黒の場合
		RenderNumbers(g, 30, 120, m_othello->whitestones);
		RenderNumbers(g, 1000, 530, m_othello->blackstones);
	}
	else
	{
		//自分が白の場合
		RenderNumbers(g, 30, 120, m_othello->blackstones);
		RenderNumbers(g, 1000, 530, m_othello->whitestones);
	}
}

void PlayScene::OnStop()
{
}

void PlayScene::OnDestroy()
{
}
