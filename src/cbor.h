#ifndef CBOR_H
#define CBOR_H

#define CBOR_TOKEN_TYPE_INT 1
#define CBOR_TOKEN_TYPE_LONG 2
#define CBOR_TOKEN_TYPE_MAP 3
#define CBOR_TOKEN_TYPE_ARRAY 4
#define CBOR_TOKEN_TYPE_STRING 5
#define CBOR_TOKEN_TYPE_BYTES 6
#define CBOR_TOKEN_TYPE_TAG 7
#define CBOR_TOKEN_TYPE_SPECIAL 8

#define CBOR_TOKEN_TYPE_INCOMPLETE 1000
#define CBOR_TOKEN_TYPE_ERROR 2000


struct cbor_token {
    unsigned int type;
    int sign;
    unsigned int int_value;
    unsigned long long long_value;
    char *string_value;
    unsigned char *bytes_value;
    const char *error_value;
};

unsigned int cbor_read_token(unsigned char *data, unsigned int size, unsigned int offset, struct cbor_token *token);

unsigned char *cbor_write_type_size(unsigned char *data, unsigned int size, unsigned int type, unsigned int type_size);
unsigned char *cbor_write_type_long_size(unsigned char *data, unsigned int size, unsigned int type, unsigned long long type_size);
unsigned char *cbor_write_type_size_bytes(unsigned char *data, unsigned int size, unsigned int type, unsigned int type_size, const unsigned char *bytes);
unsigned char *cbor_write_pint(unsigned char *data, unsigned int size, unsigned int value);
unsigned char *cbor_write_nint(unsigned char *data, unsigned int size, unsigned int value);
unsigned char *cbor_write_plong(unsigned char *data, unsigned int size, unsigned long long value);
unsigned char *cbor_write_nlong(unsigned char *data, unsigned int size, unsigned long long value);
unsigned char *cbor_write_uint(unsigned char *data, unsigned int size, unsigned int value);
unsigned char *cbor_write_ulong(unsigned char *data, unsigned int size, unsigned long long value);
unsigned char *cbor_write_int(unsigned char *data, unsigned int size, int value);
unsigned char *cbor_write_long(unsigned char *data, unsigned int size, long long value);
unsigned char *cbor_write_string(unsigned char *data, unsigned int size, const char *str);
unsigned char *cbor_write_string_with_len(unsigned char *data, unsigned int size, const char *str, unsigned int str_size);
unsigned char *cbor_write_bytes(unsigned char *data, unsigned int size, const unsigned char *bytes, unsigned int bytes_size);
unsigned char *cbor_write_array(unsigned char *data, unsigned int size, unsigned int array_size);
unsigned char *cbor_write_map(unsigned char *data, unsigned int size, unsigned int map_size);
unsigned char *cbor_write_tag(unsigned char *data, unsigned int size, unsigned int tag);
unsigned char *cbor_write_special(unsigned char *data, unsigned int size, unsigned int special);

#endif