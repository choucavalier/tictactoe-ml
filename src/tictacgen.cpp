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
    BSM::generate("resources/states");
    BSM::load("resources/states");

    for (auto& e : BSM::strategies)
    {
        cout << BSM::str(e.first) << " : ";
        for (auto& s : e.second)
            cout << BSM::str(s.second) << "(" << s.first << ")";
        cout << endl;
    }

    //Genetic genetic(1000);

    auto p1 = make_unique<Human>(1);
    auto p2 = make_unique<Individual>(2);
    p2->initialize();
    Game g (move(p1), move(p2));
    g.run(1);
    return 0;
}
