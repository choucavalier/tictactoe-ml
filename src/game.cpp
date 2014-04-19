#include <memory>
#include <vector>
#include <vector>
#include <iostream>
#include <tuple>
#include <exception>

#include "utils/make_unique.h"
#include "game.h"
#include "player.h"

using namespace std;

Game::Game(unique_ptr<Player> p1, unique_ptr<Player> p2) : winner_id(0),
    board(make_shared<Board>()), p1(move(p1)), p2(move(p2))
{
}

Game::~Game()
{
}


bool Game::is_over() const
{
    return (this->winner_id != 0);
}

short Game::get_winner_id() const
{
    return this->winner_id;
}

void Game::update_winner_id()
{
    this->winner_id = this->board->get_winner_id();
}

std::shared_ptr<Board> Game::get_board() const
{
    return this->board;
}


string Game::get_state() const
{
    switch (this->winner_id)
    {
        case 3:
            return "draw";
        case 1:
            return "player 1 won";
        case 2:
            return "player 2 won";
        default:
            return "not finished";
    }
}


void Game::run(short verbose)
{
    shared_ptr<Player> current;
    if (this->p1->get_id() == 1)
        current = this->p1;
    else
        current = this->p2;

    while (!this->is_over())
    {
        short move = current->get_move(this->get_board());

        this->board->update(move, current->get_id());
        this->update_winner_id();

        if (&(*current) == &(*this->p1))
            current = this->p2;
        else
            current = this->p1;
    }
    
    if (verbose > 0)
        cout << "GAME OVER: " << this->get_state() << endl;
    if (verbose > 1)
        this->board->print();
}

