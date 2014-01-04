#include "unity_fixture.h"
#include "edge.c"
#include "helper.c"
#include "server.c"

TEST_GROUP(server);
TEST_SETUP(server) { }
TEST_TEAR_DOWN(server) { } 

// ===============================================================================

TEST(server, test_tmp){
    TEST_ASSERT_EQUAL(1, 1);
}

// ===============================================================================

TEST_GROUP_RUNNER(server) {
    RUN_TEST_CASE(server, test_tmp);
}

static void runServerTests() {
    RUN_TEST_GROUP(server);
}

int main(int argc, char* argv[]) {
    return UnityMain(argc, argv, runServerTests);
}
