#include <memory>

#include "utils/make_unique.h"
#include "player.h"
#include "board.h"

using namespace std;

Player::Player(short id) : id(id)
{
}

Player::~Player()
{
}

short Player::get_id() const
{
    return this->id;
}

void Player::set_id(short id)
{
    this->id = id;
}

unique_ptr<vector<short>> Player::get_possible_moves(string const& board)
{
    auto moves = make_unique<vector<short>>();

    for (short i = 1; i < 10; ++i)
        if (board[i] == 0)
            moves->push_back(i);
    return moves;
}

unique_ptr<vector<short>> Player::get_possible_moves(
        std::shared_ptr<Board> board)
{
    return Player::get_possible_moves(board->get_map());
}

