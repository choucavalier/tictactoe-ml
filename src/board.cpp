#include <iostream>
#include <vector>
#include <memory>

#include "utils/make_unique.h"
#include "board.h"

using namespace std;

const vector<Line> Board::END_LINES = {
    // rows
    make_tuple(0, 1, 2),
    make_tuple(3, 4, 5),
    make_tuple(6, 7, 8),
    // columns
    make_tuple(0, 3, 6),
    make_tuple(1, 4, 7),
    make_tuple(2, 5, 8),
    // diagonals
    make_tuple(0, 4, 8),
    make_tuple(2, 4, 6)
};


Board::Board() : cells(make_unique<vector<unsigned char>>(9))
{
    for (int i = 0; i < 9; ++i)
        this->cells->at(i) = 0;
}

Board::~Board()
{
}


Line Board::get_line(const Line& end_line) const
{
    return make_tuple(
            this->cells->at(get<0>(end_line)),
            this->cells->at(get<1>(end_line)),
            this->cells->at(get<2>(end_line))
    );
}


unsigned char Board::get_winner_id() const
{
    bool finished = true;
    for (auto& end : this->END_LINES)
    {
        unsigned char a, b, c;
        tie(a, b, c) = this->get_line(end);
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


void Board::undo(unsigned char move)
{
    if (this->cells->at(move) == 0)
    {
        this->print();
        cerr << "Trying to undo a move at empty cell " << (int)move << endl;
        throw 2;
    }
    this->cells->at(move) = 0;
}

std::vector<unsigned char> const* Board::get_cells() const
{
    return this->cells.get();
}

void Board::update(unsigned char move, unsigned char player_id)
{
    if (this->cells->at(move) != 0)
    {
        cerr << "Cell not empty at " << move << endl;
        throw 1;
    }

    this->cells->at(move) = player_id;
}

std::unique_ptr<std::vector<unsigned char>> Board::get_features(
        unsigned char player_id)
{
    unsigned char oid = player_id == 1 ? 2 : 1;
    auto features = make_unique<vector<unsigned char>>(6);
    for (auto& end : this->END_LINES)
    {
        auto line = this->get_line(end);
        unsigned char n[3] = {0, 0, 0};
        n[(int)get<0>(line)]++;
        n[(int)get<1>(line)]++;
        n[(int)get<2>(line)]++;
        features->at(0) += n[player_id] == 3;
        features->at(1) += n[oid] == 3;
        features->at(2) += n[player_id] == 1 && n[0] == 2;
        features->at(3) += n[player_id] == 2 && n[0] == 1;
        features->at(4) += n[oid] == 1 && n[0] == 2;
        features->at(5) += n[oid] == 2 && n[0] == 1;
    }
    return features;
}


void Board::print() const
{
    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 3; ++j)
        {
            cout << " ";
            string s;
            if (this->cells->at(i + j) == 0)
                s = to_string(i + j + 1);
            else
            {
                if (this->cells->at(i + j) == 1)
                    s = "X";
                else
                    s = "O";
            }
            cout << s;
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool Board::is_over()
{
    return (this->get_winner_id() != 0);
}


void Board::set_cell(int const& index, char const& value)
{
    this->cells->at(index) = value;
}

void Board::swap(int const& i, int const& j)
{
    char tmp = this->cells->at(i);
    this->cells->at(i) = this->cells->at(j);
    this->cells->at(j) = tmp;
}

shared_ptr<Board> Board::copy()
{
    auto board = make_shared<Board>();
    for (unsigned int i = 0; i < this->cells->size(); ++i)
        board->set_cell(i, this->cells->at(i));
    return board;
}

string Board::in_string()
{
    string s = "";
    for (auto& e : *this->cells)
        s = s + to_string((int)e);
    return s;
}

