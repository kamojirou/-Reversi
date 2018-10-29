#pragma once
#include "Scene.h"
#include"Point.h"
#include"Rect.h"


class TitleScene : public Scene
{
private:
	//カーソルがボタンの上にあるか判断する変数
	int m_titleMenuIndex;
	//クリックされたか判断する変数
	bool m_prevLButtonPressed;

	//ボタンの位置情報を保存する変数
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

 