#pragma once

// �O���錾
class Texture2D;

enum class OthelloState
{
	PlayStart,
	TurnStart,
	Playing,
	TurnEnd,
	PlayEnd,
};

class Othello
{
public:
	//�Ֆʂ̏�Ԃ�\��
    //#define EMPTY   ( 0)
    //#define WHITE   ( 1)
    //#define BLACK   (-1)
	static const char EMPTY =  0;
	static const char WHITE =  1;
	static const char BLACK = -1;
	//�Ֆ�
	char board[8][8];

	/*�u�����΂̎����T��
	7 �@ 0  �@ 1
	6 �@ �� �@ 2
	5 �@ 4  �@ 3
	*/
	int dir_x[8] = { +0,+1,+1,+1,+0,-1,-1,-1 };
	int dir_y[8] = { -1,-1,+0,+1,+1,+1,+0,-1 };

	//�^�[���̐؂�ւ����L��������ϐ�
	char turn;

	//�����̐F���L������ϐ�
	char mycolor;

	//���΂̐�
	char blackstones;
	//���΂̐�
	char whitestones;

	//�N���b�N���ꂽ���ǂ���
	bool prevLButtonPressed = false;

	//�}�X�̈ʒu
	struct Point
	{
		int x;
		int y;
	};

	//�}�X�Ɋւ���z��
	Point putable[64];
	int count;

	struct FlipStones
	{
		int total;
		int count[8];
		int putx;
		int puty;
	};

	OthelloState m_state;

	// �I�Z���̐i�s�󋵂�\���l��Ԃ��܂��B
	OthelloState GetState() const { return m_state; }

	// �Ֆʂ̏���������֐�
	void ResetBoard();

	//�F�����߂�֐�
	void ColorDecide();

	// �Ֆʂɂ���΂̐��𐔂���֐�
	void CountStones();

	// (x,y)�ɒu�����Ƃ��ɁA�Ђ�����Ԃ���΂̌��𒲂ׂ�֐�
	bool CheckFlip(int x, int y, int turn, FlipStones & result);

	//�Ђ�����Ԃ������̊֐�
	void Flip(const FlipStones & info);

	//�u�����Ƃ��ł���ꏊ��T���֐�
	void SearchPutable(int turn_);

	//player���u���ꏊ�����߂�֐�
	void ThinkTimeForPlayer(int ox, int oy, int mx, int my);

	//com���u���ꏊ�����߂�֐�
	void ThinkTimeForCom();

	//�^�[�����̐i�s���X�V����֐�
	void PlayStart();

	void TurnStart();

	void Playing();

	void PlayEnd();
};
