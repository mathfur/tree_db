#include "unity_fixture.h"
#include "edge.c"

TEST_GROUP(edge);
TEST_SETUP(edge) { }
TEST_TEAR_DOWN(edge) { } 

TEST(edge, test_get_key) {
    TEST_ASSERT_EQUAL((int)'a', get_key(1000, "a"));
    TEST_ASSERT_EQUAL((int)'b', get_key(1000, "b"));

    TEST_ASSERT_EQUAL(    get_key(1000, "ba"), get_key(1000, "ab"));
    TEST_ASSERT_NOT_EQUAL(get_key(1000, "a"), get_key(1000, "ab"));
}

TEST(edge, test_init_table){
    HashTable* tbl = init_table(1000);

    int i;
    for(i = 0;i < tbl->size;i++){
        TEST_ASSERT_EQUAL(NULL, tbl->start_table[i].edge);
        TEST_ASSERT_EQUAL(NULL, tbl->start_table[i].next);
    }

    free_table(tbl);
}

TEST_GROUP_RUNNER(edge) {
  RUN_TEST_CASE(edge, test_get_key);
  RUN_TEST_CASE(edge, test_init_table);
}
