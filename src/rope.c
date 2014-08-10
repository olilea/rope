
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "rope.h"

static const size_t SIZEOF_ROPE_NODE = sizeof(rope_node);

static rope_node *next_node_location;

rope *make_rope(void) {
	
	rope *r = (rope *) malloc(sizeof(rope));

	r->bytes_used = 0;
	r->depth = 0;
	r->head = NULL;
	next_node_location = (rope_node *)malloc(INITIAL_POOL_SIZE);

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

void add_rope_node(rope *r, rope_node *node) {

	assert(node != NULL);

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

rope_node *make_rope_node(void) {

	rope_node *node = next_node_location++;
	node->weight = 0;
	node->left_child = NULL;
	node->right_child = NULL;

	return node;
}

static inline void balance_rope(rope *r) {
	
}

static inline void print_string(rope_node *node) {

	if (node->left_child == NULL && node->right_child == NULL) {
		printf("%d", *node->str);
	} else {
		if (node->left_child != NULL) {
			print_string(node->left_child);
		}
		if (node->right_child != NULL) {
			print_string(node->right_child);
		}
	}
}

// Depth first iteration over the rope
void print_rope(rope *r) {

	if (r->depth == 0) {
		return;
	}

	print_string(r->head);

}
