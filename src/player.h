#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>

class Board;

class Player
{
public:
    Player (short id);
    virtual ~Player ();

    virtual short get_id() const;

    virtual void set_id(short id);

    virtual short get_move(std::shared_ptr<Board> board) const = 0;

    static std::unique_ptr<std::vector<short>> get_possible_moves(
            std::shared_ptr<Board> board);

    static std::unique_ptr<std::vector<short>> get_possible_moves(
            std::string const& board);

protected:
    short id;
};


#endif
