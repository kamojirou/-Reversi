#include "Graphics.h"
#include "Texture2D.h"
#include <cassert>
#include <cstdio>

// ライブラリのリンク
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// 静的メンバ変数の実体を宣言
Graphics * Graphics::s_instance = nullptr;

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}


void Graphics::Startup(HWND hWnd, uint32_t resolutionX, uint32_t resolutionY)
{
	assert(s_instance == nullptr);
	s_instance = new Graphics();
	s_instance->CreateDevice(hWnd, resolutionX, resolutionY);
}


void Graphics::Shutdown()
{
	assert(s_instance != nullptr);
	delete s_instance;
	s_instance = nullptr;
}


IDirect3DDevice9 * Graphics::GetD3DDevice() const
{
	return m_d3dDevice;
}

Texture2D * Graphics::LoadTexture(const char * path)
{
	IDirect3DTexture9 * texture;
	D3DXIMAGE_INFO imageInfo;
	
	HRESULT hr = D3DXCreateTextureFromFileEx(
		m_d3dDevice,				// Direct3Dデバイス
		path,						// テクスチャファイルのパス
		0,							// テクスチャの横幅(0の場合すべて）
		0,							// テクスチャの高さ(0の場合すべて）
		1,							// ミップレベル
		0,							// テクスチャの利用方法
		D3DFMT_UNKNOWN,				// ピクセルフォーマット
		D3DPOOL_DEFAULT,			// どこのメモリに置くか？
		D3DX_FILTER_NONE,			// 拡大フィルタ
		D3DX_FILTER_NONE,			// ミップフィルタ
		D3DCOLOR_ARGB(0, 0, 0, 0),	// カラーキー (透過色)
		&imageInfo,					// 画像の詳細情報
		nullptr,					// 1,2,4,8ビットイメージ用パレット
		&texture);					// テクスチャを受け取る変数

	if (FAILED(hr))
	{
		char msg[1024];
		sprintf_s(msg, "\"%s\"の読み込みに失敗しました", path);
		MessageBox(nullptr, msg, "LoadTexture()関数内エラー", MB_OK);
	}

	Texture2D * texture2D = new Texture2D(texture, imageInfo.Width, imageInfo.Height);
	return texture2D;
}



void Graphics::DrawSprite(float dx, float dy, float dw, float dh, Texture2D * texture, float sx, float sy, float sw, float sh, D3DCOLOR color)
{
	// 頂点1個分を表す構造体
	struct VERTEX
	{
		float x, y, z;		// 常に必須
		float rhw;			// wの逆数 (2Dの時のみ必要)
		D3DCOLOR color;		// 頂点カラー
		float u, v;			// テクスチャ座標 (UV座標系)
	};

	dx -= 0.5f;
	dy -= 0.5f;

	const uint32_t surfaceW = texture->GetSurfaceWidth();
	const uint32_t surfaceH = texture->GetSurfaceHeight();

	// 頂点型の配列で用意する必要がある。
	// DirectXで扱う頂点はメモリ上で連続している必要がある。
	VERTEX v[4];

	// 頂点は右回りで配置しないとポリゴンが見えない。
	v[0].x = dx;
	v[0].y = dy;
	v[0].z = 0.0f;
	v[0].rhw = 1.0f;
	v[0].color = color;
	v[0].u = sx / surfaceW;
	v[0].v = sy / surfaceH;

	v[1].x = dx + dw;
	v[1].y = dy;
	v[1].z = 0.0f;
	v[1].rhw = 1.0f;
	v[1].color = color;
	v[1].u = (sx + sw) / surfaceW;
	v[1].v = sy / surfaceH;

	v[2].x = dx;
	v[2].y = dy + dh;
	v[2].z = 0.0f;
	v[2].rhw = 1.0f;
	v[2].color = color;
	v[2].u = sx / surfaceW;
	v[2].v = (sy + sh) / surfaceH;

	v[3].x = dx + dw;
	v[3].y = dy + dh;
	v[3].z = 0.0f;
	v[3].rhw = 1.0f;
	v[3].color = color;
	v[3].u = (sx + sw) / surfaceW;
	v[3].v = (sy + sh) / surfaceH;

	// 頂点1個分がどんな要素を持っているかをDirectXに教える
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	// 貼りたいテクスチャをDirectXに教える
	m_d3dDevice->SetTexture(0, texture->GetNativeTexture());

	// プリミティブ描画命令をDirectXに発行する。
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX));
}


void Graphics::FillRect(float dx, float dy, float dw, float dh, D3DCOLOR color)
{
	struct FILLVERTEX
	{
		float x, y, z;		// 常に必須
		float rhw;			// wの逆数 (2Dの時のみ必要)
		D3DCOLOR color;		// 頂点カラー
	};

	dx -= 0.5f;
	dy -= 0.5f;

	const FILLVERTEX v[4] =
	{
		{ dx,    dy,    0, 1, color },
		{ dx+dw, dy,    0, 1, color },
		{ dx,    dy+dh, 0, 1, color },
		{ dx+dw, dy+dh, 0, 1, color },
	};

	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_d3dDevice->SetTexture(0, nullptr);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FILLVERTEX));
}


void Graphics::RenderAxisGrid()
{
	struct LineVertex
	{
		float x, y, z;
		D3DCOLOR color;
	};

	const LineVertex line[6] =
	{
		{ 0,0,0, 0xffff0000 },	// x軸始点
		{ 20,0,0, 0xffff0000 },	// x軸終点

		{ 0,0,0,  0xff008000 },	// y軸始点
		{ 0,20,0, 0xff008000 },	// y軸終点

		{ 0,0,0,  0xff0000ff },	//z軸始点
		{ 0,0,20, 0xff0000ff },	//z軸終点
	};

	m_d3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_d3dDevice->SetTexture(0, nullptr);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, line, sizeof(LineVertex));

	LineVertex grid[31 * 2 * 2];

	int index = 0;
	for (int i = -15; i <= 15; i++)
	{
		grid[index++] = { -15, 0, (float)i, 0xff000000 };	// x軸始点
		grid[index++] = { +15, 0, (float)i, 0xff000000 };	// x軸終点
	}

	for (int i = -15; i <= 15; i++)
	{
		grid[index++] = { (float)i, 0, -15, 0xff000000 };	// z軸始点
		grid[index++] = { (float)i, 0, +15, 0xff000000 };	// z軸終点
	}

	m_d3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_d3dDevice->SetTexture(0, nullptr);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 31 * 2, grid, sizeof(LineVertex));
}

void Graphics::CreateDevice(HWND hWnd, uint32_t resolutionX, uint32_t resolutionY)
{
	// Direct3Dオブジェクトの初期化
	IDirect3D9 * d3dObject;
	if (!(d3dObject = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		assert(0);
		return;
	}

	//プレゼンテーションパラメータの設定
	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));
	d3dpp.BackBufferWidth = resolutionX;	// バックバッファの水平方向解像度
	d3dpp.BackBufferHeight = resolutionY;	// バックバッファの鉛直方向解像度
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.Flags = 0;

	if (FAILED(d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice)))
	{
		if (FAILED(d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &m_d3dDevice)))
		{
			if (FAILED(d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice)))
			{
				if (FAILED(d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice)))
				{
					MessageBox(nullptr, "Direct3Dの初期化に失敗しました", "エラー", MB_OK);
					d3dObject->Release();
					return;
				}
			}
		}
	}

	// Direct3Dオブジェクトの解放
	d3dObject->Release();

	// 光源計算のON/OFF
	// 光源計算ON、かつ、ライトなしの場合は黒で描画される
	// 光源計算OFFの場合は白またはテクスチャの色で描画される
	m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αブレンディングのON/OFF
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ZバッファのON/OFF
	// 3Dの場合は有効にするべき
	// 2Dの場合は無効でもOK
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 3Dの場合は変換行列も設定しておくべき

	// ワールド変換行列を設定する。
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	m_d3dDevice->SetTransform(D3DTS_WORLD, &world);

	// ビュー変換行列を設定する。
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(5, 5, -10), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	m_d3dDevice->SetTransform(D3DTS_VIEW, &view);

	// プロジェクション変換行列を設定する。
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DXToRadian(45), (float)resolutionX / resolutionY, 0.1f, 10000.0f);
	m_d3dDevice->SetTransform(D3DTS_PROJECTION, &projection);

	// ビューポートを設定する。
	D3DVIEWPORT9 viewport;
	memset(&viewport, 0, sizeof(viewport));
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = resolutionX;
	viewport.Height = resolutionY;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	m_d3dDevice->SetViewport(&viewport);
}
