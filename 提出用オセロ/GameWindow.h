#pragma once
#include <windows.h>
#include <cstdint>

// �Q�[���E�B���h�E (�V���O���g��)
//  �E�p���֎~
//  �E�����֎~
//  �E�R�s�[�֎~
//  �E���[�u�֎~
class GameWindow final
{
private:
	// �B���GameWindow�^�C���X�^���X
	static GameWindow * s_instance;

	// �E�B���h�E�n���h��
	HWND m_hWnd;

private:
	// �R���X�g���N�^
	GameWindow();

	// �f�X�g���N�^
	~GameWindow();

	// �R�s�[�R���X�g���N�^
	GameWindow(const GameWindow &) = delete;

	// ���[�u�R���X�g���N�^
	GameWindow(const GameWindow &&) = delete;

	// ������Z�q
	GameWindow & operator =(const GameWindow &) = delete;

public:
	// GameWindow�T�u�V�X�e�������������܂��B
	static void Startup(uint32_t clientWidth, uint32_t clientHeight);

	// GameWindow�T�u�V�X�e�����I�����܂��B
	static void Shutdown();

	// �B��̃C���X�^���X���擾���܂��B
	static GameWindow * GetInstance() { return s_instance; }

	// �E�B���h�E�n���h�����擾���܂��B
	HWND GetSafeHWnd() const;

private:
	// �E�B���h�E���쐬���܂��B
	void Create(uint32_t clientWidth, uint32_t clientHeight);
};
