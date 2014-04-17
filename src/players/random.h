#ifndef RANDOM_PLAYER_H
#define RANDOM_PLAYER_H

#include <memory>

#include "player.h"
#include "board.h"

class RandomPlayer : public Player
{
public:
    RandomPlayer (unsigned char id);
    virtual ~RandomPlayer ();

    unsigned char get_move(std::shared_ptr<Board> board) const;

private:
    /* data */
};

#endif
