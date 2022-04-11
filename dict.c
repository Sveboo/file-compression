#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Pair Pair;

typedef struct Bucket Bucket;

struct Pair {
    char *key;
    char value;
};

struct Bucket {
    unsigned int count;
    Pair *pairs;
};

typedef struct StrMap {
    unsigned int count;
    Bucket *buckets;
} StrMap;

static Pair * getPair(Bucket *bucket, const char *key);
static unsigned long hash(const char *str);

StrMap * smNew(unsigned int capacity)
{
    StrMap * map;

    map = malloc(sizeof(StrMap));
    if (map == NULL) {
        return NULL;
    }
    map->count = capacity;
    map->buckets = malloc(map->count * sizeof(Bucket));
    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }
    memset(map->buckets, 0, map->count * sizeof(Bucket));
    return map;
}


int smGet(const StrMap *map, const char *key)
{
    unsigned int index;
    Bucket *bucket;
    Pair *pair;

    if (map == NULL) {
        return 0;
    }
    if (key == NULL) {
        return 0;
    }
    index = hash(key) % map->count;
    bucket = &(map->buckets[index]);
    pair = getPair(bucket, key);
    if (pair == NULL) {
        return 0;
    }
    return pair->value;
}

int smExists(const StrMap *map, const char *key)
{
    unsigned int index;
    Bucket *bucket;
    Pair *pair;

    if (map == NULL) {
        return 0;
    }
    if (key == NULL) {
        return 0;
    }
    index = hash(key) % map->count;
    bucket = &(map->buckets[index]);
    pair = getPair(bucket, key);
    if (pair == NULL) {
        return 0;
    }
    return 1;
}

int smPut(StrMap *map, const char *key, const char value)
{
    unsigned int key_len, value_len, index;
    Bucket *bucket;
    Pair *tmp_pairs, *pair;
    char *tmp_value;
    char *new_key, *new_value;

    if (map == NULL) {
        return 0;
    }
    if (key == NULL) {
        return 0;
    }
    key_len = strlen(key);
    /* Get a pointer to the bucket the key string hashes to */
    index = hash(key) % map->count;
    bucket = &(map->buckets[index]);
    /* Check if we can handle insertion by simply replacing
     * an existing value in a key-value pair in the bucket.
     */
    if ((pair = getPair(bucket, key)) != NULL) {
        /* The bucket contains a pair that matches the provided key,
         * change the value for that pair to the new value.
         */
        /* Copy the new value into the pair that matches the key */
        pair->value = value;
        return 1;
    }
    /* Allocate space for a new key and value */
    new_key = malloc((key_len + 1) * sizeof(char));
    if (new_key == NULL) {
        return 0;
    }
    /* Create a key-value pair */
    if (bucket->count == 0) {
        /* The bucket is empty, lazily allocate space for a single
         * key-value pair.
         */
        bucket->pairs = malloc(sizeof(Pair));
        if (bucket->pairs == NULL) {
            free(new_key);
            return 0;
        }
        bucket->count = 1;
    }
    else {
        /* The bucket wasn't empty but no pair existed that matches the provided
         * key, so create a new key-value pair.
         */
        tmp_pairs = realloc(bucket->pairs, (bucket->count + 1) * sizeof(Pair));
        if (tmp_pairs == NULL) {
            free(new_key);
            return 0;
        }
        bucket->pairs = tmp_pairs;
        bucket->count++;
    }
    /* Get the last pair in the chain for the bucket */
    pair = &(bucket->pairs[bucket->count - 1]);
    pair->key = new_key;
    /* Copy the key and its value into the key-value pair */
    strcpy(pair->key, key);
    pair->value = value;
    return 1;
}

int smGetCount(const StrMap *map)
{
    unsigned int i, j, n, m;
    unsigned int count;
    Bucket *bucket;
    Pair *pair;

    if (map == NULL) {
        return 0;
    }
    bucket = map->buckets;
    n = map->count;
    i = 0;
    count = 0;
    while (i < n) {
        pair = bucket->pairs;
        m = bucket->count;
        j = 0;
        while (j < m) {
            count++;
            pair++;
            j++;
        }
        bucket++;
        i++;
    }
    return count;
}


/*
 * Returns a pair from the bucket that matches the provided key,
 * or null if no such pair exist.
 */
static Pair * getPair(Bucket *bucket, const char *key)
{
    unsigned int i, n;
    Pair *pair;

    n = bucket->count;
    if (n == 0) {
        return NULL;
    }
    pair = bucket->pairs;
    i = 0;
    while (i < n) {
        if (pair->key != NULL) {
            if (strcmp(pair->key, key) == 0) {
                return pair;
            }
        }
        pair++;
        i++;
    }
    return NULL;
}

/*
 * Returns a hash code for the provided string.
 */
static unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void decode(char* source, char* filename, StrMap* dict, unsigned long newFileLen, unsigned long len){
    // filename == NEW FILE NAME!
    FILE * out = fopen(filename, "wb");
    char *key = (char *) malloc(len * sizeof(char));
    char *result = (char *) malloc(len * sizeof(char));
    for(int i = 0; i < newFileLen * 8; ++i){
        if(smExists(dict, key)){  // If symb code exists then decode
            result[strlen(result)] = smGet(dict, key);
            key[0] = 0;
        }
        else{
            key[strlen(key)] = source[i]; // Else search for existing code
        }
    }
    fprintf(out, "%s", result); //Write nex file
}

int kmain () {
    StrMap* dict = smNew(256);
    char buff;
    smPut(dict, "keeeeey", '1');
    buff = smGet(dict, "keeeeey");
    printf("%c", buff);
    return 0;
}