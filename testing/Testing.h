//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_TESTUNG_H
#define STRANGEGAME_TESTUNG_H

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "docktest.h"
#include "../game/world/octree/Octree.hpp"

TEST_CASE("Octree instancing") {
    const Octree octree(8);
    CHECK(octree.nodesCount() == 1);
}

#endif //STRANGEGAME_TESTUNG_H
