#pragma once
#include<unordered_map>

//�O���錾
class Texture2D;

class TextureManager
{
private:
	// �B���TextureManager�^�C���X�^���X
	static TextureManager * s_instance;

	std::unordered_map<int, Texture2D*> m_textures;


private:
	// �R���X�g���N�^
	TextureManager();

	// �f�X�g���N�^
	~TextureManager();

	// �R�s�[�R���X�g���N�^
	TextureManager(const TextureManager &) = delete;

	// ���[�u�R���X�g���N�^
	TextureManager(const TextureManager &&) = delete;

	// ������Z�q
	TextureManager & operator =(const TextureManager &) = delete;

public:
	// TextureManager�T�u�V�X�e�������������܂��B
	static void Startup();

	// TextureManager�T�u�V�X�e�����I�����܂��B
	static void Shutdown();

	// �B��̃C���X�^���X���擾���܂��B
	static TextureManager * GetInstance() { return s_instance; }


	//�e�N�X�`����o�^����֐�
	Texture2D * RegisterTexture(const char * path, int textureID);

	//�w�肵���e�N�X�`����T���o���֐�
	Texture2D * FindTexture(int textureID);

	//�w�肵���e�N�X�`������������֐�
	void UnregisterTexture(int textureID);

	//�e�N�X�`�������ׂĉ�������֐�
	void Clear();
};