
#ifndef ROPE_H_
#define ROPE_H_

#include <stdint.h>

#define DEBUG

#define MAX_NODE_STR_SIZE 64
#define MAX_ROPE_DEPTH 50

#define INITIAL_POOL_SIZE 1048576

typedef struct rope_node_t {
	// The sum of the size of the string at the node and the sum of all
	// weights at the nodes left subtree
	size_t weight;
	uint8_t str[MAX_NODE_STR_SIZE];
	struct rope_node_t *left_child;
	struct rope_node_t *right_child;
} rope_node;

typedef struct rope_t {
	size_t bytes_used;
	size_t depth;
	rope_node *head;
} rope;

rope *make_rope(void);

rope *make_rope_cstr(uint8_t *);

void rope_insert(rope *, size_t, uint8_t *);

void rope_concat(rope *, rope *);

void add_rope_node(rope *, rope_node *);

rope_node *make_rope_node(void);

static inline void balance_rope(rope *);

uint8_t *rope_to_cstr(rope *);
#endif
