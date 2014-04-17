#include <memory>
#include <iostream>

#include "utils/make_unique.h"
#include "game.h"
#include "players/human.h"
#include "players/minmax.h"
#include "utils/bstates_manager.h"

int main(int argc, char *argv[])
{
    auto p1 = make_unique<Human>(1);
    auto p2 = make_unique<MinMax>(2);
    Game g (std::move(p1), std::move(p2));
    g.run(1);
    return 0;
}
