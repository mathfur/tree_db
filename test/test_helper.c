#include "unity_fixture.h"
#include "helper.c"

TEST_GROUP(helper);
TEST_SETUP(helper) { }
TEST_TEAR_DOWN(helper) { } 

// ===============================================================================

TEST(helper, test_get_value_from_query_string__with_1_pair){
    char result[BUFSIZ];

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=bar", "foo", result, BUFSIZ), 3);
    TEST_ASSERT_EQUAL_STRING(result, "bar");

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=bar", "baz", result, BUFSIZ), 0);
    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=bar", "bar", result, BUFSIZ), 0);
    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=bar", "fo", result, BUFSIZ), 0);

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=bar", "oo", result, BUFSIZ), 0);
    TEST_ASSERT_EQUAL_STRING(result, "bar");
}

TEST(helper, test_get_value_from_query_string__with_2_pairs){
    char result[BUFSIZ];

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cd", "foo", result, BUFSIZ), 2);
    TEST_ASSERT_EQUAL_STRING(result, "ab");

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cde", "bar", result, BUFSIZ), 3);
    TEST_ASSERT_EQUAL_STRING(result, "cde");

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cd", "baz", result, BUFSIZ), 0);
}

TEST(helper, test_get_value_from_query_string__with_3_pairs){
    char result[BUFSIZ];

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cde&baz=fghi", "foo", result, BUFSIZ), 2);
    TEST_ASSERT_EQUAL_STRING(result, "ab");

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cde&baz=fghi", "bar", result, BUFSIZ), 3);
    TEST_ASSERT_EQUAL_STRING(result, "cde");

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cde&baz=fghi", "baz", result, BUFSIZ), 4);
    TEST_ASSERT_EQUAL_STRING(result, "fghi");

    TEST_ASSERT_EQUAL(get_pairs_from_query_string("foo=ab&bar=cde&baz=fghi", "hoge", result, BUFSIZ), 0);
}

// ===============================================================================

TEST(helper, test_get_substring_by_start_and_end){
    char buf[BUFSIZ];

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("GET /foo?x=10&y=20 HTTP/1.1", '/', ' ', buf, 20), 13);
    TEST_ASSERT_EQUAL_STRING(buf, "foo?x=10&y=20");

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("GET /foo?x=10&y=20 HTTP/1.1", '/', '&', buf, 20), 8);
    TEST_ASSERT_EQUAL_STRING(buf, "foo?x=10");

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("GET /foo?x=10&y=20 HTTP/1.1", 'G', 'T', buf, 20), 1);
    TEST_ASSERT_EQUAL_STRING(buf, "E");

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("GET /foo?x=10&y=20 HTTP/1.1", '?', ' ', buf, 20), 9);
    TEST_ASSERT_EQUAL_STRING(buf, "x=10&y=20");

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("GET /foo?x=10&y=20 HTTP/1.1", '?', 'y', buf, 20), 5);
    TEST_ASSERT_EQUAL_STRING(buf, "x=10&");

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("GET /foo?x=10&y=20 HTTP/1.1", '?', '&', buf, 20), 4);
    TEST_ASSERT_EQUAL_STRING(buf, "x=10");

    TEST_ASSERT_EQUAL(get_substring_by_start_and_end("", '?', '&', buf, 20), 0);
    TEST_ASSERT_EQUAL_STRING(buf, "");
}

// ===============================================================================

TEST_GROUP_RUNNER(helper) {
  RUN_TEST_CASE(helper, test_get_value_from_query_string__with_1_pair);
  RUN_TEST_CASE(helper, test_get_value_from_query_string__with_2_pairs);
  RUN_TEST_CASE(helper, test_get_value_from_query_string__with_3_pairs);

  RUN_TEST_CASE(helper, test_get_substring_by_start_and_end);
}

static void runHelperTests() {
    RUN_TEST_GROUP(helper);
}

int main(int argc, char* argv[]) {
    return UnityMain(argc, argv, runHelperTests);
}
