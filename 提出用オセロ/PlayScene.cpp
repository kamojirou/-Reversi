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
	//�摜�̓ǂݍ���
	TextureManager * texmgr = TextureManager::GetInstance();
	//�Ֆ�
	texmgr->RegisterTexture("Assets/board.png", (int)PlaySceneTextureID::TexBoard);
	//����
	texmgr->RegisterTexture("Assets/white.png", (int)PlaySceneTextureID::TexWhite);
	//����
	texmgr->RegisterTexture("Assets/black.png", (int)PlaySceneTextureID::TexBlack);
	//�����̐F�\��(��)
	texmgr->RegisterTexture("Assets/turn_black.png", (int)PlaySceneTextureID::MyTurnBlack);
	//�����̐F�\��(��)
	texmgr->RegisterTexture("Assets/turn_white.png", (int)PlaySceneTextureID::MyTurnWhite);
	//�^�[���\��(����)
	texmgr->RegisterTexture("Assets/myturn.png", (int)PlaySceneTextureID::MyTurn);
	//�b�n�l�\��
	texmgr->RegisterTexture("Assets/enemy.png", (int)PlaySceneTextureID::Enemy);
	//�^�[���\��(����)
	texmgr->RegisterTexture("Assets/enemyturn .png", (int)PlaySceneTextureID::EnemyTurn);
	//�w�i
	texmgr->RegisterTexture("Assets/background.png", (int)PlaySceneTextureID::BackGround);

	//����0����9
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

	//�����Ƃɕ�����
	int digit[16];
	//�������L��������
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
	//�J�[�\���̌��݈ʒu�Ɋւ���ϐ�
	int ox = cursor.x - 320;
	int oy = cursor.y - 50;
	int mx = ox / 76;
	int my = oy / 76;

	/*�z�u�p
	char text[256];
	sprintf(text, "(%4d,%4d) (%4d,%4d) (%4d,%4d)", cursor.x, cursor.y, ox, oy, mx, my);
	SetWindowText(hWnd, text);*/

	//�^�[���̐i�s�ɂ���ĕ���
	switch (m_othello->GetState())
	{
	case OthelloState::PlayStart:
	{
		m_othello->TurnStart();
	}
	break;

	case OthelloState::TurnStart:
	{
		// �^�[���ύX���Ɉ�x��������B
		// �΂�u�����Ƃ��ł���ꏊ��S�ĒT���B
		m_othello->SearchPutable(m_othello->turn);
		m_othello->Playing();
	}
	break;

	case OthelloState::Playing:
	{
		if (m_othello->turn == GlobalData::GetInstance()->GetMyColor())
		{
			//�v���C���[�̎v�l���
			m_othello->ThinkTimeForPlayer(ox, oy, mx, my);
		}
		else
		{
			//�b�n�l�̎v�l���
			m_othello->ThinkTimeForCom();
			//�b�n�l�̎v�l����(�ύX��)
			Sleep(500);
		}

		//���s�𔻒�
		m_othello->CountStones();
		if (m_othello->blackstones + m_othello->whitestones >= 64)
		{
			Sleep(1000);
			if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK &&
				m_othello->blackstones > m_othello->whitestones)
			{
				//���������ŏ���
				GlobalData::GetInstance()->SetResult(Result::Win);
			}
			else if (GlobalData::GetInstance()->GetMyColor() == Othello::WHITE &&
				m_othello->whitestones > m_othello->blackstones)
			{
				//���������ŏ���
				GlobalData::GetInstance()->SetResult(Result::Win);
			}
			else if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK &&
				m_othello->blackstones < m_othello->whitestones)
			{
				//���������Ŕs�k
				GlobalData::GetInstance()->SetResult(Result::Lose);
			}
			else
			{
				//���������ŏ���
				GlobalData::GetInstance()->SetResult(Result::Lose);
			}
			//���΂Ɣ��΂̐������̃V�[���Ɏ󂯓n�����߂̑��
			GlobalData::GetInstance()->SetBlackStone(m_othello->blackstones);
			GlobalData::GetInstance()->SetWhiteStone(m_othello->whitestones);
			m_othello->PlayEnd();
		}
	}
	break;

	case OthelloState::TurnEnd:
	{
		// �Ֆʂɂ��邨�݂��̐΂̐��̕\�����X�V
		m_othello->TurnStart();
		break;
	}
	case OthelloState::PlayEnd:
	{
		//���U���g��ʂɈړ�
		SceneManager::GetInstance()->ReserveNextScene("���U���g");
	}
	break;
	}
}


void PlayScene::OnRender()
{
	Graphics * g = Graphics::GetInstance();
	TextureManager*texmgr = TextureManager::GetInstance();

	//�w�i�摜
	g->DrawSprite(0, 0, 1280, 720, texmgr->FindTexture((int)PlaySceneTextureID::BackGround), 0, 0, 1280, 720, D3DCOLOR_ARGB(255, 255, 255, 255));
	//�Ֆ�
	g->DrawSprite(320, 50, 613, 613, texmgr->FindTexture((int)PlaySceneTextureID::TexBoard), 0, 0, 613, 613, D3DCOLOR_ARGB(255, 255, 255, 255));
	//�u�����v
	if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK)
		g->DrawSprite(955, 435, 300, 50, texmgr->FindTexture((int)PlaySceneTextureID::MyTurnBlack), 0, 0, 300, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		g->DrawSprite(955, 435, 300, 50, texmgr->FindTexture((int)PlaySceneTextureID::MyTurnWhite), 0, 0, 300, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	//�u�b�n�l�v
	g->DrawSprite(0, 50, 100, 50, texmgr->FindTexture((int)PlaySceneTextureID::Enemy), 0, 0, 100, 50, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_othello->turn == GlobalData::GetInstance()->GetMyColor())
	{
		//�����̃^�[���\��
		g->DrawSprite(943, 460, 300, 50, texmgr->FindTexture((int)PlaySceneTextureID::MyTurn), 0, 0, 300, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		//�G�̃^�[���\��
		g->DrawSprite(0, 25, 250, 50, texmgr->FindTexture((int)PlaySceneTextureID::EnemyTurn), 0, 0, 250, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (m_othello->board[y][x] == Othello::WHITE)
				//����
				g->DrawSprite(x * 77.0f + 325, y * 77.0f + 56, STONE, STONE, texmgr->FindTexture((int)PlaySceneTextureID::TexWhite), 0, 0, 64, 64, D3DCOLOR_ARGB(255, 255, 255, 255));
			else if (m_othello->board[y][x] == Othello::BLACK)
				//����
				g->DrawSprite(x * 77.0f + 325, y * 77.0f + 56, STONE, STONE, texmgr->FindTexture((int)PlaySceneTextureID::TexBlack), 0, 0, 64, 64, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// �u�����Ƃ��ł���ꏊ(�K�C�h)�̕\��

	for (int i = 0; i < m_othello->count; i++)
	{
		g->FillRect(m_othello->putable[i].x*76.5f + 322, m_othello->putable[i].y*76.5f + 52, 75, 75, D3DCOLOR_ARGB(200, 255, 255, 0));
	}

	//���ݒu���Ă��鍕�Ɣ��̐΂����ꂼ��\��
	if (GlobalData::GetInstance()->GetMyColor() == Othello::BLACK)
	{
		//���������̏ꍇ
		RenderNumbers(g, 30, 120, m_othello->whitestones);
		RenderNumbers(g, 1000, 530, m_othello->blackstones);
	}
	else
	{
		//���������̏ꍇ
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
