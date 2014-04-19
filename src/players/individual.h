#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <memory>
#include <map>

#include "player.h"

typedef std::map<std::string, std::string> Strategy;

class Individual : public Player
{
public:
    Individual (short id);
    virtual ~Individual ();

    short get_move(std::shared_ptr<Board> board) const;

    float get_fitness() const;

    void mutate();

    std::string operator[](std::string const& board);

private:
    std::unique_ptr<Strategy> strategy;
};

#endif

