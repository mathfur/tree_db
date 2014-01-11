#include "unity_fixture.h"
#include "edge.c"

TEST_GROUP(edge);
TEST_SETUP(edge) { }
TEST_TEAR_DOWN(edge) { } 

// ===============================================================================

TEST(edge, test_inspect_edge){
  char buf[BUFSIZ];

  Edge edge1 = {"foo", "bar"};
  TEST_ASSERT_EQUAL(inspect_edge(&edge1, buf, BUFSIZ), 7);
  TEST_ASSERT_EQUAL_STRING(buf, "foo,bar");

  Edge edge2 = {"1", "12"};
  TEST_ASSERT_EQUAL(inspect_edge(&edge2, buf, BUFSIZ), 4);
  TEST_ASSERT_EQUAL_STRING(buf, "1,12");
}

// ===============================================================================

TEST(edge, test_is_equal_edge_is_true){
    Edge edge1 = {"foo", "bar"};
    Edge edge2 = {"foo", "bar"};

    TEST_ASSERT_EQUAL(is_equal_edge(&edge1, &edge2), 1);
}

TEST(edge, test_is_equal_edge_is_false){
    Edge edge1 = {"foo", "bar"};
    Edge edge2 = {"foo", "baz"};

    TEST_ASSERT_EQUAL(is_equal_edge(&edge1, &edge2), 0);
}

// ===============================================================================

TEST(edge, test_get_key) {
    TEST_ASSERT_EQUAL((int)'a', get_key(1000, "a"));
    TEST_ASSERT_EQUAL((int)'b', get_key(1000, "b"));

    TEST_ASSERT_EQUAL(    get_key(1000, "ba"), get_key(1000, "ab"));
    TEST_ASSERT_NOT_EQUAL(get_key(1000, "a"), get_key(1000, "ab"));
}

// ===============================================================================

TEST(edge, test_init_table){
    HashTable* tbl = init_table(1000);

    int i;
    for(i = 0;i < tbl->size;i++){
        TEST_ASSERT_EQUAL(NULL, tbl->start_table[i].edge);
        TEST_ASSERT_EQUAL(NULL, tbl->start_table[i].next);
    }

    free_table(tbl);
}

// ===============================================================================

TEST(edge, test_free_hash_elem__0_edge){
    HashElem* hash_elem = (HashElem *) malloc(sizeof(HashElem));
    Edge *edge;

    hash_elem->edge = NULL;
    hash_elem->next = NULL;

    free_hash_elem(hash_elem, 1);

    free(hash_elem);
}

TEST(edge, test_free_hash_elem__1_edge){
    HashElem* hash_elem = (HashElem *) malloc(sizeof(HashElem));

    char* start_entity = (char*)malloc(sizeof(char) * 4);
    char* end_entity   = (char*)malloc(sizeof(char) * 4);
    Edge *edge = (Edge *) malloc(sizeof(Edge));

    edge->start = start_entity;
    edge->end = end_entity;

    hash_elem->edge = edge;
    hash_elem->next = NULL;

    free_hash_elem(hash_elem, 1);
}

// ===============================================================================

TEST(edge, test_search_edges_by_start_and_end__with_0_edges){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが無い場合

    TEST_ASSERT_EQUAL(search_edges_by_start_and_end(tbl, "foo", "bar", result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_search_edges_by_start_and_end__with_1_edges){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, bar)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);

    TEST_ASSERT_EQUAL(search_edges_by_start_and_end(tbl, "foo", "bar", result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");

    free_table(tbl);
}

TEST(edge, test_search_edges_by_start_and_end__with_2_edges){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, bar), (foo, baz)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "baz"), 0);

    TEST_ASSERT_EQUAL(search_edges_by_start_and_end(tbl, "foo", "bar", result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");

    TEST_ASSERT_EQUAL(search_edges_by_start_and_end(tbl, "foo", "baz", result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "baz");

    TEST_ASSERT_EQUAL(search_edges_by_start_and_end(tbl, "bar", "baz", result, 10), 0);

    free_table(tbl);
}

// ===============================================================================

TEST(edge, test_get_edges_by_start_with_depth_0){
    HashTable* tbl = init_table(1000);
    Edge *result;

    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "foo", result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_edges_by_start_with_one_edge){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが"foo"と"bar"のみの場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);

    // 中心がfooの場合, (foo, bar)のみが返る
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "foo", result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");

    // 中心がbarの場合, 0が返る
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "bar", result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_edges_by_start_with_2_edge){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, bar), (foo, baz)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "baz"), 0);

    // 中心がfooの場合, (foo, bar), (foo, baz)が返る
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "foo", result, 10), 2);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[1]).end,   "baz");

    // 中心がbarの場合, 何も返らない
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "bar", result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_edges_when_start_is_equal_to_end_with_1_edge){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, foo)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "foo"), 0);

    // 中心がfooの場合, (foo, foo)が返る
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "foo", result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "foo");

    // 中心がbarの場合, 何も返らない
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "bar", result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_edges_when_start_is_equal_to_end_with_2_edge){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, foo), (foo, foo)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "foo"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "foo"), 0);

    // 中心がfooの場合, (foo, foo)が返る ( 重複が合っても1個しか返さない )
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "foo", result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "foo");

    // 中心がbarの場合, 何も返らない
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "bar", result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_edges_by_start_with_small_size_table){
    HashTable* tbl = init_table(1);
    Edge result[10];

    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "e1", "foo"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "e2", "foo"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "e3", "foo"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "e4", "foo"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "e5", "foo"), 0);

    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "e1", result, 10), 1);
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "e2", result, 10), 1);
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "e3", result, 10), 1);
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "e4", result, 10), 1);
    TEST_ASSERT_EQUAL(get_edges_by_start(tbl, "e5", result, 10), 1);

    free_table(tbl);
}

// ===============================================================================

TEST(edge, test_get_descendants_with_negative_depth){
    HashTable* tbl = init_table(1000);
    Edge *result;

    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", -1, result, 10), -1);

    free_table(tbl);
}

TEST(edge, test_get_descendants_with_depth_0){
    HashTable* tbl = init_table(1000);
    Edge *result;

    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 0, result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_descendants_with_one_edge){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが"foo"と"bar"のみの場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);

    // depthが1で、中心がfooの場合, (foo, bar)のみが返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 1, result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");

    // depthが2で、中心がfooの場合, (foo, bar)のみが返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 2, result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");

    // depthが1で、中心がbarの場合, 何も返らない
    TEST_ASSERT_EQUAL(get_descendants(tbl, "bar", 1, result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_descendants_with_2_edge){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, bar), (foo, baz)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "baz"), 0);

    // depthが0で、中心がfooの場合, 何も返らない。
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 0, result, 10), 0);

    // depthが1で、中心がfooの場合, (foo, bar), (foo, baz)が返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 1, result, 10), 2);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[1]).end,   "baz");

    // depthが2で、中心がfooの場合, (foo, bar), (foo, baz)が返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 2, result, 10), 2);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[1]).end,   "baz");

    // depthが1で、中心がbarの場合, 何も返らない。
    TEST_ASSERT_EQUAL(get_descendants(tbl, "bar", 1, result, 10), 0);

    free_table(tbl);
}

TEST(edge, test_get_descendants_with_loop){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    // 前提: エッジが(foo, bar), (bar, foo)の場合
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "foo", "bar"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "bar", "foo"), 0);

    // depthが0で、中心がfooの場合, 何も返らない。
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 0, result, 10), 0);

    // depthが1で、中心がfooの場合, (foo, bar)が返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 1, result, 10), 1);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");

    // depthが2で、中心がfooの場合, (foo, bar), (bar, foo)が返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 2, result, 10), 2);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).start, "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).end,   "foo");

    // depthが3で、中心がfooの場合, (foo, bar), (bar, foo)が返る
    TEST_ASSERT_EQUAL(get_descendants(tbl, "foo", 3, result, 10), 2);
    TEST_ASSERT_EQUAL_STRING((result[0]).start, "foo");
    TEST_ASSERT_EQUAL_STRING((result[0]).end,   "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).start, "bar");
    TEST_ASSERT_EQUAL_STRING((result[1]).end,   "foo");

    free_table(tbl);
}

TEST(edge, test_tmp){
    HashTable* tbl = init_table(1000);
    Edge result[10];

    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "v", "v"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "v", "v"), 0);
    TEST_ASSERT_EQUAL(add_edge_to_table(tbl, "v", "v"), 0);

    TEST_ASSERT_EQUAL(get_descendants(tbl, "v", 2, result, 10), 1);

    free_table(tbl);
}

// ===============================================================================

TEST(edge, test_get_descendants__edge_is_unique){
    HashTable* tbl = init_table(10000);

    int max_result = 10000;

    Edge result[max_result];
    char buf[BUFSIZ], buf2[BUFSIZ];

    int len;
    long sum;
    long sums[max_result];
    int i, j;

    int max = 10;
    for(i=0;i<max;i++){
        for(j=0;j<max;j++){
            sprintf(buf, "%d", i);
            sprintf(buf2, "%d", j);
            TEST_ASSERT_EQUAL(add_edge_to_table(tbl, buf, buf2), 0);
        }
    }

    len = get_descendants(tbl, "1", 10, result, max_result);
    TEST_ASSERT_TRUE(len > 0);

    for(i=0;i<len;i++){
        inspect_edge(&(result[i]), buf, BUFSIZ);

        sum = 0;
        for(j=0;j<strlen(buf);j++){
            sum += (int) (buf[j] << j*8);
        }

        sums[i] = sum;
    }

    for(i=0;i<len;i++){
        for(j=0;j<len;j++){
            if((i != j) && (sums[i] == sums[j])){
                TEST_FAIL_MESSAGE("duplicated edges found");
            }
        }
    }

    free_table(tbl);
}

// ===============================================================================

TEST_GROUP_RUNNER(edge) {
  RUN_TEST_CASE(edge, test_inspect_edge);

  RUN_TEST_CASE(edge, test_get_key);
  RUN_TEST_CASE(edge, test_init_table);

  RUN_TEST_CASE(edge, test_get_edges_by_start_with_depth_0);
  RUN_TEST_CASE(edge, test_get_edges_by_start_with_one_edge);
  RUN_TEST_CASE(edge, test_get_edges_by_start_with_2_edge);
  RUN_TEST_CASE(edge, test_get_edges_when_start_is_equal_to_end_with_1_edge);
  RUN_TEST_CASE(edge, test_get_edges_when_start_is_equal_to_end_with_2_edge);
  RUN_TEST_CASE(edge, test_get_edges_by_start_with_small_size_table);

  RUN_TEST_CASE(edge, test_get_descendants_with_negative_depth);
  RUN_TEST_CASE(edge, test_get_descendants_with_depth_0);
  RUN_TEST_CASE(edge, test_get_descendants_with_one_edge);
  RUN_TEST_CASE(edge, test_get_descendants_with_2_edge);
  RUN_TEST_CASE(edge, test_get_descendants_with_loop);

  RUN_TEST_CASE(edge, test_is_equal_edge_is_true);
  RUN_TEST_CASE(edge, test_is_equal_edge_is_false);

  RUN_TEST_CASE(edge, test_free_hash_elem__0_edge);
  RUN_TEST_CASE(edge, test_free_hash_elem__1_edge);

  RUN_TEST_CASE(edge, test_search_edges_by_start_and_end__with_0_edges);
  RUN_TEST_CASE(edge, test_search_edges_by_start_and_end__with_1_edges);
  RUN_TEST_CASE(edge, test_search_edges_by_start_and_end__with_2_edges);

  RUN_TEST_CASE(edge, test_get_descendants__edge_is_unique);

  RUN_TEST_CASE(edge, test_tmp);
}

static void runEdgeTests() {
    RUN_TEST_GROUP(edge);
}

int main(int argc, char* argv[]) {
    return UnityMain(argc, argv, runEdgeTests);
}
