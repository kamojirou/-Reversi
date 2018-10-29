#include "Texture2D.h"

Texture2D::Texture2D(IDirect3DTexture9 * nativeTexture, uint32_t originalWidth, uint32_t originalHeight)
	: m_nativeTexture(nativeTexture)
	, m_originalWidth(originalWidth)
	, m_originalHeight(originalHeight)
{
	m_nativeTexture->AddRef();

	// テクスチャの詳細情報を取得する
	D3DSURFACE_DESC desc;
	m_nativeTexture->GetLevelDesc(0, &desc);
	m_surfaceWidth = desc.Width;
	m_surfaceHeight = desc.Height;
}


Texture2D::~Texture2D()
{
	m_nativeTexture->Release();
}


IDirect3DTexture9 * Texture2D::GetNativeTexture() const
{
	return m_nativeTexture;
}
