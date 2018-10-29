#pragma once
#include<unordered_map>

//前方宣言
class Texture2D;

class TextureManager
{
private:
	// 唯一のTextureManager型インスタンス
	static TextureManager * s_instance;

	std::unordered_map<int, Texture2D*> m_textures;


private:
	// コンストラクタ
	TextureManager();

	// デストラクタ
	~TextureManager();

	// コピーコンストラクタ
	TextureManager(const TextureManager &) = delete;

	// ムーブコンストラクタ
	TextureManager(const TextureManager &&) = delete;

	// 代入演算子
	TextureManager & operator =(const TextureManager &) = delete;

public:
	// TextureManagerサブシステムを初期化します。
	static void Startup();

	// TextureManagerサブシステムを終了します。
	static void Shutdown();

	// 唯一のインスタンスを取得します。
	static TextureManager * GetInstance() { return s_instance; }


	//テクスチャを登録する関数
	Texture2D * RegisterTexture(const char * path, int textureID);

	//指定したテクスチャを探し出す関数
	Texture2D * FindTexture(int textureID);

	//指定したテクスチャを解除する関数
	void UnregisterTexture(int textureID);

	//テクスチャをすべて解除する関数
	void Clear();
};