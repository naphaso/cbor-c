#include <stdio.h>
#include "cbor.h"

int main(int argc, char **argv) {
    unsigned char buffer[4096];
    unsigned int size = 4096;
    unsigned char *data = buffer;

    data = cbor_write_array(data, size, 5);
    data = cbor_write_int(data, size, 123);
    data = cbor_write_string(data, size, "hello");
    data = cbor_write_string(data, size, "world");
    data = cbor_write_long(data, size, 12312312311ULL);
    data = cbor_write_string(data, size, ":D");

    unsigned int offset = 0;
    struct cbor_token token;
    while(1) {
        offset = cbor_read_token(buffer, data - buffer, offset, &token);

        if(token.type == CBOR_TOKEN_TYPE_INCOMPLETE) {
            printf("INCOMPLETE\n");
            break;
        }

        if(token.type == CBOR_TOKEN_TYPE_ERROR) {
            printf("ERROR: %s\n", token.error_value);
            break;
        }

        if(token.type == CBOR_TOKEN_TYPE_INT) {
            printf("int %s%u\n", token.sign < 0 ? "-" : "", token.int_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_LONG) {
            printf("long %s%llu\n", token.sign < 0 ? "-" : "", token.long_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_ARRAY) {
            printf("array %u\n", token.int_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_MAP) {
            printf("map %u\n", token.int_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_TAG) {
            printf("tag %u\n", token.int_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_SPECIAL) {
            printf("special %u\n", token.int_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_STRING) {
            printf("string '%.*s'\n", token.int_value, token.string_value);
            continue;
        }

        if(token.type == CBOR_TOKEN_TYPE_BYTES) {
            printf("bytes with size %u\n", token.int_value);
            continue;
        }
    }

    return 0;
}