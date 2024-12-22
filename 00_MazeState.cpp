//============================================================================
// Name        : MazeState.cpp
// Author      : HajimeProletarier
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <sstream>

// 座標を保持する構造体
struct Coord
{
	int y_;
	int x_;
	Coord(const int y = 0, const int x = 0) : y_(y), x_(x) {}
};

// 数字集め迷路のコンストラクタ

constexpr int H = 3;		// 迷路の高さ
constexpr int W = 4;		// 迷路の幅
constexpr int END_TURN = 4; // ゲーム終了ターン

class MazeState
{
private:
	int points_[H][W] = {}; // 床のポイントを 1 ～　9 で表現する
	int turn_ = 0;			// 現在のターン

	static constexpr int dx[4] = {1, -1, 0, 0};
	static constexpr int dy[4] = {0, 0, 1, -1};

public:
	Coord character_ = Coord();
	int game_score_ = 0; // ゲーム上で実際に得たスコア

	// [どのゲームでも実装する]：ゲームの終了判定
	bool isDone() const
	{
		return this->turn_ == END_TURN;
	}

	// 空のコンストラクタ
	MazeState()
	{
	}

	// h * w の迷路を生成する
	MazeState(const int seed)
	{
		auto mt_for_construct = std::mt19937(seed); // 盤面生成用の乱数生成器を初期化

		// キャラクターを盤面上のランダムな位置に配置する
		this->character_.y_ = mt_for_construct() % H;
		this->character_.x_ = mt_for_construct() % W;

		// 盤面にポイントを配置していく
		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
			{
				// キャラクターの初期位置にはポイントは置かない
				if (y == character_.y_ && x == character_.x_)
				{
					continue;
				}
				this->points_[y][x] = mt_for_construct() % 10; // 値は[0, 9]の範囲
			}
		}
	}

	// 指定した action で 1ターン進める
	void advance(const int action)
	{
		this->character_.x_ += dx[action];
		this->character_.y_ += dy[action];
		auto &point = this->points_[this->character_.y_][this->character_.x_]; // 参照を取得
		if (point > 0)
		{

			this->game_score_ += point;
			point = 0;
		}
		this->turn_++;
	}

	// 現在の状況でプレイヤーが可能な行動を全て取得する
	std::vector<int> legalActions() const
	{
		std::vector<int> actions;
		for (int action = 0; action < 4; action++)
		{
			int ty = this->character_.y_ + dy[action];
			int tx = this->character_.x_ + dx[action];
			if (ty >= 0 && ty < H && tx >= 0 && tx < W)
			{
				actions.emplace_back(action);
			}
		}
		return actions;
	}
	// 現在のゲーム状況を文字列で表現する
	std::string toString() const
	{
		std::stringstream ss;
		ss << "turn:\t" << this->turn_ << "\n";
		ss << "score:\t" << this->game_score_ << "\n";
		for (int h = 0; h < H; h++)
		{
			for (int w = 0; w < W; w++)
			{
				if (this->character_.y_ == h && this->character_.x_ == w)
				{
					ss << '@';
				}
				else if (this->points_[h][w] > 0)
				{
					ss << points_[h][w];
				}
				else
				{
					ss << '.';
				}
			}
			ss << '\n';
		}

		return ss.str();
	}
};

using State = MazeState;
std::mt19937 mt_for_action(0);
// ランダムに行動を決定する
int randomAction(const State &state)
{
	auto legal_actions = state.legalActions();
	return legal_actions[mt_for_action() % (legal_actions.size())];
}

void playGame(const int seed)
{
	using std::cout;
	using std::endl;

	auto state = State(seed);
	cout << state.toString() << endl;
	while (!state.isDone())
	{
		state.advance(randomAction(state));
		cout << state.toString() << endl;
	}
}

int main()
{
	using std::cout;
	using std::endl;
	playGame(/*盤面初期化のシード */ 121321);
	return 0;
}
