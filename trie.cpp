/*
 * Author: Mitesh Kanjariya
 * @description: This file contains an implementation of a bitwise TRIE data
 * structure. It provides API to create/insert/search and subtree walk API.
 */
#include <bits/stdc++.h>
#include <assert.h>
#include <stdint.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "trie.h"

using namespace std;

/*
 * utility function to check if the bit at position i is set in a key
 */
static bool
get_bit (void *key, int klen, int i)
{
  uint8_t *a = (uint8_t *)key;
  unsigned int offset = i - (i & ~(0x07));

  return (a[(klen - i -1) >> 3] & (1 << offset));
}

/*
 * utility function to get the bit representation of a key
 */
static void
key2bits (void *key, int klen)
{
  uint8_t *a = (uint8_t *)key;
  for (int i = 0; i < 16; i++) {
    printf("0x%x ", a[i]);
  }
  cout << endl;

  for (int i = klen - 1; i >= 0; i--) {
    cout << get_bit(key, klen, i) << " ";
  }
  cout << endl;
}

/*
 * utility functiob to allocate memory for a new trie node
 */
static trie_node_t *
trie_node_new (void *key, int klen)
{
  trie_node_t *n;

  n = new trie_node_t;
  assert(n && "Failed to alocate data\n");

  n->children[0] = NULL;
  n->children[1] = NULL;
  n->data = NULL;
  return n;
}

/*
 * API to create a new TRIE
 * @param attr  Attributes of the TRIE to be created
 *
 * @return The newly created TRIE with the passed attributes
 */
trie_t *
trie_new (trie_attr_t attr)
{
  trie_t *t;

  t = new trie_t;
  assert(t && "Failed to allocate trie\n");
  t->walk_f = attr.walk_fn;
  t->key_f = attr.key_fn;

  t->root = new trie_node_t;
  assert(t->root && "Failed to allocate trie root\n");

  t->root->children[0] = NULL;
  t->root->children[1] = NULL;
  t->root->data = NULL;
  return t;
}

/*
 * API to insert an user defined data to the given TRIE.
 * The user needs to define a data2key function which gives
 * the key for the passed data.
 *
 * @param t     The TRIE where the data needs to be inserted
 * @param data  The data to be inserted in the TRIE
 *
 * @return 0 on success; -1 otherwise
 */
int
trie_insert (trie_t *t, void *data)
{
  void *key;
  int klen;
  trie_node_t *n;

  if (!t || !t->key_f) {
    cout << "invalid params" << endl;
    return -1;
  }

  t->key_f(data, &key, &klen);
  if (!key || !klen) {
    cout << "could not get key" << endl;
    return -1;
  }

  n = t->root;
  for (int i = klen - 1; i >= 0; i--) {
    bool bit = get_bit(key, klen, i);
    if (!n->children[bit]) {
      n->children[bit] = trie_node_new(key, klen - i);
    }

    n = n->children[bit];
  }
  n->data = data;
  return 0;
}

/*
 * Recursive function to walk the contents of the TRIE
 * It uses the post order traversal to make sure more specific key/mask
 * are walked first followed by less specific key/mask.
 */
static void
walk_ (trie_t *t, trie_node_t *n)
{
  void *key;
  int klen;

  for (int i = 0; i < NUM_KIDS; i++) {
    if (n->children[i]) {
      walk_(t, n->children[i]);
    }
  }
  if (n->data && t->walk_f) {
    t->walk_f(n->data);
  }
}

/*
 * API to walk the contents of the TRIE.
 * It calls a recursive function which traverses the TRIE
 * in post order and walk the more specific key/mask first
 * followed by less specific key/mask.
 */
void
trie_show (trie_t *t)
{
  trie_node_t *n;

  walk_(t, t->root);
}

/*
 * API to search a key with the given length from the TRIE.
 *
 * @param t    The TRIE to be searched
 * @param key  The key to be searched
 * @param klen The length of the key to be searched
 * @param data The data to be filled
 *
 * @return true if the data is found in the TRIE; false otherwise
 */
trie_node_t *
trie_search (trie_t *t, void *key, int klen, void **data)
{
  trie_node_t *n;

  if (!t)
    return NULL;

  if (data)
    *data = NULL;
  n = t->root;
  for (int i = klen - 1; i >= 0; i--) {
    bool bit = get_bit(key, klen, i);
    if (!n->children[bit])
      return NULL;
    n = n->children[bit];
  }

  if (data)
    *data = n->data;
  return (n->data ? n : NULL);
}

/*
 * API to do a subtree walk.
 * Given the key/mask, this API will walk all the
 * nodes of the TRIE which are greater than or equal to the given key/mask.
 *
 * @param t    The TRIE
 * @param key  The key to be searched
 * @param klen The length of the key
 *
 * @return void
 */
void
trie_subtree_walk (trie_t *t, void *key, int klen)
{
  trie_node_t *n;

  if (!t)
    return;

  n = trie_search(t, key, klen, NULL);
  if (!n)
    return;

  walk_(t, n);
}
