#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <vector>

#include "board.h"

class Player;

class Game
{
public:
    Game (std::shared_ptr<Player> p1, std::shared_ptr<Player> p2);
    virtual ~Game ();

    void run(short verbose = 0);

    bool is_over() const;

    std::string get_state() const;

    std::shared_ptr<Board> get_board() const;
    
    short get_winner_id() const;

private:
    short winner_id;
    void update_winner_id();
    std::shared_ptr<Board> board;
    std::shared_ptr<Player> p1;
    std::shared_ptr<Player> p2;

};

#endif

