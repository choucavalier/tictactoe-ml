#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>

class Board;

class Player
{
public:
    Player (unsigned char id);
    virtual ~Player ();

    virtual unsigned char get_id() const;

    virtual unsigned char get_move(std::shared_ptr<Board> board) const = 0;

    static std::unique_ptr<std::vector<unsigned char>> get_possible_moves(
            std::shared_ptr<Board> board);

protected:
    unsigned char id;
};


#endif
