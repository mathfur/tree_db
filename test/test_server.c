#include "unity_fixture.h"
#include "server.c"

TEST_GROUP(server);
TEST_SETUP(server) { }
TEST_TEAR_DOWN(server) { } 

// ===============================================================================

TEST_GROUP_RUNNER(server) {
}

static void runServerTests() {
    RUN_TEST_GROUP(server);
}

int main(int argc, char* argv[]) {
    return UnityMain(argc, argv, runServerTests);
}
