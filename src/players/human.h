#ifndef HUMAN_H
#define HUMAN_H
#include <memory>

#include "player.h"

class Human : public Player
{
public:
    Human (short id);
    virtual ~Human ();

    short get_move(std::shared_ptr<Board> board) const;
};

#endif

