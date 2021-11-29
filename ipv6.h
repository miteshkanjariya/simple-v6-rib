/*
 * Author: Mitesh Kanjariya
 * Description: header file for ipv6 rib infrastructure
 */
#ifndef __IPV6_H__
#define __IPV6_H__

#include <vector>
#include <netinet/in.h>

#include "trie.h"

using namespace std;

typedef struct route_s {
  struct in6_addr v6;
  int mask_len;
  int metric;
} route_t;

typedef struct ipv6_global_s {
  vector<route_t> routes;
  trie_t *trie;
} ipv6_global_t;

#endif /* __IPV6_H__ */
