#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <memory>
#include <map>

#include "player.h"

typedef std::map<std::string, std::pair<short, std::string>> Strategy;

class Individual : public Player
{
public:
    Individual (short id);
    virtual ~Individual ();

    short get_move(std::shared_ptr<Board> board) const;

    float get_fitness() const;
    void set_fitness(float const& fitness);

    void mutate();

    std::shared_ptr<Strategy> get_strategy() const;

    void save(std::string const& path);
    void load(std::string const& path);

private:
    float fitness;
    std::shared_ptr<Strategy> strategy;
};

#endif

