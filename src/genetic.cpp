#include <memory>
#include <iostream>
#include <algorithm>

#include "utils/make_unique.h"
#include "genetic.h"
#include "game.h"

using namespace std;

const double Genetic::P = 200;
const double Genetic::R = 0.6;
const double Genetic::M = 0.05;

Genetic::Genetic() : population(make_unique<vector<shared_ptr<Individual>>>()),
    max_fitness(0), min_fitness(10000)
{
    for (int i = 0; i < Genetic::P; ++i)
    {
        auto individual = make_shared<Individual>(1);
        this->population->push_back(move(individual));
    }
}

Genetic::~Genetic()
{
}

void Genetic::run()
{
    int generations = 0;
    while (generations < 200 || this->max_fitness < 99.5)
    {
        this->next_gen();
        generations++;
        cout << "Generation #" << generations << ": ";
        cout << this->min_fitness << ", " << this->max_fitness;
        cout << "(mean=" << this->mean_fitness << ")";
        cout << " " << this->population->size() << " individuals" << endl;
    }
}

void Genetic::next_gen()
{
    this->min_fitness = 10000;
    this->max_fitness = 0;
    auto counts = map<shared_ptr<Individual>, int>();
    for (auto p1 : *this->population)
    {
        int losses = 0;
        for (auto p2 : *this->population)
        {
            if (&(*p1) == &(*p2))
                continue;
            int pid = rand() % 2 + 1;
            int oid = pid == 1 ? 2 : 1;
            p1->set_id(pid);
            p2->set_id(oid);
            Game g(p1, p2);
            g.run();
            if (g.get_winner_id() == oid)
                losses++;
        }
        counts[p1] = losses;
    }
    auto countscount = map<int, int>();

    for (auto& e : counts)
        countscount[e.second]++;

    this->sum_fitness = 0;
    for (auto& e : counts)
    {
        double fitness = 100 * (1 / (double)countscount[e.second]) *
            (1 - ((double)e.second / (double)(Genetic::P - 1)));
        this->sum_fitness += fitness;
        e.first->set_fitness(fitness);
        if (fitness > this->max_fitness)
            this->max_fitness = fitness;
        if (fitness < this->min_fitness)
            this->min_fitness = fitness;
    }
    this->mean_fitness = sum_fitness / Genetic::P;
    evolve();
}

double Genetic::get_max_fitness()
{
    return this->max_fitness;
}


void Genetic::evolve()
{
    random_shuffle(this->population->begin(), this->population->end());
    auto np = make_unique<vector<shared_ptr<Individual>>>();
    unsigned int to_select = (1 - Genetic::R) * Genetic::P;
    cout << to_select << " randomly selected" << endl;
    while (np->size() < to_select)
        for (auto& individual : *this->population)
        {
            if (np->size() >= to_select)
                break;
            if ((double) rand() / (RAND_MAX) <
                    (individual->get_fitness() / this->sum_fitness))
                np->push_back(individual);
        }
    random_shuffle(this->population->begin(), this->population->end());
    unsigned int to_crossover = Genetic::R * Genetic::P;
    auto parents1 = vector<shared_ptr<Individual>>();
    for (unsigned int i = 0; i < to_crossover; ++i)
        parents1.push_back(this->population->at(i));
    random_shuffle(this->population->begin(), this->population->end());
    auto parents2 = vector<shared_ptr<Individual>>();
    for (unsigned int i = 0; i < to_crossover; ++i)
        parents2.push_back(this->population->at(i));
    for (unsigned int i = 0; i < to_crossover; ++i)
        np->push_back(cross_over(parents1[i], parents2[i]));
    cout << to_crossover << " from cross over" << endl;
    random_shuffle(np->begin(), np->end());
    unsigned int to_mutate = Genetic::M * Genetic::P;
    cout << to_mutate << " mutated" << endl;
    for (unsigned int i = 0; i < to_mutate; ++i)
        np->at(i)->mutate();
    this->population = move(np);
}

shared_ptr<Individual> Genetic::cross_over(shared_ptr<Individual> mama,
        shared_ptr<Individual> papa)
{
    auto baby = make_shared<Individual>(1);
    for (auto& e : *baby->get_strategy())
        if (rand() % 2)
            e.second = papa->get_strategy()->at(e.first);
        else
            e.second = mama->get_strategy()->at(e.first);
    return baby;
}

