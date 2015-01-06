#include <string.h>
#include <stdio.h>
#include "cbor.h"

unsigned int cbor_read_token(unsigned char *data, unsigned int size, unsigned int offset, struct cbor_token *token) {
    if(offset >= size) {
        token->type = CBOR_TOKEN_TYPE_INCOMPLETE;
        return offset;
    }

    unsigned int current_offset = offset;
    unsigned char type = data[current_offset++];
    unsigned char majorType = type >> 5;
    unsigned char minorType = type & 31;
    unsigned int length = 0;

    unsigned int remaining = size - current_offset;

    //printf("size %u, offset %u, remaining %u, major type %d, minor type %d\n", size, offset, remaining, majorType, minorType);

    switch(majorType) {
        case 0: // positive integer
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_INT;
                token->int_value = minorType;
                token->sign = 1;
                return current_offset;
            } else if(minorType == 24) { // 1 byte
                token->type = CBOR_TOKEN_TYPE_INT;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_INT;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_INT;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_LONG;
                length = 8;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid positive integer length";
                return offset;
            }
            break;
        case 1: // negative integer
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_INT;
                token->int_value = minorType;
                token->sign = -1;
                return current_offset;
            } else if(minorType == 24) { // 1 byte
                token->type = CBOR_TOKEN_TYPE_INT;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_INT;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_INT;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_LONG;
                length = 8;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid negative integer length";
                return offset;
            }
            break;
        case 2: // bytes
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_BYTES;
                token->int_value = minorType;
            } else if(minorType == 24) {
                token->type = CBOR_TOKEN_TYPE_BYTES;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_BYTES;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_BYTES;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "bytes size too long";
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid bytes size";
                return offset;
            }
            break;
        case 3: // string
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_STRING;
                token->int_value = minorType;
            } else if(minorType == 24) {
                token->type = CBOR_TOKEN_TYPE_STRING;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_STRING;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_STRING;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "string too long";
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid string length";
                return offset;
            }
            break;
        case 4: // array
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                token->int_value = minorType;
                return current_offset;
            } else if(minorType == 24) {
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "array too long";
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid array length";
                return offset;
            }
            break;
        case 5: // map
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_MAP;
                token->int_value = minorType;
                return current_offset;
            } else if(minorType == 24) {
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_ARRAY;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "map too long";
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid map length";
                return offset;
            }
            break;
        case 6: // tag
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_TAG;
                token->int_value = minorType;
                return current_offset;
            } else if(minorType == 24) {
                token->type = CBOR_TOKEN_TYPE_TAG;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_TAG;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_TAG;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "64 bit tags not supported";
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid tag";
                return offset;
            }
            break;
        case 7: // special
            if(minorType < 24) {
                token->type = CBOR_TOKEN_TYPE_SPECIAL;
                token->int_value = minorType;
                return current_offset;
            } else if(minorType == 24) {
                token->type = CBOR_TOKEN_TYPE_SPECIAL;
                length = 1;
            } else if(minorType == 25) { // 2 byte
                token->type = CBOR_TOKEN_TYPE_SPECIAL;
                length = 2;
            } else if(minorType == 26) { // 4 byte
                token->type = CBOR_TOKEN_TYPE_SPECIAL;
                length = 4;
            } else if(minorType == 27) { // 8 byte
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "64 bit specials not supported";
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_ERROR;
                token->error_value = "invalid special value";
                return offset;
            }
            break;
        default:
            token->type = CBOR_TOKEN_TYPE_ERROR;
            token->error_value = "unknown error";
            return offset;
    }

    if(length > 0) {
        if(remaining < length) {
            token->type = CBOR_TOKEN_TYPE_INCOMPLETE;
            return offset;
        }

        switch(length) {
            case 1:
                token->int_value = data[current_offset];
                break;
            case 2:
                token->int_value = ((unsigned short)data[current_offset] << 8) | ((unsigned short)data[current_offset + 1]);
                break;
            case 4:
                token->int_value = ((unsigned int)data[current_offset] << 24) | ((unsigned int)data[current_offset + 1] << 16) | ((unsigned int)data[current_offset + 2] << 8) | ((unsigned int)data[current_offset + 3]);
                break;
            case 8:
                token->long_value = ((unsigned long long)data[current_offset] << 56) | ((unsigned long long)data[current_offset +1] << 48) | ((unsigned long long)data[current_offset +2] << 40) | ((unsigned long long)data[current_offset +3] << 32) | ((unsigned long long)data[current_offset +4] << 24) | ((unsigned long long)data[current_offset +5] << 16) | ((unsigned long long)data[current_offset +6] << 8) | ((unsigned long long)data[current_offset +7]);
                if(majorType == 0) {
                    token->sign = 1;
                } else if(majorType == 1) {
                    token->sign = -1;
                }
                return current_offset + 8;
        }

        current_offset += length;
        remaining = size - current_offset;
    }

    switch(majorType) {
        case 0: // positive integer
            token->sign = 1;
            return current_offset;
        case 1: // negative integer
            token->sign = -1;
            return current_offset;
        case 2: // bytes
            if(remaining < token->int_value) {
                token->type = CBOR_TOKEN_TYPE_INCOMPLETE;
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_BYTES;
                token->bytes_value = (data + current_offset);
                return current_offset + token->int_value;
            }
        case 3: // string
            if(remaining < token->int_value) {
                token->type = CBOR_TOKEN_TYPE_INCOMPLETE;
                return offset;
            } else {
                token->type = CBOR_TOKEN_TYPE_STRING;
                token->string_value = (char *) (data + current_offset);
                return current_offset + token->int_value;
            }
        case 4: // array
        case 5: // map
        case 6: // tag
        case 7: // special
            return current_offset;
        default:
            token->type = CBOR_TOKEN_TYPE_ERROR;
            token->error_value = "unknown error";
            return offset;
    }
}


unsigned char *cbor_write_type_size(unsigned char *data, unsigned int size, unsigned int type, unsigned int type_size) {
    type <<= 5;
    if(type_size < 24) {
        if(size < 1) {
            return data;
        }

        *data++ = (unsigned char) (type | type_size);

        return data;
    } else if(type_size < 256) {
        if(size < 2) {
            return data;
        }

        *data++ = (unsigned char) (type | 24);
        *data++ = (unsigned char) type_size;

        return data;
    } else if(type_size < 65536) {
        if(size < 3) {
            return data;
        }

        *data++ = (unsigned char) (type | 25);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        return data;
    } else {
        if(size < 5) {
            return data;
        }

        *data++ = (unsigned char) (type | 26);
        *data++ = (unsigned char) (type_size >> 24);
        *data++ = (unsigned char) (type_size >> 16);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        return data;
    }
}

unsigned char *cbor_write_type_long_size(unsigned char *data, unsigned int size, unsigned int type, unsigned long long type_size) {
    type <<= 5;
    if(type_size < 24ULL) {
        if(size < 1) {
            return data;
        }

        *data++ = (unsigned char) (type | type_size);

        return data;
    } else if(type_size < 256ULL) {
        if(size < 2) {
            return data;
        }

        *data++     = (unsigned char) (type | 24);
        *data++ = (unsigned char) type_size;

        return data + 2;
    } else if(type_size < 65536ULL) {
        if(size < 3) {
            return data;
        }

        *data++ = (unsigned char) (type | 25);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        return data;
    } else if(type_size < 4294967296ULL) {
        if(size < 5) {
            return data;
        }

        *data++ = (unsigned char) (type | 26);
        *data++ = (unsigned char) (type_size >> 24);
        *data++ = (unsigned char) (type_size >> 16);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        return data;
    } else {
        if(size < 9) {
            return data;
        }

        *data++ = (unsigned char) (type | 27);
        *data++ = (unsigned char) (type_size >> 56);
        *data++ = (unsigned char) (type_size >> 48);
        *data++ = (unsigned char) (type_size >> 40);
        *data++ = (unsigned char) (type_size >> 32);
        *data++ = (unsigned char) (type_size >> 24);
        *data++ = (unsigned char) (type_size >> 16);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        return data;
    }
}

unsigned char *cbor_write_type_size_bytes(unsigned char *data, unsigned int size, unsigned int type, unsigned int type_size, const unsigned char *bytes) {
    type <<= 5;
    if(type_size < 24) {
        if(size < 1 + type_size) {
            return data;
        }

        *data++ = (unsigned char) (type | type_size);

        memcpy(data, bytes, type_size);

        return data + type_size;
    } else if(type_size < 256) {
        if(size < 2 + type_size) {
            return data;
        }

        *data++ = (unsigned char) (type | 24);
        *data++ = (unsigned char) type_size;

        memcpy(data, bytes, type_size);

        return data + type_size;
    } else if(type_size < 65536) {
        if(size < 3 + type_size) {
            return data;
        }

        *data++  = (unsigned char) (type | 25);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        memcpy(data, bytes, type_size);

        return data + type_size;
    } else {
        if(size < 5 + type_size) {
            return data;
        }

        *data++ = (unsigned char) (type | 26);
        *data++ = (unsigned char) (type_size >> 24);
        *data++ = (unsigned char) (type_size >> 16);
        *data++ = (unsigned char) (type_size >> 8);
        *data++ = (unsigned char) type_size;

        memcpy(data, bytes, type_size);

        return data + type_size;
    }


}

unsigned char *cbor_write_pint(unsigned char *data, unsigned int size, unsigned int value) {
    return cbor_write_type_size(data, size, 0, value);
}

unsigned char *cbor_write_nint(unsigned char *data, unsigned int size, unsigned int value) {
    return cbor_write_type_size(data, size, 1, value);
}

unsigned char *cbor_write_plong(unsigned char *data, unsigned int size, unsigned long long value) {
    return cbor_write_type_long_size(data, size, 0, value);
}

unsigned char *cbor_write_nlong(unsigned char *data, unsigned int size, unsigned long long value) {
    return cbor_write_type_long_size(data, size, 1, value);
}

unsigned char *cbor_write_uint(unsigned char *data, unsigned int size, unsigned int value) {
    return cbor_write_pint(data, size, value);
}

unsigned char *cbor_write_ulong(unsigned char *data, unsigned int size, unsigned long long value) {
    return cbor_write_plong(data, size, value);
}

unsigned char *cbor_write_int(unsigned char *data, unsigned int size, int value) {
    if(value < 0) {
        return cbor_write_nint(data, size, (unsigned int)(-value));
    } else {
        return cbor_write_pint(data, size, (unsigned int)(value));
    }
}

unsigned char *cbor_write_long(unsigned char *data, unsigned int size, long long value) {
    if(value < 0) {
        return cbor_write_nlong(data, size, (unsigned long long)(-value));
    } else {
        return cbor_write_plong(data, size, (unsigned long long)(value));
    }
}

unsigned char *cbor_write_string_with_len(unsigned char *data, unsigned int size, const char *str, unsigned int str_size) {
    return cbor_write_type_size_bytes(data, size, 3, str_size, (unsigned char *) str);
}

unsigned char *cbor_write_string(unsigned char *data, unsigned int size, const char *str) {
    return cbor_write_type_size_bytes(data, size, 3, strlen(str), (unsigned char *) str);
}

unsigned char *cbor_write_bytes(unsigned char *data, unsigned int size, const unsigned char *bytes, unsigned int bytes_size) {
    return cbor_write_type_size_bytes(data, size, 2, bytes_size, bytes);
}

unsigned char *cbor_write_array(unsigned char *data, unsigned int size, unsigned int array_size) {
    return cbor_write_type_size(data, size, 4, array_size);
}

unsigned char *cbor_write_map(unsigned char *data, unsigned int size, unsigned int map_size) {
    return cbor_write_type_size(data, size, 5, map_size);
}

unsigned char *cbor_write_tag(unsigned char *data, unsigned int size, unsigned int tag) {
    return cbor_write_type_size(data, size, 6, tag);
}

unsigned char *cbor_write_special(unsigned char *data, unsigned int size, unsigned int special) {
    return cbor_write_type_size(data, size, 7, special);
}
