#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

#include "utils/make_unique.h"
#include "bstates_manager.h"
#include "player.h"

using namespace std;

map<int, shared_ptr<vector<shared_ptr<Board>>>> BSM::cache = {};

unique_ptr<vector<unique_ptr<Board>>> BSM::load(const string& path)
{
    auto loaded = make_unique<vector<unique_ptr<Board>>>();
    return loaded;
}

void BSM::save(Boards boards, const string& path)
{
    ofstream file;
    file.open(path);
    for (auto& board : *boards)
        file << board->in_string() << "\n";
    file.close();
}

void BSM::generate(const string& path)
{
    auto boards = make_shared<vector<shared_ptr<Board>>>();
    auto board = make_shared<Board>();

    auto copy = board->copy();

    BSM::cache[BSM::hash(board)] = make_shared<vector<shared_ptr<Board>>>();
    BSM::cache[BSM::hash(board)]->push_back(copy);

    boards->push_back(copy);

    BSM::genrec(board, boards, 0);

    cout << boards->size() << " board states generated" << endl;

    BSM::save(boards, path);

    BSM::cache.clear();
}

void BSM::genrec(shared_ptr<Board> board, Boards boards, char p)
{
    auto moves = Player::get_possible_moves(board);
    for (auto& e : *moves)
    {
        board->update(e, p + 1);

        auto base_case = BSM::get_base_case(board);

        if (!BSM::is_generated(base_case))
        {
            boards->push_back(base_case);
            int hash = BSM::hash(base_case);
            if (BSM::cache[hash] == nullptr)
                BSM::cache[hash] = make_shared<vector<shared_ptr<Board>>>();
            BSM::cache[hash]->push_back(base_case);
        }

        if (boards->size() <= 765 && !board->is_over())
            genrec(board, boards, !p);

        board->undo(e);
    }
}

bool BSM::is_generated(shared_ptr<Board> board)
{
    int hash = BSM::hash(board);
    if (BSM::cache[hash] == nullptr)
        return false;
    for (auto& e : *BSM::cache[hash])
        if (BSM::eq(board, e))
            return true;
    return false;
}

bool BSM::eq(shared_ptr<Board> b1, shared_ptr<Board> b2)
{
    if (*b1->get_cells() != *b2->get_cells())
        return false;

    return true;
}

void BSM::rotate(shared_ptr<Board> board)
{
    auto tmp = board->copy();
    for (int i = 0; i < 9; ++i)
        board->set_cell(i, tmp->get_cells()->at(((i + 1) * 7 % 10) - 1));
}

void BSM::sym(shared_ptr<Board> board)
{
    for (int i = 0; i < 3; ++i)
        board->swap(i, i + 6);
}

int BSM::hash(shared_ptr<Board> board)
{
    int count = 0;
    for (int i = 0; i < 9; ++i)
        count += board->get_cells()->at(i);
    return count;
}

shared_ptr<Board> BSM::get_base_case(shared_ptr<Board> board)
{
    auto transforms = make_shared<vector<shared_ptr<Board>>>();
    auto tmp = board->copy();
    for (int i = 0; i < 3; ++i)
    {
        BSM::rotate(tmp);
        transforms->push_back(tmp->copy());
    }
    BSM::rotate(tmp);
    BSM::sym(tmp);
    transforms->push_back(tmp->copy());
    for (int i = 0; i < 3; ++i)
    {
        BSM::rotate(tmp);
        transforms->push_back(tmp->copy());
    }
    auto base_case = board->copy();
    int m1 = BSM::metric1(base_case);
    int m2 = BSM::metric2(base_case);
    int m3 = BSM::metric3(base_case);
    int m4 = BSM::metric4(base_case);

    for (auto& e : *transforms)
    {
        int em1 = BSM::metric1(e);
        if (em1 < m1)
        {
            base_case = e->copy();
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
                base_case = e->copy();
                m2 = em2;
                m3 = BSM::metric3(e);
                m4 = BSM::metric4(e);
            }
            else if (em2 == m2)
            {
                int em3 = BSM::metric3(e);
                if (em3 < m3)
                {
                    base_case = e->copy();
                    m3 = em3;
                    m4 = BSM::metric4(e);
                }
                else if (em3 == m3)
                {
                    int em4 = BSM::metric4(e);
                    if (em4 < m4)
                    {
                        base_case = e->copy();
                        m4 = em4;
                    }
                }
            }
        }
    }
    return base_case;
}

int BSM::metric1(shared_ptr<Board> board)
{
    int count = 0;
    for (int i = 0; i < 9; ++i)
        if (board->get_cells()->at(i) == 1)
            count += (i + 1);
    return count;
}

int BSM::metric2(shared_ptr<Board> board)
{
    int count = 0;
    for (int i = 0; i < 9; ++i)
        if (board->get_cells()->at(i) == 2)
            count += (i + 1);
    return count;
}

int BSM::metric3(shared_ptr<Board> board)
{
    int count = 1;
    for (int i = 0; i < 9; ++i)
        if (board->get_cells()->at(i) == 1)
            count *= (i + 1);
    return count;
}

int BSM::metric4(shared_ptr<Board> board)
{
    int count = 1;
    for (int i = 0; i < 9; ++i)
        if (board->get_cells()->at(i) == 2)
            count *= (i + 1);
    return count;
}

