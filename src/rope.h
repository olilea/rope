#ifndef ROPE_H
#define ROPE_H

#include <stdint.h>
#include <stddef.h>

typedef struct r_node {
    struct r_node *left;
    struct r_node *right; 
    uint32_t value;
    uint8_t *str;
} r_node;

typedef struct rope {
    r_node *head;
    size_t char_width;
    uint32_t str_length;
} rope;

// The width of each char in bytes
rope *make_rope(const size_t char_width);

int8_t append_to_rope(rope *r, uint8_t *str, const uint32_t str_length);

uint8_t *rope_to_string(rope *r);

//uint8_t *substring_of_rope(rope *r, const uint32_t start, const uint32_t end);

//int8_t remove_from_rope(rope *r, const uint32_t start, const uint32_t end);

#endif
