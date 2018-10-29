#pragma once
#include "Scene.h"
#include"Point.h"
#include"Rect.h"


class TitleScene : public Scene
{
private:
	//�J�[�\�����{�^���̏�ɂ��邩���f����ϐ�
	int m_titleMenuIndex;
	//�N���b�N���ꂽ�����f����ϐ�
	bool m_prevLButtonPressed;

	//�{�^���̈ʒu����ۑ�����ϐ�
	Rect m_startbutton;
	Rect m_exitbutton;


public:
	TitleScene(const char * name);
	virtual void OnCreate() override;
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnStop() override;
	virtual void OnDestroy() override;


};

 