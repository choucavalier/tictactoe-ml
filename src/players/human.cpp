#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>

#include "game.h"
#include "player.h"
#include "players/human.h"

using namespace std;

Human::Human(short id) : Player(id)
{
}

Human::~Human()
{
}


short Human::get_move(std::shared_ptr<Board> board) const
{
    auto pm = this->get_possible_moves(board);
    board->print();
    short move;
    while (1)
    {
        cout << "Choose a move, player " << (short)this->id << " » ";
        cin >> move;
        move--;
        if (find(pm->begin(), pm->end(), move) != pm->end()
                && move < 10 && move > 0)
            break;
        else
            cout << "Move " << move << " not allowed" << endl;
    }

    return move;
}

