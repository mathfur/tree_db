#include "unity_fixture.h"
#include "tree.c"

TEST_GROUP(tree);
TEST_SETUP(tree) { }
TEST_TEAR_DOWN(tree) { } 

TEST(tree, test_tmp){
    TEST_ASSERT_EQUAL(1, 1);
}

TEST_GROUP_RUNNER(tree) {
    RUN_TEST_CASE(tree, test_tmp);
}

static void runTreeTests() {
  RUN_TEST_GROUP(tree);
}

int main(int argc, char* argv[]) {
  return UnityMain(argc, argv, runTreeTests);
}
