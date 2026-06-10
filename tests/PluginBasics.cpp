#include "helpers/test_helpers.h"
#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("one is equal to one", "[dummy]")
{
    REQUIRE (1 == 1);
}

TEST_CASE ("Plugin instance", "[instance]")
{
    Dumumub0000004AudioProcessor testPlugin;

    SECTION ("has a name")
    {
        CHECK (testPlugin.getName().isNotEmpty());
    }
}
