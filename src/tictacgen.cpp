#include <memory>
#include <iostream>
#include <fstream>

#include "utils/make_unique.h"
#include "game.h"
#include "players/human.h"
#include "players/minmax.h"
#include "players/individual.h"
#include "utils/bsm.h"
#include "genetic.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    BSM::load("resources/states");

    //Genetic genetic;

    //genetic.run();
    
    auto p1 = make_shared<Individual>(2);
    p1->load("resources/individuals/2014-04-20-19-26-07");
    auto p2 = make_shared<Human>(1);
    Game g(p1, p2);
    g.run(2);

    return 0;
}
