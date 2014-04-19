#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>

#include "utils/make_unique.h"
#include "bstates_manager.h"
#include "player.h"
#include "board.h"

using namespace std;

map<string, vector<string>> BSM::states = {};

void BSM::load(const string& path)
{
    BSM::states.clear();
    ifstream file;
    file.open(path);
    string line;
    while (getline(file, line))
    {
        if (line.length() < 10)
            continue;
        string board = line.substr(0, 10);
        BSM::states[board] = {};
    }
    for (auto& b : BSM::states)
        for (auto& s : BSM::states)
            if (BSM::is_successor(s.first, b.first))
                b.second.push_back(s.first);
    file.close();
}

void BSM::save(const string& path)
{
    ofstream file;
    file.open(path);
    for (auto& board : BSM::states)
    {
        for (auto& c : board.first)
            file << (short)c;
        file << "\n";
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

    BSM::genrec(board, 0);

    cout << BSM::states.size() << " board states generated" << endl;

    BSM::save(path);
}

void BSM::genrec(string& board, char p)
{
    if (BSM::states.size() >= 765)
        return;
    auto moves = Player::get_possible_moves(board);
    for (auto& e : *moves)
    {
        board[e] = p + 1;
        board[0]++;

        string base_case = BSM::get_base_case(board);

        if (!BSM::states.count(base_case))
            BSM::states[base_case] = {};

        if (!Board::is_over(board))
            genrec(board, !p);

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

bool BSM::is_successor(std::string s, string f)
{
    if (s[0] != (f[0] + 1))
        return false;
    auto transforms = BSM::get_transforms(s);
    for (auto& t : *transforms)
    {
        short cell = 0;
        bool found = 0;
        for (short i = 1; i < 10; ++i)
            if (found)
                break;
            else if (t[i] != f[i])
            {
                found = 1;
                cell = i;
            }
        if (found && t[cell] != 0 && f[cell])
            return true;
    }
    return false;
}

void BSM::print(std::string const& board)
{
    cout << "(" << (short)board[0] << ")";
    for (int i = 1; i < 10; ++i)
        cout << (short)board[i];
    cout << endl;
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
