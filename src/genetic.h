#ifndef GENETIC_H
#define GENETIC_H

#include <memory>
#include <vector>

#include "players/individual.h"

class Genetic
{
public:
    Genetic (int p);
    virtual ~Genetic ();

private:
    std::unique_ptr<std::vector<std::unique_ptr<Individual>>> population;
};

#endif
