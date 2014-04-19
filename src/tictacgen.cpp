#include <memory>
#include <iostream>

#include "utils/make_unique.h"
#include "game.h"
#include "players/human.h"
#include "players/minmax.h"
#include "utils/bsm.h"

using namespace std;

int main(int argc, char *argv[])
{
    //BSM::generate("resources/combinations");
    BSM::load("resources/combinations");
    for (auto& e : BSM::states)
    {
        BSM::print(e.first);
        cout<< " : ";
        for (auto& s : e.second)
        {
            BSM::print(s);
            cout << ", ";
        }
        cout << endl;
    }
    //auto p1 = make_unique<Human>(1);
    //auto p2 = make_unique<MinMax>(2);
    //Game g (std::move(p1), std::move(p2));
    //g.run(1);
    return 0;
}
