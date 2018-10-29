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

	//初期石の配置
	board[3][3] = WHITE;
	board[4][4] = WHITE;
	board[3][4] = BLACK;
	board[4][3] = BLACK;
}

void Othello::ColorDecide()
{
	//ターン決めのための乱数生成
	srand((unsigned int)time(0));
	// 条件 ? 真だったら : 偽だったら
	mycolor = (rand() % 2) ? Othello::BLACK : Othello::WHITE;
	GlobalData::GetInstance()->SetMyColor(mycolor);
	//常に黒から先行
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
			{   //白石を数える
				whitestones++;
			}
			else if (board[y][x] == BLACK)
			{   //黒石を数える
				blackstones++;
			}
		}
	}
}

bool Othello::CheckFlip(int x, int y, int turn, FlipStones & result)
{
	//・ひっくり返せる条件
	//　(右方向の場合)石を置いた右方向先に相手の連続した所に自分の石がある
	//　置く場所に石がない
	result.putx = x;
	result.puty = y;

	//全方向のひっくり返せる石の数
	result.total = 0;

	for (int dir = 0; dir< 8; dir++)
	{
		//一方向のひっくり返せる石の数
		result.count[dir] = 0;

		// 元々置いた石の場所に位置を戻す
		x = result.putx;
		y = result.puty;

		while (1)
		{
			y += dir_y[dir];
			x += dir_x[dir];

			//盤面の外に出ていたら終了
			if (x >= 8 || y >= 8 || x < 0 || y < 0)
			{
				result.count[dir] = 0;
				break;
			}
			//自分の石だったら終了
			if (board[y][x] == turn)
			{
				break;
			}
			//空きマスだったら終了
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
