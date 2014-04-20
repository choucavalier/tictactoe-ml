#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "utils/make_unique.h"
#include "genetic.h"
#include "game.h"

using namespace std;

const double Genetic::P = 200;
const double Genetic::R = 0.6;
const double Genetic::M = 0.05;

Genetic::Genetic() : population(make_unique<vector<shared_ptr<Individual>>>()),
    max_fitness(0), min_fitness(10000),
    history(make_unique<vector<HistoryEntry>>())
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
    while (generations < 200 && this->max_fitness < 140)
    {
        this->next_gen();
        HistoryEntry entry;
        entry.max_fitness = this->max_fitness;
        entry.min_fitness = this->min_fitness;
        entry.mean_fitness = this->mean_fitness;
        entry.generation = generations;
        this->history->push_back(entry);
        generations++;
        cout << "Generation #" << generations << ": ";
        cout << this->min_fitness << ", " << this->max_fitness;
        cout << "(mean=" << this->mean_fitness << ")";
        cout << " " << this->population->size() << " individuals" << endl;
    }
    this->save();
}

void Genetic::next_gen()
{
    this->min_fitness = 10000;
    this->max_fitness = 0;
    auto counts = map<shared_ptr<Individual>, tuple<int, int, int>>();
    for (auto p1 : *this->population)
    {
        int losses = 0, won = 0, draw = 0;
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
            else if (g.get_winner_id() == pid)
                won++;
            else
                draw++;
        }
        counts[p1] = tuple<int, int, int>(draw, won, losses);
    }
    auto countdraw = map<int, int>();
    auto countlost = map<int, int>();
    auto countwin = map<int, int>();

    for (auto& e : counts)
    {
        countdraw[get<0>(e.second)]++;
        countlost[get<1>(e.second)]++;
        countwin[get<2>(e.second)]++;
    }

    this->sum_fitness = 0;
    for (auto& e : counts)
    {
        //double fitness = 100 * (1 / (double)countscount[e.second.second]) *
            //(1 - ((double)e.second.second / (double)(Genetic::P - 1)));
        double fitness = 100 * (double)(get<2>(e.second) + get<0>(e.second))
            / (double)(Genetic::P - 1);
        fitness -= 100 * (double)get<1>(e.second) / (double)(Genetic::P - 1);
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

    // choose mumbers randomly, weighted by their fitness
    unsigned int to_select = (1 - Genetic::R) * Genetic::P;
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

    // choose random parents & generate children
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
    random_shuffle(np->begin(), np->end());
    
    // mutate some members of the new population
    unsigned int to_mutate = Genetic::M * Genetic::P;
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

void Genetic::save()
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S",timeinfo);
    string time = buffer;

    for (auto& e : *this->population)
        if (e->get_fitness() == this->max_fitness)
        {
            e->save("resources/individuals/" + time);
            break;
        }
    ofstream file;
    file.open("resources/histories/" + time);
    for (auto& e : *this->history)
    {
        file << e.generation << endl;
        file << e.max_fitness << endl;
        file << e.min_fitness << endl;
        file << e.mean_fitness << endl;
    }
    file.close();
}

