#include <stdlib.h>
#include <string.h>

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

rope *make_rope(const size_t char_width) {
    rope *r = malloc(sizeof(rope));
    r_node *head = malloc(sizeof(r_node));
    if ((r != NULL) && (head != NULL)) {
        r->head = head;
        r->char_width = char_width;
        r->str_length = 0;
        head->left = NULL;
        head->right = NULL;
        head->value = 0;
        head->str = NULL;
    }
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
        return -1;

    new_head->left = r1->head;
    new_head->right = r2->head;
    new_head->str = NULL;
    new_head->value = value_of_tree(r1->head); 

    r1->str_length = r1->str_length + r2->str_length;
    r1->head = new_head;
    return 0;
}

int8_t append_to_rope(rope *r, uint8_t *str, const uint32_t str_length) {
    r_node *cur = r->head;
    while (cur->right != NULL)
        cur = cur->right;
    if (str_length < MAX_NODE_CONTENTS) {
        // Create new parent node for original node and new node
        r_node *new_parent = malloc(sizeof(r_node));
        r_node *right_node = malloc(sizeof(r_node));
        if ((new_parent == NULL) || (right_node == NULL))
            return -1;
        right_node->left = NULL;
        right_node->right = NULL;
        right_node->value = str_length;
        right_node->str = str;

        new_parent->left = cur;
        new_parent->right = right_node;
        new_parent->value = cur->value;
        new_parent->str = NULL;
    }
    r->str_length += str_length;

    return 0;
}

uint8_t *rope_to_string(rope *r) {
    r_node *cur = r->head;
    uint8_t *output_string = malloc(r->char_width * r->str_length);
    if (output_string == NULL)
        return NULL;
    if (cur->left == NULL) {
        strncpy(output_string, cur->str, r->str_length);
        return output_string;
    } else {
        r_stack *stack = malloc(sizeof(stack));
        uint8_t *output_cur = output_string;
        if (stack == NULL)
            return NULL;
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
                       return NULL;
                    }
                } else {
                    strncpy(output_cur, cur->str, cur->value);
                    output_cur += cur->value;
                }
            }
        }
        free_stack(stack);
        return output_string;
    }
}

