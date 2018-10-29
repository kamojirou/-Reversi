#pragma once
#include "Scene.h"
#include "Texture2D.h"
#include "Graphics.h"

class Othello;

class PlayScene : public Scene
{
private:
	//�����̃e�N�X�`���p�̓��ꕨ
	Texture2D * m_numberTexture[10];
    //�΂̃e�N�X�`���̑傫��64px*64px
	static const char STONE =64;

	//�I�Z���N���X���p�̂��߂̓��ꕨ
	Othello * m_othello;

public:
	PlayScene(const char * name);
	virtual void OnCreate() override;
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnStop() override;
	virtual void OnDestroy() override;

	// �΂̐����L��������֐�
	void RenderNumbers(Graphics * g, int x, int y, int value);
};

