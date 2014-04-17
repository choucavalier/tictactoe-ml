#ifndef HUMAN_H
#define HUMAN_H
#include <memory>

#include "player.h"

class Human : public Player
{
public:
    Human (unsigned char id);
    virtual ~Human ();

    unsigned char get_move(std::shared_ptr<Board> board) const;
};

#endif

