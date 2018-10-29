#include "TitleScene.h"
#include "Graphics.h"
#include "GameWindow.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "Othello.h"
#include"CollisionDetection.h"

enum class TitleSceneTextureID
{
	BaseID=1000,
	Othelloimg,
	Title,
	Startbutton,
	Exitbutton,
	StartbuttonAfter,
	ExitbuttonAfter
};

TitleScene::TitleScene(const char * name)
	: Scene(name)
{
	m_startbutton = { 970,1218,450,526};
	m_exitbutton = { 970,1218,550,626};
}

void TitleScene::OnCreate()
{
	//�摜�̓ǂݍ���
	TextureManager*texmgr = TextureManager::GetInstance();
	texmgr->RegisterTexture("Assets/othello.jpg", (int)TitleSceneTextureID::Othelloimg);
	texmgr->RegisterTexture("Assets/title.png", (int)TitleSceneTextureID::Title);
	texmgr->RegisterTexture("Assets/startbutton.png", (int)TitleSceneTextureID::Startbutton);
	texmgr->RegisterTexture("Assets/exitbutton.jpg", (int)TitleSceneTextureID::Exitbutton);
	texmgr->RegisterTexture("Assets/a.startbutton.jpg", (int)TitleSceneTextureID::StartbuttonAfter);
	texmgr->RegisterTexture("Assets/a.exitbutton.jpg", (int)TitleSceneTextureID::ExitbuttonAfter);
}


void TitleScene::OnStart()
{
	//�J�[�\�����{�^���̏�ɂȂ���Ԃ�\��
	m_titleMenuIndex = -1;
	//�N���b�N����Ă��Ȃ���Ԃ�\��
	m_prevLButtonPressed = false;
}


void TitleScene::OnUpdate(float deltaTime)
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

	m_titleMenuIndex = -1;

	Point cursorpoint{cursor.x,cursor.y};

	//�J�[�\�����X�^�[�g�{�^���̏�ɂ����Ԃ�\��
	if (PointInRect(m_startbutton,cursorpoint))
	{
		m_titleMenuIndex = 0;
	}
	//�J�[�\�����o���{�^���̏�ɂ����Ԃ�\��
	if (PointInRect(m_exitbutton, cursorpoint))
	{
		m_titleMenuIndex = 1;
	}

	bool currLButtonPressed = (GetKeyState(VK_LBUTTON) & 0x8000) == 0x8000;
	//���̃V�[���֕���
	if (!m_prevLButtonPressed && currLButtonPressed)
	{
		switch (m_titleMenuIndex)
		{
		case 0:
			SceneManager::GetInstance()->ReserveNextScene("�v���C");
			break;
		case 1:
			DestroyWindow(hWnd);
			break;
		}
	}

	m_prevLButtonPressed = currLButtonPressed;
}


void TitleScene::OnRender()
{
	Graphics * g = Graphics::GetInstance();
	TextureManager*texmgr = TextureManager::GetInstance();

	//�w�i
	g->DrawSprite(0, 0, 1280, 720, texmgr->FindTexture((int)TitleSceneTextureID::Othelloimg), 0, 0, 600, 340, D3DCOLOR_ARGB(255, 255, 255, 255));
	//�^�C�g�����S
	g->DrawSprite(50, 75, 506, 117, texmgr->FindTexture((int)TitleSceneTextureID::Title), 0, 0, 506, 117, D3DCOLOR_ARGB(255, 255, 255, 255));

	//�X�^�[�g�{�^��
	if (m_titleMenuIndex == 0)
		g->DrawSprite(m_startbutton.left, m_startbutton.top, m_startbutton.GetWidth(), m_startbutton.GetHeight(), texmgr->FindTexture((int)TitleSceneTextureID::Startbutton), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		g->DrawSprite(m_startbutton.left, m_startbutton.top, m_startbutton.GetWidth(), m_startbutton.GetHeight(), texmgr->FindTexture((int)TitleSceneTextureID::StartbuttonAfter), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));

	//�o���{�^��
	if (m_titleMenuIndex == 1)
		g->DrawSprite(m_exitbutton.left, m_exitbutton.top, m_exitbutton.GetWidth(), m_exitbutton.GetHeight(), texmgr->FindTexture((int)TitleSceneTextureID::Exitbutton), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		g->DrawSprite(m_exitbutton.left, m_exitbutton.top, m_exitbutton.GetWidth(), m_exitbutton.GetHeight(), texmgr->FindTexture((int)TitleSceneTextureID::ExitbuttonAfter), 0, 0, 150, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void TitleScene::OnStop()
{
}

void TitleScene::OnDestroy()
{
	TextureManager * texmgr = TextureManager::GetInstance();
}
