#include <iostream>
#include <vector>
#include <memory>

#include "utils/make_unique.h"
#include "board.h"

using namespace std;

const vector<Line> Board::END_LINES = {
    // rows
    make_tuple(1, 2, 3),
    make_tuple(4, 5, 6),
    make_tuple(7, 8, 9),
    // columns
    make_tuple(1, 4, 7),
    make_tuple(2, 5, 8),
    make_tuple(3, 6, 9),
    // diagonals
    make_tuple(1, 5, 9),
    make_tuple(3, 5, 7)
};


Board::Board() : map("")
{
    for (int i = 0; i < 10; ++i)
        map[i] = 0;
}

Board::~Board()
{
}


Line Board::get_line(const Line& end_line) const
{
    return Board::get_line(end_line, this->map);
}

Line Board::get_line(const Line& end_line, string const& board)
{
    return make_tuple(
            board[(get<0>(end_line))],
            board[(get<1>(end_line))],
            board[(get<2>(end_line))]
    );
}

short Board::get_winner_id() const
{
    return this->get_winner_id(this->map);
}


short Board::get_winner_id(string const& board)
{
    bool finished = true;
    for (auto& end : Board::END_LINES)
    {
        short a, b, c;
        tie(a, b, c) = Board::get_line(end, board);
        if (a == 1 && b == 1 && c == 1)
            return 1;
        if (a == 2 && b == 2 && c == 2)
            return 2;
        if (a == 0 || b == 0 || c == 0)
            finished = false;
    }
    if (finished)
        return 4;
    return 0;
}


void Board::undo(short move)
{
    if (this->map[move] == 0)
    {
        this->print();
        cerr << "Trying to undo a move at empty cell " << (int)move << endl;
        throw 2;
    }
    this->map[move] = 0;
    this->map[0]--;
}

void Board::update(short move, short player_id)
{
    if (this->map[move] != 0)
    {
        cerr << "Cell not empty at " << move << endl;
        throw 1;
    }

    this->map[move] = player_id;
    this->map[0]++;
}

short Board::level()
{
    return this->map[0];
}

void Board::print() const
{
    this->print(this->map);
}

bool Board::is_over()
{
    return (this->get_winner_id() != 0);
}

bool Board::is_over(string const& board)
{
    return (Board::get_winner_id(board) != 0);
}

short Board::operator[](int index)
{
    return this->map[index];
}

void Board::swap(int i, int j)
{
    std::swap(this->map[i], this->map[j]);
}

string const& Board::get_map()
{
    return this->map;
}

void Board::print(string const& bmap)
{
    for (short i = 1; i < 10; ++i)
    {
        cout << " ";
        if (!bmap[i])
            cout << i;
        else
            cout << ((bmap[i] == 1) ? "X" : "O");
        if (i % 3 == 0)
            cout << endl;
    }
    cout << endl;
}
