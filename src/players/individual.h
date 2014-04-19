#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <memory>
#include <utility>

#include "player.h"

typedef std::vector<std::pair<unsigned short, unsigned short>> Strategies;

class Individual : public Player
{
public:
    Individual (short id);
    virtual ~Individual ();

    short get_move(std::shared_ptr<Board> board) const;

    float get_fitness() const;

    void mutate();

    Strategies const* get_strategies();

private:
    std::unique_ptr<Strategies> strategies;
};

#endif

