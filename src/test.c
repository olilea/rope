
#include <stdio.h>

#include "rope.h"

static void print_rope_node_size(void) {
	printf("\nrope_node_size: %lu\n", sizeof(rope_node));

	printf("weight: %lu\nstr: %lu\nleft_child: %lu\nright_child: %lu\n",
		sizeof(size_t), sizeof(uint8_t) * MAX_NODE_STR_SIZE,
		sizeof(rope_node *), sizeof(rope_node *));
}

static void print_sizes(void) {
	print_rope_node_size();
}

int main(int argc, char *argv[]) {
	print_sizes();

	return 0;
}
