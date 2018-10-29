#pragma once
#include <windows.h>
#include <cstdint>

// ゲームウィンドウ (シングルトン)
//  ・継承禁止
//  ・生成禁止
//  ・コピー禁止
//  ・ムーブ禁止
class GameWindow final
{
private:
	// 唯一のGameWindow型インスタンス
	static GameWindow * s_instance;

	// ウィンドウハンドル
	HWND m_hWnd;

private:
	// コンストラクタ
	GameWindow();

	// デストラクタ
	~GameWindow();

	// コピーコンストラクタ
	GameWindow(const GameWindow &) = delete;

	// ムーブコンストラクタ
	GameWindow(const GameWindow &&) = delete;

	// 代入演算子
	GameWindow & operator =(const GameWindow &) = delete;

public:
	// GameWindowサブシステムを初期化します。
	static void Startup(uint32_t clientWidth, uint32_t clientHeight);

	// GameWindowサブシステムを終了します。
	static void Shutdown();

	// 唯一のインスタンスを取得します。
	static GameWindow * GetInstance() { return s_instance; }

	// ウィンドウハンドルを取得します。
	HWND GetSafeHWnd() const;

private:
	// ウィンドウを作成します。
	void Create(uint32_t clientWidth, uint32_t clientHeight);
};
