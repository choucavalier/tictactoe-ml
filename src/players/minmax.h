#ifndef MINMAX_H
#define MINMAX_H

#include <memory>
#include <utility>

#include "player.h"
#include "board.h"

class MinMax : public Player
{
public:
    MinMax (short id);
    virtual ~MinMax ();

    short get_move(std::shared_ptr<Board> board) const;

private:
    short oid;
    std::pair<short, int> min(std::shared_ptr<Board> board) const;
    std::pair<short, int> max(std::shared_ptr<Board> board) const;
    int evaluate(std::shared_ptr<Board> board) const;
};

#endif
