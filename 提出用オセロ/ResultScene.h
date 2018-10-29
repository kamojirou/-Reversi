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
		//カーソルがボタンの上にあるか判断する変数
		int m_resultMenuIndex;
		//数字のテクスチャの保存する配列
		Texture2D * m_numberTexture[10];
		bool m_prevLButtonPressed;
		Othello * m_othello;

		//ボタンの位置情報を保存する変数
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

		// 石の数を記憶させる関数
		void RenderNumbers(Graphics * g, int x, int y, int value);

};
