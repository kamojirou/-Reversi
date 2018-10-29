#pragma once
#include "Scene.h"
#include "Texture2D.h"
#include "Graphics.h"
#include"Point.h"
#include"Rect.h"

class PlayScene;

class Othello;

class ResultScene : public Scene
{
	private:
		//�J�[�\�����{�^���̏�ɂ��邩���f����ϐ�
		int m_resultMenuIndex;
		//�����̃e�N�X�`���̕ۑ�����z��
		Texture2D * m_numberTexture[10];
		bool m_prevLButtonPressed;
		Othello * m_othello;

		//�{�^���̈ʒu����ۑ�����ϐ�
		Rect m_retrybutton;
		Rect m_exitbutton;

public:
		ResultScene(const char * name);
		virtual void OnCreate() override;
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnStop() override;
		virtual void OnDestroy() override;

		// �΂̐����L��������֐�
		void RenderNumbers(Graphics * g, int x, int y, int value);

};
