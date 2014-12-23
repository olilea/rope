
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "rope.h"

int main(int argc, char *argv[]) {
    rope *r = make_rope(sizeof(char));
    uint8_t *data = malloc(sizeof(uint8_t) * 5);
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[5] = 5;
    append_to_rope(r, data, 5);
    rope_to_string(r);


	return 0;
}
