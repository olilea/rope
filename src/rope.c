
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

	// THIS NEEDS FIXING

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
	r->depth++;

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

typedef struct {
	rope_node *stk[MAX_ROPE_DEPTH];
	rope_node **top;
} node_stack;

// Depth first iteration over the rope
uint8_t *rope_to_cstr(rope *r) {

	if (r->depth == 0) {
		return NULL;
	}

	size_t nodes_expanded = 0;
	size_t total_nodes = r->bytes_used / SIZEOF_ROPE_NODE;

	uint8_t *cstr = (uint8_t *) malloc(total_nodes * MAX_NODE_STR_SIZE);
	uint8_t *next_free = cstr;

	node_stack stack;
	stack.top = stack.stk;
	stack.stk[0] = r->head;

	uint8_t children = 0;

	rope_node *popped_node;

	while (nodes_expanded < total_nodes) {

		#ifdef DEBUG
		printf("LEFT_CHILD: %p\n", (*stack.top)->left_child);
		printf("RIGHT_CHILD: %p\n", (*stack.top)->right_child);
		#endif

		popped_node = *(stack.top);

		if (popped_node->left_child != NULL) {
			children++;
			*stack.top = popped_node->left_child;
		}

		if (popped_node->right_child != NULL) {
			if (children == 1) {
				stack.top++;
			}
			*stack.top = popped_node->right_child;
		}

		// Expand the node on the top of the stack
		if (children == 0) {
			// Determine if the string contains '\0'
			int i = 0;
			for (; i < MAX_NODE_STR_SIZE; ++i) {
				if (popped_node->str[i] == '\0') {
					break;
				}
			}
			memcpy(next_free, popped_node->str, i);
			next_free += i;
			stack.top--;
		}

		nodes_expanded++;
		children = 0;
	}

	return cstr;

}
