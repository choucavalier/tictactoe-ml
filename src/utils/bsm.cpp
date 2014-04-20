#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "utils/make_unique.h"
#include "bsm.h"
#include "player.h"
#include "board.h"

using namespace std;

map<string, vector<pair<short, string>>> BSM::states = {};
map<string, vector<pair<short, string>>> BSM::strategies = {};

void BSM::load(const string& path)
{
    BSM::states.clear();
    ifstream file;
    file.open(path);
    string line;
    bool first = 1;
    string last;
    while (getline(file, line))
    {
        if (first)
        {
            string board = BSM::board(line.substr(0, 10));
            BSM::states[board] = {};
            last = board;
        }
        else
            for (unsigned int i = 11; i <= line.length(); i += 11)
            {
                string s = BSM::board(line.substr(i - 11, i - 1));
                short move = line[i - 1] - '0';
                auto entry = pair<short, string>(move, s);
                BSM::states[last].push_back(entry);
            }
        first = !first;
    }
    for (auto& e : BSM::states)
        if (!Board::is_over(e.first))
            BSM::strategies[e.first] = e.second;
    file.close();
}

void BSM::save(const string& path)
{
    ofstream file;
    file.open(path);
    for (auto& board : BSM::states)
    {
        file << BSM::str(board.first) << endl;
        for (auto& s : board.second)
            file << BSM::str(s.second) << to_string(s.first);
        file << endl;
    }
    file.close();
}

void BSM::generate(const string& path)
{
    BSM::states.clear();

    string board;
    for (int i = 0; i < 10; ++i)
        board.push_back(0);

    BSM::states[board] = {};

    BSM::genrec(board, 0, board);

    for (auto& e : BSM::states)
        if (!Board::is_over(e.first))
            BSM::strategies[e.first] = e.second;

    cout << BSM::states.size() << " board states generated" << endl;

    BSM::save(path);
}

void BSM::genrec(string& board, char p, string lastbc)
{
    auto moves = Player::get_possible_moves(board);
    for (auto& e : *moves)
    {
        board[e] = p + 1;
        board[0]++;

        string base_case = BSM::get_base_case(board);

        if (!BSM::states.count(base_case))
            BSM::states[base_case] = {};

        if (find_if(BSM::states[lastbc].begin(), BSM::states[lastbc].end(),
                [&base_case](const pair<short, string>& p)
                { return p.second == base_case; }) == BSM::states[lastbc].end())
        {
            auto p = pair<short, string>(BSM::get_move(lastbc, board, e),
                    base_case);
            BSM::states[lastbc].push_back(p);
            if (lastbc[p.first] != 0)
                throw 1;
        }

        if (!Board::is_over(board))
            genrec(board, !p, base_case);

        board[0]--;
        board[e] = 0;
    }
}

void BSM::rotate(string& board)
{
    string tmp(board);
    for (int i = 1; i < 10; ++i)
        board[i] = tmp[i * 7 % 10];
}

void BSM::sym(string& board)
{
    for (int i = 1; i < 4; ++i)
        swap(board[i], board[i + 6]);
}

string BSM::get_base_case(string const& board)
{
    string base_case(board);
    int m1 = BSM::metric1(base_case);
    int m2 = BSM::metric2(base_case);
    int m3 = BSM::metric3(base_case);
    int m4 = BSM::metric4(base_case);

    auto transforms = BSM::get_transforms(board);
    for (auto& e : *transforms)
    {
        int em1 = BSM::metric1(e);
        if (em1 < m1)
        {
            base_case = e;
            m1 = em1;
            m2 = BSM::metric2(e);
            m3 = BSM::metric3(e);
            m4 = BSM::metric4(e);
        }
        else if (em1 == m1)
        {
            int em2 = BSM::metric2(e);
            if (em2 < m2)
            {
                base_case = e;
                m2 = em2;
                m3 = BSM::metric3(e);
                m4 = BSM::metric4(e);
            }
            else if (em2 == m2)
            {
                int em3 = BSM::metric3(e);
                if (em3 < m3)
                {
                    base_case = e;
                    m3 = em3;
                    m4 = BSM::metric4(e);
                }
                else if (em3 == m3)
                {
                    int em4 = BSM::metric4(e);
                    if (em4 < m4)
                    {
                        base_case = e;
                        m4 = em4;
                    }
                }
            }
        }
    }
    return base_case;
}

int BSM::metric1(string const& board)
{
    unsigned int count = 0;
    for (int i = 1; i < 10; ++i)
        if (board[i] == 1)
            count += i;
    return count;
}

int BSM::metric2(string const& board)
{
    unsigned int count = 0;
    for (int i = 1; i < 10; ++i)
        if (board[i] == 2)
            count += i;
    return count;
}

int BSM::metric3(string const& board)
{
    unsigned int count = 1;
    for (int i = 1; i < 10; ++i)
        if (board[i] == 1)
            count *= i;
    return count;
}

int BSM::metric4(string const& board)
{
    unsigned int count = 1;
    for (int i = 1; i < 10; ++i)
        if (board[i] == 2)
            count *= i;
    return count;
}

string BSM::str(string const& board)
{
    string s;
    for (int i = 0; i < 10; ++i)
        s = s + to_string((short)board[i]);
    return s;
}

unique_ptr<vector<string>> BSM::get_transforms(string const& board)
{
    auto transforms = make_unique<vector<string>>();
    string tmp(board);
    for (int i = 0; i < 3; ++i)
    {
        BSM::rotate(tmp);
        transforms->push_back(tmp);
    }
    BSM::rotate(tmp);
    BSM::sym(tmp);
    transforms->push_back(tmp);
    for (int i = 0; i < 3; ++i)
    {
        BSM::rotate(tmp);
        transforms->push_back(tmp);
    }
    return transforms;
}

string BSM::board(string const& base)
{
    string board;
    for (int i = 0; i < 10; ++i)
        board.push_back(base[i] - '0');
    return board;
}

void BSM::print_strategy()
{
    for (auto& e : BSM::strategies)
    {
        cout << BSM::str(e.first) << " : ";
        for (auto& s : e.second)
            cout << "<" << s.first << ", " << BSM::str(s.second) << "> ";
        cout << endl;
    }
}

short BSM::diff1(string const& s, string const& f)
{
    short found = 0;
    short move = 0;
    for (short i = 1; i < 10; ++i)
        if (found > 1)
            return 0;
        else if (s[i] != f[i])
        {
            move = i;
            found++;
        }
    if (found == 1)
        return move;
    return 0;
}

short BSM::get_move(string bc, string b, short move)
{
    move = BSM::diff1(b, bc);
    if (move)
        return move;
    auto transforms = BSM::get_transforms(b);
    for (const auto& transform : *transforms)
    {
        short move = BSM::diff1(transform, bc);
        if (move)
            return move;
    }
    throw 1;
}

short BSM::whos_turn(string const& bc)
{
    short countX = 0;
    short countO = 0;
    for (int i = 1; i < 10; ++i)
        if (bc[i] == 1)
            countX++;
        else if (bc[i] == 2)
            countO++;
    if (countX > countO)
        return 2;
    return 1;
}
