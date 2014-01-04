#ifndef __SERVER_H_INCLUDED_
#define __SERVER_H_INCLUDED_

size_t get_pairs_from_query_string(char *query, char *key, char *output, size_t max_len);
size_t get_substring_by_start_and_end(char *s, char start, char end, char *result, size_t max_len);

#endif
