#ifndef GENETIC_H
#define GENETIC_H

#include <memory>
#include <vector>
#include <tuple>

#include "players/individual.h"
#include "players/minmax.h"

struct HistoryEntry
{
    int generation;
    float min_fitness;
    float max_fitness;
    float mean_fitness;
};

class Genetic
{
public:
    Genetic ();
    virtual ~Genetic ();

    void run();

    static const double P;
    static const double R;
    static const double M;

    double get_max_fitness();
    
    void next_gen();
    void evolve();

private:
    std::unique_ptr<std::vector<std::shared_ptr<Individual>>> population;

    double max_fitness;
    double min_fitness;
    double mean_fitness;
    double sum_fitness;

    std::shared_ptr<Individual> cross_over(std::shared_ptr<Individual> mama,
            std::shared_ptr<Individual> papa);

    std::unique_ptr<std::vector<HistoryEntry>> history;

    void save();

    std::shared_ptr<MinMax> teacher;
};

#endif
