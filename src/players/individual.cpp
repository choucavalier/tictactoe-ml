#include "players/individual.h"

using namespace std;

Individual::Individual(short id) : Player(id)
{
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

Strategies const* Individual::get_strategies()
{
}

