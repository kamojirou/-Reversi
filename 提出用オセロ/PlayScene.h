#pragma once
#include "Scene.h"
#include "Texture2D.h"
#include "Graphics.h"

class Othello;

class PlayScene : public Scene
{
private:
	//数字のテクスチャ用の入れ物
	Texture2D * m_numberTexture[10];
    //石のテクスチャの大きさ64px*64px
	static const char STONE =64;

	//オセロクラス利用のための入れ物
	Othello * m_othello;

public:
	PlayScene(const char * name);
	virtual void OnCreate() override;
	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnStop() override;
	virtual void OnDestroy() override;

	// 石の数を記憶させる関数
	void RenderNumbers(Graphics * g, int x, int y, int value);
};

