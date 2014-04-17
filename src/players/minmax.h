#ifndef MINMAX_H
#define MINMAX_H

#include <memory>
#include <utility>

#include "player.h"
#include "board.h"

class MinMax : public Player
{
public:
    MinMax (unsigned char id);
    virtual ~MinMax ();

    unsigned char get_move(std::shared_ptr<Board> board) const;

private:
    unsigned char oid;
    std::pair<unsigned char, int> min(std::shared_ptr<Board> board) const;
    std::pair<unsigned char, int> max(std::shared_ptr<Board> board) const;
    int evaluate(std::shared_ptr<Board> board) const;
};

#endif
