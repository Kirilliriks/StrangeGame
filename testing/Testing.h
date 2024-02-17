//
// Created by KirilliriK on 23.05.2023.
//

#ifndef STRANGEGAME_TESTUNG_H
#define STRANGEGAME_TESTUNG_H

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "docktest.h"
#include "../game/world/Octree.hpp"
#include "../game/utils/Paths.hpp"

TEST_CASE("Octree create") {
    Octree octree;
    CHECK(octree.nodesCount() == 1);
}

TEST_CASE("Path check") {
    REQUIRE(Paths::shader == "TODO");
}

#endif //STRANGEGAME_TESTUNG_H
