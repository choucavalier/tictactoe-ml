#include <map>
#include <algorithm>
#include <utility>
#include <iostream>

#include "utils/make_unique.h"
#include "players/individual.h"
#include "utils/bsm.h"

using namespace std;

Individual::Individual(short id) : Player(id), strategy(make_unique<Strategy>())
{
}

Individual::~Individual()
{
}

void Individual::initialize()
{
    for (auto& bs : BSM::strategies)
    {
        random_shuffle(bs.second.begin(), bs.second.end());
        auto entry = pair<string, pair<short, string>>(bs.first, bs.second[0]);
        //this->strategy->insert(entry);
    }
}


float Individual::get_fitness() const
{
    return 1.0;
}


short Individual::get_move(shared_ptr<Board> board) const
{
    return 1;
}


void Individual::mutate()
{
}

