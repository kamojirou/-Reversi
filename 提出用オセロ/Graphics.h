#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdint>

// 前方宣言
class Texture2D;

// グラフィック関連 (シングルトン)
//  ・継承禁止
//  ・生成禁止
//  ・コピー禁止
//  ・ムーブ禁止
class Graphics final
{
private:
	// 唯一のGraphics型インスタンス
	static Graphics * s_instance;

	// Direct3Dデバイス
	IDirect3DDevice9 * m_d3dDevice;

private:
	// コンストラクタ
	Graphics();

	// デストラクタ
	~Graphics();

	// コピーコンストラクタ
	Graphics(const Graphics &) = delete;

	// ムーブコンストラクタ
	Graphics(const Graphics &&) = delete;

	// 代入演算子
	Graphics & operator =(const Graphics &) = delete;

public:
	// Graphicsサブシステムを初期化します。
	static void Startup(HWND hWnd, uint32_t resolutionX, uint32_t resolutionY);

	// Graphicsサブシステムを終了します。
	static void Shutdown();

	// 唯一のインスタンスを取得します。
	static Graphics * GetInstance() { return s_instance; }

	// Direct3Dデバイスを取得します。
	IDirect3DDevice9 * GetD3DDevice() const;

	// テクスチャをロードします。
	Texture2D * LoadTexture(const char * path);

	// 2Dスプライトを描画します。
	void DrawSprite(float dx, float dy, float dw, float dh, Texture2D * texture, float sx, float sy, float sw, float sh, D3DCOLOR color);

	// 矩形領域を単色で塗りつぶします。
	void FillRect(float dx, float dy, float dw, float dh, D3DCOLOR color);

	// グリッド線を描画します。
	void RenderAxisGrid();

private:
	// Direct3Dデバイスを作成します。
	void CreateDevice(HWND hWnd, uint32_t resolutionX, uint32_t resolutionY);
};





