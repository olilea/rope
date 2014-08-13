
#include <stdio.h>
#include <string.h>

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

static void print_uint8(uint8_t *string) {
	int i = 0;
	while (1) {
		if (string[i] == '\0') {
			break;
		} else {
			printf("%c", string[i]);
		}
		++i;
	}
}

int main(int argc, char *argv[]) {
	//print_sizes();

	rope *r = make_rope();
	rope_node *node = make_rope_node();
	rope_node *node2 = make_rope_node();
	rope_node *node3 = make_rope_node();

	strcpy((char *)(node2->str), "LOLCANOE");
	strcpy((char *)(node3->str), "IT WORKS!");
	node2->weight = 9;
	node3->weight = 10;
	add_rope_node(r, node);
	add_rope_node(r, node2);
	add_rope_node(r, node3);

	uint8_t *string = rope_to_cstr(r);
	print_uint8(string);

	return 0;
}
