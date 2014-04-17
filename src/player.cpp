#include <memory>

#include "utils/make_unique.h"
#include "player.h"
#include "board.h"

using namespace std;

Player::Player(unsigned char id) : id(id)
{
}

Player::~Player()
{
}

unsigned char Player::get_id() const
{
    return this->id;
}

unique_ptr<vector<unsigned char>> Player::get_possible_moves(
        std::shared_ptr<Board> board)
{
    auto moves = make_unique<vector<unsigned char>>();

    for (unsigned char i = 0; i < 9; ++i)
        if (board->get_cells()->at(i) == 0)
            moves->push_back(i);
    return moves;
}

