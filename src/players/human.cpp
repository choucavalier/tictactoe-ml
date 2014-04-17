#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>

#include "game.h"
#include "player.h"
#include "players/human.h"

using namespace std;

Human::Human(unsigned char id) : Player(id)
{
}

Human::~Human()
{
}


unsigned char Human::get_move(std::shared_ptr<Board> board) const
{
    auto pm = this->get_possible_moves(board);
    board->print();
    unsigned int move;
    while (1)
    {
        cout << "Choose a move, player " << (int)this->id << " » ";
        cin >> move;
        move--;
        if (find(pm->begin(), pm->end(), move) != pm->end()
                && move < 9 && move >= 0)
            break;
        else
            cout << "Move " << (move + 1) << " not allowed" << endl;
    }

    return move;
}

