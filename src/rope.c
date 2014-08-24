
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "rope.h"

static const size_t SIZEOF_ROPE_NODE = sizeof(rope_node);

static rope_node *next_node_location;

rope *make_rope(void) {
	
	rope *r = (rope *) malloc(sizeof(rope));

	r->bytes_used = 0;
	r->head = NULL;
	next_node_location = (rope_node *)malloc(INITIAL_POOL_SIZE);

	return r;
}

rope *make_rope_cstr(uint8_t *cstr) {

	rope *r = make_rope();
	rope_insert(r, 0, cstr);

	return r;
}

char rope_index(rope *r, size_t index) {
	rope_node *iter = r->head;
	size_t weight_counter = index;

	while (1) {
		if (weight_counter <= iter->weight) {
			if (iter->left_child == NULL && iter->right_child == NULL) {
				fflush(stdout);
				return ((iter->str)[weight_counter]);
			}
			iter = iter->left_child;
		} else {
			fflush(stdout);
			weight_counter -= iter->weight;
			iter = iter->right_child;
		}
	}
}

void rope_insert(rope *r, size_t pos, uint8_t *cstr) {

}

void rope_concat(rope *r1, rope *r2) {

}

enum direction_e {
	LEFT,
	RIGHT
};

void add_rope_node(rope *r, rope_node *node) {

	assert(node != NULL);

	rope_node *iter;

	// If the rope has no head
	if (r->bytes_used == 0) {
		r->head = node;
	} else {
		bool complete = false;
		iter = r->head;
		while (!complete) {
			// If the current node is a leaf
			if (iter->left_child == NULL && iter->right_child == NULL) {
				// If the node to insert's weight is less than the iter's weight, insert
				// the node to insert to the left of this node, and copy the iter node to the right
				// of iter. Else, node to the right, iter to the left
				if (node->weight <= iter->weight) {
					iter->left_child = node;
					iter->right_child = make_rope_node_w(iter->weight);
					memcpy(iter->right_child->str, iter->str, MAX_NODE_STR_SIZE);
					iter->weight = node->weight;
				} else {
					iter->left_child = make_rope_node_w(iter->weight);
					memcpy(iter->left_child->str, iter->str, MAX_NODE_STR_SIZE);
					iter->right_child = node;
				}
				r->bytes_used += SIZEOF_ROPE_NODE;
				complete = true;
			} else if (node->weight <= iter->weight) {
				// If the node to insert's weight is less than the current node's weight
				// add the correct amount to the current node's weight
				iter->weight += node->weight;
				if (iter->left_child == NULL) {
					// If the current node's left child is NULL, insert the node here
					// Else on the right
					iter->left_child = node;
					complete = true;
				} else {
					iter = iter->left_child;
				}
			} else {
				// If the node to insert's weight is greater than the current node's weight...
				if (iter->right_child == NULL) {
					// If there is space on the current node's right, insert the node here
					iter->right_child = node;
					complete = true;
				} else {
					// Else, move the current node to the current node's right child and repeat
					iter = iter->right_child;
				}
			}
		}
	}
	r->bytes_used += SIZEOF_ROPE_NODE;
}

rope_node *make_rope_node(void) {

	rope_node *node = next_node_location++;
	node->weight = 0;
	node->left_child = NULL;
	node->right_child = NULL;

	return node;
}

static inline rope_node *make_rope_node_w(int mass) {
	rope_node *node = next_node_location++;
	node->weight = mass;
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

	if (r->bytes_used == 0) {
		return NULL;
	}

	size_t nodes_expanded = 0;
	size_t total_nodes = r->bytes_used / SIZEOF_ROPE_NODE;

	uint8_t *cstr = (uint8_t *) malloc(total_nodes * MAX_NODE_STR_SIZE);
	uint8_t *next_free = cstr;

	node_stack stack;
	stack.top = stack.stk;
	stack.stk[0] = r->head;

	bool children = false;

	rope_node *popped_node;

	while (nodes_expanded < total_nodes) {
		#ifdef DEBUG
		printf("LEFT_CHILD: %p\n", (*stack.top)->left_child);
		printf("RIGHT_CHILD: %p\n", (*stack.top)->right_child);
		#endif

		popped_node = *(stack.top);

		if (popped_node->right_child != NULL) {
			children = true;
			*stack.top = popped_node->right_child;
		}

		if (popped_node->left_child != NULL) {
			if (children) {
				stack.top++;
			}
			children = true;
			*stack.top = popped_node->left_child;
		}

		// Expand the node on the top of the stack
		if (!children) {
			// Determine if the string contains '\0'
			// Prevents copying the whole string if the entire space in the node is not used
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
		children = false;

	}
	return cstr;
}
