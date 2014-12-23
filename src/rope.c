#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rope.h"

#define MAX_NODE_CONTENTS 128

typedef struct r_stack {
    struct r_stack *next;
    r_node *element;
} r_stack;

static r_stack *pop_stack(r_stack *stack) {
    if (stack->next == NULL) {
        free(stack);
        return NULL;
    } else {
        r_stack *next_stack = stack->next;
        free(stack);
        return next_stack;
    }
}

static int8_t push_to_stack(r_stack *stack, r_node *element) {
    if (element == NULL)
        return -1;
    r_stack *new_stack = malloc(sizeof(stack));
    if (new_stack == NULL)
        return -1;
    new_stack->next = stack->next;
    new_stack->element = stack->element;
    stack->next = new_stack;
    stack->element = element;
    return 0;
}

static void free_stack(r_stack *stack) {
    if (stack->next == NULL) {
        free(stack);
    } else {
        r_stack *prev = stack;
        r_stack *next_stack = stack->next;
        while (next_stack->next != NULL) {
            free(prev);
            prev = next_stack;
            next_stack = next_stack->next;
        }
        free(next_stack);
    }
}

static inline r_node *create_empty_node() {
    r_node *node = malloc(sizeof(node));
    if (node == NULL)
        return NULL;
    node->left = NULL;
    node->right = NULL;
    node->value = 0;
    node->str = NULL;
    return node;
}

rope *make_rope(const size_t char_width) {
    rope *r = malloc(sizeof(rope));
    r_node *head = create_empty_node();
    if ((r == NULL) || (head == NULL)) {
        free(r);
        free(head);
        return NULL;
    }
    r->head = head;
    r->char_width = char_width;
    r->str_length = 0;
    return r;
}

static uint32_t value_of_tree(r_node *head) {
    uint32_t value = 0;
    if (head->left != NULL) {
        r_stack *stack = malloc(sizeof(r_stack));
        if (stack == NULL)
            return -1;
        stack->element = head->left;
        stack->next = NULL;
        while (stack != NULL) {
           r_node *cur = stack->element;
           stack = pop_stack(stack);
           if ((cur->left == NULL) || (cur->right == NULL))
               value += cur->value;
           else {
               if ((push_to_stack(stack, cur->left) == -1)
                   || push_to_stack(stack, cur->right) == -1) {
                   free_stack(stack);
                   return -1;
               }
           }
        }
        free_stack(stack);
    }
    return value;
}

static int8_t concat_ropes(rope *r1, rope *r2) {
    r_node *new_head = malloc(sizeof(r_node));
    if ((new_head == NULL) || (r1->char_width != r2->char_width))
        free(new_head);
        return -1;

    new_head->left = r1->head;
    new_head->right = r2->head;
    new_head->str = NULL;
    new_head->value = value_of_tree(r1->head); 

    r1->str_length = r1->str_length + r2->str_length;
    r1->head = new_head;
    free(r2);
    return 0;
}

static r_node *split_node_above(r_node *left_node) {
    // Create new parent node for original node and new node
    r_node *new_parent = malloc(sizeof(r_node));
    r_node *right_node = create_empty_node();
    if ((new_parent == NULL) || (right_node == NULL))
        free(new_parent);
        free(right_node);
        return NULL;

    new_parent->left = left_node;
    new_parent->right = right_node;
    new_parent->value = left_node->value;
    new_parent->str = NULL;

    return new_parent;
}

static r_node *create_node_from_string(const uint32_t char_width, uint8_t *str, const uint32_t str_length) {
    if (str_length > MAX_NODE_CONTENTS)
        return NULL;
    r_node *node = malloc(sizeof(r_node));
    if (node == NULL)
        return NULL;
    node->left = NULL;
    node->right = NULL;    
    node->value = str_length;

    uint32_t str_byte_len = char_width * str_length;
    node->str = malloc(str_byte_len);
    if (node->str == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->str, str, str_byte_len);

    return node;
}

static r_node *create_node_structure_from_string(const uint32_t char_width, uint8_t *str, const uint32_t str_length) {
    // If the string is short enough to fit in one node,
    // do it the simple way
    if (str_length <= MAX_NODE_CONTENTS)
        return create_node_from_string(char_width, str, str_length);

    // The number of nodes of the bttom of the node structure,
    // making sure to add one if their is a remainder from the division
    uint32_t bottom_nodes = str_length / MAX_NODE_CONTENTS;
    if ((str_length % MAX_NODE_CONTENTS) != 0)
        bottom_nodes++;

    // The number of layers required to make up the tree
    uint32_t layers = (uint32_t)(ceil(log2((double)bottom_nodes)) + 0.5f);
    r_node *cur = create_empty_node();
    r_node *head = cur;
    if (cur == NULL)
        return NULL;
    layers--;

    r_stack *stack = malloc(sizeof(r_stack));
    if (stack == NULL)
        return NULL;
    stack->element = cur;
    stack->next = NULL;

    uint8_t *str_pos = str;
    int layer = 1;
    while (stack != NULL) {
        cur = stack->element;
        stack = pop_stack(stack);
        if (layer == layers) {
            uint32_t node_value = 1;//str_length - str_pos;
            cur->left = NULL;
            cur->right = NULL;
            cur->value = node_value;
            memcpy(cur->str, str_pos, node_value * char_width);
        } else {
            cur->left = create_empty_node();
            cur->right = create_empty_node();
            if ((cur->left == NULL) || (cur->right) == NULL) {
                free(cur->left);
                free(cur->right);
                goto cleanup;
            }
            if ((push_to_stack(stack, cur->left) == -1)
                    || (push_to_stack(stack, cur->right)) == -1)
                goto cleanup;
        }
    }
cleanup:
    free(cur);
    free_stack(stack);
    // TODO Free tree
}

static rope *rope_from_string(const uint32_t char_width, uint8_t *str, const uint32_t str_length) {
    rope *r = malloc(sizeof(rope));
    if (r == NULL)
        return NULL;
    r->char_width = char_width;
    r->str_length = str_length;
    if (str_length < MAX_NODE_CONTENTS) {
        r_node *head = create_node_from_string(char_width, str, str_length);
        if (head == NULL) {
            free(r);
            return NULL;
        }
        r->head = head;
    } else {
        create_node_structure_from_string(char_width, str, str_length);
    }
    return r;
}

int8_t append_to_rope(rope *r, uint8_t *str, const uint32_t str_length) {
    r_node *cur = r->head;
    if (str_length < MAX_NODE_CONTENTS) {
        // If is a simple insert of a new node
        while (cur->right != NULL)
            cur = cur->right;
        if (cur->value == 0) {
            // If the found node has no string put it there
            uint32_t str_byte_len = str_length * r->char_width;
            cur->str = malloc(str_byte_len);
            if (cur->str == NULL)
                return -1;
            memcpy(cur->str, str, str_byte_len)
            cur->value = str_length;
        } else {
            r_node *new_parent = split_node_above(cur);
            if (new_parent == NULL)
                return -1;
            new_parent->right->value = str_length;
            new_parent->right->str = str;
            if (cur == r->head)
                r->head = new_parent;
        }
        r->str_length += str_length;
    } else {
        rope *r2 = rope_from_string(r->char_width, str, str_length);
        if (r2 == NULL)
            return -1;
        concat_ropes(r, r2);
    }

    return 0;
}

uint8_t *rope_to_string(rope *r) {
    r_node *cur = r->head;
    uint8_t *output_string = malloc(r->char_width * r->str_length);
    if (output_string == NULL)
        return NULL;
    if (cur->left == NULL) {
        memcpy(output_string, cur->str, r->str_length * r->char_width);
        return output_string;
    } else {
        r_stack *stack = malloc(sizeof(stack));
        uint8_t *output_cur = output_string;
        if (stack == NULL)
            goto cleanup;
        else {
            stack->element = cur;
            stack->next = NULL;
            while (stack != NULL) {
                cur = stack->element;
                stack = pop_stack(stack);
                if (cur->left != NULL) {
                    if ((push_to_stack(stack, cur->right) == -1)
                        || push_to_stack(stack, cur->left) == -1) {
                        free_stack(stack);
                        goto cleanup;
                    }
                } else {
                    memcpy(output_cur, cur->str, cur->value * r->char_width);
                    output_cur += cur->value;
                }
            }
        }
        free_stack(stack);
        return output_string;
    }
cleanup:
    free(output_string);
    return NULL;
}

