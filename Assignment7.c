#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_CAPACITY 3
#define TABLE_SIZE 7

typedef struct Node
{
    int key;
    char value[32];
    struct Node *prev, *next;
    struct Node *hnext;
} Node;

typedef struct
{
    Node **table;
    int table_size;
} HashMap;

typedef struct
{
    int capacity, size;
    Node *head, *tail;
    HashMap *map;
} LRUCache;

int hashFunc(int key, int tableSize)
{
    return key % tableSize;
}

int isValidKey(int key)
{
    return key >= 0;
}

int isValidValue(const char *value)
{
    if (!value)
        return 0;
    if (value[0] == '\0')
        return 0;
    return 1;
}

Node *createNode(int key, const char *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->key = key;
    strncpy(node->value, value, sizeof(node->value));
    node->value[sizeof(node->value) - 1] = '\0';
    node->prev = NULL;
    node->next = NULL;
    node->hnext = NULL;
    return node;
}

HashMap *createHashMap(int size)
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (!map)
        return NULL;
    map->table = (Node **)calloc(size, sizeof(Node *));
    if (!map->table)
    {
        free(map);
        return NULL;
    }
    map->table_size = size;
    return map;
}

LRUCache *lruCreate(int capacity)
{
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    if (!cache)
        return NULL;
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->map = createHashMap(TABLE_SIZE);
    if (!cache->map)
    {
        free(cache);
        return NULL;
    }
    return cache;
}

void removeNode(LRUCache *cache, Node *node)
{
    if (!node)
        return;
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
    node->prev = NULL;
    node->next = NULL;
}

void insertTail(LRUCache *cache, Node *node)
{
    node->prev = cache->tail;
    node->next = NULL;
    if (cache->tail)
        cache->tail->next = node;
    else
        cache->head = node;
    cache->tail = node;
}

void moveToTail(LRUCache *cache, Node *node)
{
    removeNode(cache, node);
    insertTail(cache, node);
}

Node *hashMapGet(HashMap *map, int key)
{
    int idx = hashFunc(key, map->table_size);
    Node *n = map->table[idx];
    while (n)
    {
        if (n->key == key)
            return n;
        n = n->hnext;
    }
    return NULL;
}

void hashMapPut(HashMap *map, Node *node)
{
    int idx = hashFunc(node->key, map->table_size);
    node->hnext = map->table[idx];
    map->table[idx] = node;
}

void hashMapRemove(HashMap *map, int key)
{
    int idx = hashFunc(key, map->table_size);
    Node **ind = &map->table[idx];
    while (*ind)
    {
        if ((*ind)->key == key)
        {
            Node *tmp = *ind;
            *ind = tmp->hnext;
            tmp->hnext = NULL;
            return;
        }
        ind = &(*ind)->hnext;
    }
}

void lruPut(LRUCache *cache, int key, const char *value)
{
    if (!isValidKey(key))
        return;
    if (!isValidValue(value))
        return;

    Node *node = hashMapGet(cache->map, key);
    if (node)
    {
        strncpy(node->value, value, sizeof(node->value));
        node->value[sizeof(node->value) - 1] = '\0';
        moveToTail(cache, node);
    }
    else
    {
        node = createNode(key, value);
        if (!node)
            return;
        insertTail(cache, node);
        hashMapPut(cache->map, node);
        cache->size++;
        if (cache->size > cache->capacity)
        {
            Node *old = cache->head;
            removeNode(cache, old);
            hashMapRemove(cache->map, old->key);
            free(old);
            cache->size--;
        }
    }
}

const char *lruGet(LRUCache *cache, int key)
{
    if (!isValidKey(key))
        return NULL;

    Node *node = hashMapGet(cache->map, key);
    if (!node)
        return NULL;
    moveToTail(cache, node);
    return node->value;
}

void lruPrint(LRUCache *cache)
{
    printf("CACHE FROM LRU TO MRU: ");
    Node *n = cache->head;
    while (n)
    {
        printf("[%d: %s] ", n->key, n->value);
        n = n->next;
    }
    printf("\n");
}

int main()
{
    LRUCache *cache = lruCreate(CACHE_CAPACITY);
    if (!cache)
        return 1;

    lruPut(cache, 1, "A");
    lruPut(cache, 2, "B");
    lruPut(cache, 3, "C");
    lruPrint(cache);

    lruGet(cache, 2);
    lruPrint(cache);

    lruPut(cache, 4, "D");
    lruPrint(cache);

    const char *val = lruGet(cache, 3);
    if (val)
        printf("Get 3: %s\n", val);
    else
        printf("Get 3: not found\n");

    lruPrint(cache);

    return 0;
}
