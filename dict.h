#ifndef FILE_COMPRESSION_DICT_H
#define FILE_COMPRESSION_DICT_H

struct Pair {
    char *key;
    char value;
} ;

struct Bucket {
    unsigned int count;
    struct PAIR *pairs;
};

typedef struct StrMap {
    unsigned int count;
    struct BUCKET *buckets;
} STRMAP;
STRMAP * smNew(unsigned int capacity);

int smGet(const STRMAP *map, const char *key);

int smExists(const STRMAP *map, const char *key);

int smPut(STRMAP *map, const char *key, const char value);

#endif //FILE_COMPRESSION_DICT_H
