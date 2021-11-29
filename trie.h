/*
 * Author: Mitesh Kanjariya
 * Description: Header file for a bit trie.
 * Bit trie is a simple trie data structure which can have only possible values
 * at each node (0 or 1)
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#define NUM_KIDS 2

typedef void
(*trie_walk_f) (void *data);

typedef void
(*trie_data2key_f) (void *data, void **key, int *klen);

typedef struct trie_attr_s {
  trie_walk_f walk_fn;
  trie_data2key_f key_fn;
} trie_attr_t;

typedef struct trie_node_s {
  struct trie_node_s *children[NUM_KIDS];
  void *data;
} trie_node_t;

typedef struct trie_s {
  trie_node_t *root;
  trie_data2key_f key_f;
  trie_walk_f walk_f;
} trie_t;

/*
 * Public APIs
 */

/*
 * API to create a new TRIE
 * @param attr  Attributes of the TRIE to be created
 *
 * @return The newly created TRIE with the passed attributes
 */
extern trie_t *
trie_new (trie_attr_t attr);

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
extern int
trie_insert (trie_t *t, void *data);

/*
 * API to walk the contents of the TRIE.
 * It calls a recursive function which traverses the TRIE
 * in post order and walk the more specific key/mask first
 * followed by less specific key/mask.
 */
extern void
trie_show (trie_t *t);

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
extern trie_node_t *
trie_search (trie_t *t, void *key, int klen, void **data);

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
trie_subtree_walk (trie_t *t, void *key, int klen);

#endif /* __TRIE_H__*/
