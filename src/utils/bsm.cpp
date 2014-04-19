#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>

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
            if (!Board::is_over(board))
                BSM::strategies[board] = {};
            last = board;
        }
        else
            for (unsigned int i = 11; i <= line.length(); i += 11)
            {
                string s = BSM::board(line.substr(i - 11, i - 1));
                short move = line[i - 1] - '0';
                auto entry = pair<short, string>(move, s);
                BSM::states[last].push_back(entry);
                if (!Board::is_over(last))
                    BSM::strategies[last].push_back(entry);
            }
        first = !first;
    }
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

    string lastbc = board;

    BSM::genrec(board, 0, lastbc);

    //for (auto& f : BSM::states)
        //for (auto& s : BSM::states)
        //{
            //bool is_successor;
            //short move;
            //tie(is_successor, move) = BSM::successor(s.first, f.first);
            //if (is_successor)
                //f.second.push_back(pair<short, string>(move, s.first));
        //}

    cout << BSM::states.size() << " board states generated" << endl;

    BSM::save(path);
}

void BSM::genrec(string& board, char p, string const& lastbc)
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
        {
            BSM::states[base_case] = {};
            BSM::states[lastbc].push_back(pair<short, string>(e, base_case));
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

pair<bool, short> BSM::successor(string const& s, string const& f)
{
    if (s[0] != (f[0] + 1))
        return pair<bool, short>(false, 0);
    auto transforms = BSM::get_transforms(s);
    transforms->push_back(s);
    for (auto& t : *transforms)
    {
        short move = BSM::diff(s, f);
        if (move != 0)
            return pair<bool, short>(true, BSM::get_move(s, t, move));
    }
    return pair<bool, short>(false, 0);
}

short BSM::diff(string const& s, string const& f)
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

short BSM::get_move(string const& s, string t, short move)
{
    if (t == s)
        return move;
    string buf;
    buf.push_back(0);
    for (short i = 1; i < 10; ++i)
        buf.push_back(i);
    for (int i = 0; i < 3; ++i)
    {
        if (t == s)
            return move;
        BSM::rotate(t);
        BSM::rotate(buf);
        move = buf[move];
    }
    BSM::rotate(t);
    BSM::rotate(buf);
    move = buf[move];
    BSM::sym(t);
    BSM::sym(buf);
    move = buf[move];
    for (int i = 0; i < 3; ++i)
    {
        if (t == s)
            return move;
        BSM::rotate(t);
        BSM::rotate(buf);
        move = buf[move];
    }
    return move;
}

string BSM::board(string const& base)
{
    string board;
    for (int i = 0; i < 10; ++i)
        board.push_back(base[i] - '0');
    return board;
}

