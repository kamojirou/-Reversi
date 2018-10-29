#include "Othello.h"
#include "Graphics.h"
#include <time.h>
#include "GlobalData.h"

void Othello::ResetBoard()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			board[y][x] = EMPTY;
		}
	}

	//�����΂̔z�u
	board[3][3] = WHITE;
	board[4][4] = WHITE;
	board[3][4] = BLACK;
	board[4][3] = BLACK;
}

void Othello::ColorDecide()
{
	//�^�[�����߂̂��߂̗�������
	srand((unsigned int)time(0));
	// ���� ? �^�������� : �U��������
	mycolor = (rand() % 2) ? Othello::BLACK : Othello::WHITE;
	GlobalData::GetInstance()->SetMyColor(mycolor);
	//��ɍ������s
	turn = Othello::BLACK;
}

void Othello::CountStones()
{
	blackstones = 0;
	whitestones = 0;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (board[y][x] == WHITE)
			{   //���΂𐔂���
				whitestones++;
			}
			else if (board[y][x] == BLACK)
			{   //���΂𐔂���
				blackstones++;
			}
		}
	}
}

bool Othello::CheckFlip(int x, int y, int turn, FlipStones & result)
{
	//�E�Ђ�����Ԃ������
	//�@(�E�����̏ꍇ)�΂�u�����E������ɑ���̘A���������Ɏ����̐΂�����
	//�@�u���ꏊ�ɐ΂��Ȃ�
	result.putx = x;
	result.puty = y;

	//�S�����̂Ђ�����Ԃ���΂̐�
	result.total = 0;

	for (int dir = 0; dir< 8; dir++)
	{
		//������̂Ђ�����Ԃ���΂̐�
		result.count[dir] = 0;

		// ���X�u�����΂̏ꏊ�Ɉʒu��߂�
		x = result.putx;
		y = result.puty;

		while (1)
		{
			y += dir_y[dir];
			x += dir_x[dir];

			//�Ֆʂ̊O�ɏo�Ă�����I��
			if (x >= 8 || y >= 8 || x < 0 || y < 0)
			{
				result.count[dir] = 0;
				break;
			}
			//�����̐΂�������I��
			if (board[y][x] == turn)
			{
				break;
			}
			//�󂫃}�X��������I��
			if (board[y][x] == EMPTY)
			{
				result.count[dir] = 0;
				break;
			}
			result.count[dir]++;
		}

		result.total += result.count[dir];
	}

	return result.total > 0;
}

void Othello::Flip(const FlipStones & info)
{
	for (int dir = 0; dir < 8; dir++)
	{
		int x = info.putx;
		int y = info.puty;

		for (int i = 0; i < info.count[dir]; i++)
		{
			x += dir_x[dir];
			y += dir_y[dir];
			board[y][x] = -board[y][x];
		}
	}

}

void Othello::SearchPutable(int turn_)
{
	count = 0;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (board[y][x] == EMPTY)
			{
				FlipStones checkresult;
				if (CheckFlip(x, y, turn, checkresult))
				{
					putable[count].x = x;
					putable[count].y = y;
					count++;
				}
			}
		}
	}
}

void Othello::ThinkTimeForPlayer(int ox, int oy, int mx, int my)
{
	if (count>0)
	{
		if (ox >= 0 && ox <= 613 && oy >= 0 && oy <= 613)
		{
			if (board[my][mx] == EMPTY)
			{
				bool currLButtonPressed = (GetKeyState(VK_LBUTTON) & 0x8000) == 0x8000;
				if (!prevLButtonPressed && currLButtonPressed)
				{
					FlipStones result;
					if (CheckFlip(mx, my, turn, result))
					{
						Flip(result);
						board[my][mx] = turn;
						turn = -turn;
						m_state = OthelloState::TurnEnd;
					}
				}
				prevLButtonPressed = currLButtonPressed;
			}
		}
	}
	else
	{
		turn = -turn;
		m_state = OthelloState::TurnEnd;
	}

}

void Othello::ThinkTimeForCom()
{
	int putcom;
	if (count > 0)
	{
		putcom = rand() % count;

		int x = putable[putcom].x;
		int y = putable[putcom].y;

		FlipStones result;
		if (CheckFlip(x, y, turn, result))
		{
			Flip(result);
			board[y][x] = turn;
			turn = -turn;
			m_state = OthelloState::TurnEnd;
		}
	}
	else
	{
		turn = -turn;
		m_state = OthelloState::TurnEnd;
	}

}
void Othello::PlayStart()
{
	m_state = OthelloState::PlayStart;
}


void Othello::TurnStart()
{
	m_state = OthelloState::TurnStart;
}

void Othello::Playing()
{
	m_state = OthelloState::Playing;
}

void Othello::PlayEnd()
{
	m_state = OthelloState::PlayEnd;
}
