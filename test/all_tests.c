#include "unity_fixture.h"

static void runAllTests() {
  RUN_TEST_GROUP(edge);
}

int main(int argc, char* argv[]) {
  return UnityMain(argc, argv, runAllTests);
}
