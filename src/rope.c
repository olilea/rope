
#include <stdlib.h>

#include "rope.h"

static const size_t SIZEOF_ROPE_NODE = sizeof(rope_node);

static rope_node *next_node_location;

rope *make_rope(void) {
	
	rope *r = (rope *) malloc(INITIAL_POOL_SIZE);
	uint8_t *rope_location = (uint8_t *) r;

	r->bytes_used = sizeof(rope);
	r->depth = 0;
	r->head = NULL;
	next_node_location = (rope_node *) (rope_location + r->bytes_used);

	return r;
}

rope *make_rope_cstr(uint8_t *cstr) {

	rope *r = make_rope();
	rope_insert(r, 0, cstr);

	return r;
}

void rope_insert(rope *r, size_t pos, uint8_t *cstr) {

}

void rope_concat(rope *r1, rope *r2) {

}

typedef enum {
	LEFT,
	RIGHT
} node_child;

static inline void add_rope_node(rope *r, rope_node *node) {

	rope_node_iter iter;
	node_child insert_dir;

	if (r->depth == 0) {
		r->head = node;
		r->depth++;
	} else {

		iter.current = NULL;
		iter.next = r->head;

		while (iter.next != NULL) {
			iter.current = iter.next;
			if (node->weight <= iter.current->weight) {
				iter.current->weight += node->weight;
				iter.next = iter.current->left_child;
				insert_dir = LEFT;
			} else {
				iter.next = iter.current->right_child;
				insert_dir = RIGHT;
			}
		}
		if (insert_dir == LEFT) {
			iter.current->left_child = node;
		} else {
			iter.current->right_child = node;
		}
	}

	r->bytes_used += SIZEOF_ROPE_NODE;

	// Need to check to make sure that the tree is correctly balanced
	// and that there is enough space to make another insertion into the tree
	// from the pool
}

static inline rope_node *make_rope_node(void) {

	rope_node *node = next_node_location++;
	node->weight = 0;
	node->left_child = NULL;
	node->right_child = NULL;

	return node;
}

static void balance_rope(rope *r) {
	
}
