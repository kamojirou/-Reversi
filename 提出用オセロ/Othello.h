#pragma once

// 前方宣言
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
	//盤面の状態を表す
    //#define EMPTY   ( 0)
    //#define WHITE   ( 1)
    //#define BLACK   (-1)
	static const char EMPTY =  0;
	static const char WHITE =  1;
	static const char BLACK = -1;
	//盤面
	char board[8][8];

	/*置いた石の周りを探索
	7 　 0  　 1
	6 　 ○ 　 2
	5 　 4  　 3
	*/
	int dir_x[8] = { +0,+1,+1,+1,+0,-1,-1,-1 };
	int dir_y[8] = { -1,-1,+0,+1,+1,+1,+0,-1 };

	//ターンの切り替わりを記憶させる変数
	char turn;

	//自分の色を記憶する変数
	char mycolor;

	//黒石の数
	char blackstones;
	//白石の数
	char whitestones;

	//クリックされたかどうか
	bool prevLButtonPressed = false;

	//マスの位置
	struct Point
	{
		int x;
		int y;
	};

	//マスに関する配列
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

	// オセロの進行状況を表す値を返します。
	OthelloState GetState() const { return m_state; }

	// 盤面の初期化する関数
	void ResetBoard();

	//色を決める関数
	void ColorDecide();

	// 盤面にある石の数を数える関数
	void CountStones();

	// (x,y)に置いたときに、ひっくり返せる石の個数を調べる関数
	bool CheckFlip(int x, int y, int turn, FlipStones & result);

	//ひっくり返す処理の関数
	void Flip(const FlipStones & info);

	//置くことができる場所を探す関数
	void SearchPutable(int turn_);

	//playerが置く場所を決める関数
	void ThinkTimeForPlayer(int ox, int oy, int mx, int my);

	//comが置く場所を決める関数
	void ThinkTimeForCom();

	//ターン内の進行を更新する関数
	void PlayStart();

	void TurnStart();

	void Playing();

	void PlayEnd();
};
