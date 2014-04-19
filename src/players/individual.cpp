#include <algorithm>

#include "players/individual.h"
#include "utils/bsm.h"

using namespace std;

Individual::Individual(short id) : Player(id)
{
    for (auto& bs : BSM::states)
    {
        random_shuffle(bs.second.begin(), bs.second.end());
        this->strategy->at(bs.first) = bs.second[0];
    }
}

Individual::~Individual()
{
}


float Individual::get_fitness() const
{
}


short Individual::get_move(shared_ptr<Board> board) const
{

}


void Individual::mutate()
{
}

