#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdint>

class Texture2D
{
private:
	IDirect3DTexture9 * m_nativeTexture;
	uint32_t m_originalWidth;
	uint32_t m_originalHeight;
	uint32_t m_surfaceWidth;
	uint32_t m_surfaceHeight;

public:
	// コンストラクタ
	Texture2D(IDirect3DTexture9 * nativeTexture, uint32_t originalWidth, uint32_t originalHeight);

	// デストラクタ
	~Texture2D();

	// Direct3Dテクスチャを取得します。
	IDirect3DTexture9 * GetNativeTexture() const;

	// 画像の横幅を取得します。
	uint32_t GetOriginalWidth() const { return m_originalWidth; }

	// 画像の高さを取得します。
	uint32_t GetOriginalHeight() const { return m_originalHeight; }

	// レベル0サーフェイスの横幅を取得します。
	uint32_t GetSurfaceWidth() const { return m_surfaceWidth; }

	// レベル0サーフェイスを取得します。
	uint32_t GetSurfaceHeight() const { return m_surfaceHeight; }
};