#include <memory>

#include "utils/make_unique.h"
#include "genetic.h"

using namespace std;

Genetic::Genetic(int p)
{
    this->population = make_unique<vector<unique_ptr<Individual>>>();

    for (int i = 0; i < p; ++i)
    {
        auto individual = make_unique<Individual>(1);
        this->population->push_back(move(individual));
    }
}

Genetic::~Genetic()
{
}

