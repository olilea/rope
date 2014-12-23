
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "rope.h"

static void print_rope(rope *r) {
    uint8_t *output = rope_to_string(r);
    uint8_t *cur = output;
    while (*cur != '\0') {
        putchar(*cur);
    }
}

int main(int argc, char *argv[]) {
    rope *r = make_rope(sizeof(char));

    uint8_t *data = malloc(sizeof(uint8_t) * 5);
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[5] = 5;

    // Error in append to rope
    append_to_rope(r, data, 5);

    print_rope(r);
    return 0;
}
