/*
 * Author: Mitesh Kanjariya
 * Description: Main file for parsing and storing ipv6 address in
 * a in-memory database.
 */
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <stdint.h>

#include "debug.h"
#include "ipv6.h"

using namespace std;
using namespace boost;

#define ADDR_SZ 256
#define INPUT_FILE "input/ipv6.txt"

/*
 * This function takes the input from the files, parses the data and stores them
 * in a vector of route_t; each line of the file  is expected to be as follows:
 * "ipv6-address/mask-len, <distance>". If the parser cannot parse the line, it
 * will skip that entry.
 *
 * @param fname The name of the file
 * @param v     vector of route_t to be filled
 *
 * @return error-code, 0 on success
 */
static int
parser (const char *fname,
        vector<route_t> &v)
{
  if (!fname)
    return -1;

  ifstream infile(fname);
  string line;

  char_separator<char> sep(",/");

  /*
   * parse the input file line by line.
   * Skip the line if data is corrupt
   */
  while (getline(infile, line)) {

    route_t r;
    tokenizer<char_separator<char>> tokens(line, sep);
    auto it = tokens.begin();

    /* parse the v6 address */
    if (inet_pton(AF_INET6, (*it++).c_str(), &r.v6) != 1) {
      continue;
    }

    /* parse the mask length */
    try {
      r.mask_len = stoi(*it++);
    } catch (const invalid_argument &ia) {
      continue;
    }

    /* parse the distance */
    try {
      r.metric = stoi(*it++);
    } catch (invalid_argument &ia) {
      continue;
    }

    v.push_back(r);
  }

  return 0;
}

static void
ipv6_walk_func (void *data)
{
  char buf[256] = {0};
  route_t *r = (route_t *) data;

  if (!r)
    return;

  inet_ntop(AF_INET6, &r->v6, buf, sizeof(buf));
  SHOW("%s %d -----> %d", buf, r->mask_len, r->metric);
  return;
}

static void
ipv6_route2key (void *data, void **key, int *klen)
{
  route_t *d = (route_t *) data;

  if (!data || !key || !klen)
    return;

  *key = &d->v6;
  *klen = d->mask_len;
}

int
main ()
{
  ipv6_global_t ipv6_g;
  trie_attr_t attr;

  memset(&ipv6_g, 0, sizeof(ipv6_global_t));

  /* parse the input file */
  if (parser(INPUT_FILE, ipv6_g.routes)) {
    ERROR("Failed to parse the input");
    return -1;
  }

  /* init the trie */
  attr.key_fn = ipv6_route2key;
  attr.walk_fn = ipv6_walk_func;
  ipv6_g.trie = trie_new(attr);
  if (!ipv6_g.trie) {
    ERROR("ERROR: failed to init v6 trie");
    return -1;
  }

  /* populate the trie with all the routes,
   * only add the route if it has a beter preference than the already inserted
   * route */
  for (int i = 0; i < ipv6_g.routes.size(); i++) {
    char buf[ADDR_SZ] = {0};

    inet_ntop(AF_INET6, &ipv6_g.routes[i].v6, buf, sizeof(buf));
    DEBUG("inserting route: %s", buf);

    route_t *r;
    trie_node_t *n = trie_search(ipv6_g.trie, &ipv6_g.routes[i],
                                 ipv6_g.routes[i].mask_len,
                                 (void **) &r);
    if (n) {
      DEBUG("found route: %s with metric: %d", buf, r->metric);
    }
    if (!n || (n &&
        r->metric > ipv6_g.routes[i].metric)) {
      if (trie_insert(ipv6_g.trie, (void *)&ipv6_g.routes[i])) {
        ERROR("Failed to add route: %s with metric: %d", buf, ipv6_g.routes[i].metric);
        continue;
      }
    } else {
      DEBUG("skipping route: %s with metric: %d", buf, ipv6_g.routes[i].metric);
    }
  }

  /* display all the routes */
  SHOW("\nRIB:");
  trie_show(ipv6_g.trie);

  /* search all routes for an entry */
  SHOW("\n\nexample of a subtree walk  \n\n");
  trie_subtree_walk(ipv6_g.trie, &ipv6_g.routes[0], ipv6_g.routes[0].mask_len);

  return 0;
}
