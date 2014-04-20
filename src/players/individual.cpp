#include <map>
#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>

#include "utils/make_unique.h"
#include "players/individual.h"
#include "utils/bsm.h"
#include "board.h"

using namespace std;

Individual::Individual(short id) : Player(id), fitness(0),
    strategy(make_shared<Strategy>())
{
    for (auto& bs : BSM::strategies)
    {
        random_shuffle(bs.second.begin(), bs.second.end());
        auto entry = pair<string, pair<short, string>>(bs.first, bs.second[0]);
        this->strategy->insert(entry);
    }
}

Individual::~Individual()
{
}


float Individual::get_fitness() const
{
    return this->fitness;
}

void Individual::set_fitness(float const& fitness)
{
    this->fitness = fitness;
}


short Individual::get_move(shared_ptr<Board> board) const
{
    string bc = BSM::get_base_case(board->get_map());
    short move;
    string sbc;
    tie(move, sbc) = this->strategy->find(bc)->second;
    bc[move] = this->id;
    short real_move = BSM::get_move(board->get_map(), bc, move);
    return real_move;
}


void Individual::mutate()
{
    int to_mutate = rand() % 10 + 3;
    vector<string> bcs = {};
    for (auto& e : *this->strategy)
        if (BSM::strategies[e.first].size() > 1)
        {
            bcs.push_back(e.first);
            random_shuffle(BSM::strategies[e.first].begin(),
                    BSM::strategies[e.first].end());
        }
    random_shuffle(bcs.begin(), bcs.end());
    for (int i = 0; i < to_mutate; ++i)
        this->strategy->at(bcs[i]) = BSM::strategies[bcs[i]][0];
}

shared_ptr<Strategy> Individual::get_strategy() const
{
    return this->strategy;
}

void Individual::load(std::string const& path)
{
    this->strategy->clear();
    
    ifstream file;
    file.open(path);

    string line;
    while (getline(file, line))
    {
        string next;
        getline(file, next);
        auto entry = pair<short, string>(next[10] - '0',
                BSM::board(next.substr(0, 10)));
        string bs = BSM::board(line.substr(0, 10));
        this->strategy->insert(pair<string, pair<short, string>>(bs, entry));
    }
}


void Individual::save(std::string const& path)
{
    ofstream file;
    file.open(path);
    for (auto& e : *this->strategy)
    {
        file << BSM::str(e.first) << endl;
        file << BSM::str(e.second.second) << e.second.first << endl;
    }
}

