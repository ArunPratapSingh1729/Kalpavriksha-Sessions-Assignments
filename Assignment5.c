#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 100
#define NAME_SIZE 50

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *next, *prev;
} FreeBlock;

typedef struct FileNode
{
    char name[NAME_SIZE];
    int isDir;
    struct FileNode *parent;
    struct FileNode *next, *prev;
    struct FileNode *children;
    int blocks[NUM_BLOCKS];
    int blockCount;
} FileNode;

char disk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeList = NULL;
FileNode *root = NULL, *cwd = NULL;

FreeBlock *newBlock(int index)
{
    FreeBlock *b = malloc(sizeof(FreeBlock));
    b->index = index;
    b->next = b->prev = NULL;
    return b;
}

void BlockListIntialization()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        FreeBlock *b = newBlock(i);
        if (!freeList)
            freeList = b->next = b->prev = b;
        else
        {
            b->prev = freeList->prev;
            b->next = freeList;
            freeList->prev->next = b;
            freeList->prev = b;
        }
    }

    root = malloc(sizeof(FileNode));
    strcpy(root->name, "/");
    root->isDir = 1;
    root->parent = root;
    root->children = NULL;
    root->next = root->prev = root;
    cwd = root;
}

FreeBlock *AllocateBlock()
{
    if (!freeList)
        return NULL;
    FreeBlock *block = freeList;
    if (block->next == block)
        freeList = NULL;
    else
    {
        block->prev->next = block->next;
        block->next->prev = block->prev;
        freeList = block->next;
    }
    return block;
}

void FreeDiskBlock(int index)
{
    FreeBlock *temp = freeList;
    if (temp)
    {
        do
        {
            if (temp->index == index)
                return;
            temp = temp->next;
        } while (temp != freeList);
    }

    FreeBlock *b = newBlock(index);
    if (!freeList)
        freeList = b->next = b->prev = b;
    else
    {
        b->prev = freeList->prev;
        b->next = freeList;
        freeList->prev->next = b;
        freeList->prev = b;
    }
}

int IsValidName(const char *name)
{
    if (!name || strlen(name) == 0 || strlen(name) >= NAME_SIZE)
        return 0;
    if (strchr(name, '/') || strchr(name, ' ') || isdigit(name[0]))
        return 0;
    return 1;
}

int Exists(const char *name)
{
    if (!cwd->children)
        return 0;
    FileNode *cur = cwd->children;
    do
    {
        if (strcmp(cur->name, name) == 0)
            return 1;
        cur = cur->next;
    } while (cur != cwd->children);
    return 0;
}

FileNode *FindNode(const char *name)
{
    if (!cwd->children)
        return NULL;
    FileNode *cur = cwd->children;
    do
    {
        if (strcmp(cur->name, name) == 0)
            return cur;
        cur = cur->next;
    } while (cur != cwd->children);
    return NULL;
}

void AddChild(FileNode *node)
{
    if (!cwd->children)
        cwd->children = node->next = node->prev = node;
    else
    {
        FileNode *last = cwd->children->prev;
        last->next = node;
        node->prev = last;
        node->next = cwd->children;
        cwd->children->prev = node;
    }
}

void mkdir(char *name)
{
    if (!IsValidName(name))
    {
        printf("Invalid directory name.\n");
        return;
    }
    if (Exists(name))
    {
        printf("Directory already exists.\n");
        return;
    }
    FileNode *d = malloc(sizeof(FileNode));
    strcpy(d->name, name);
    d->isDir = 1;
    d->children = NULL;
    d->parent = cwd;
    AddChild(d);
}

void create(char *name)
{
    if (!IsValidName(name))
    {
        printf("Invalid file name.\n");
        return;
    }
    if (Exists(name))
    {
        printf("File already exists.\n");
        return;
    }
    FileNode *f = malloc(sizeof(FileNode));
    strcpy(f->name, name);
    f->isDir = 0;
    f->parent = cwd;
    f->blockCount = 0;
    AddChild(f);
}

void write(char *name, char *content)
{
    if (!name || strlen(name) == 0)
    {
        printf("Error: Missing file name.\n");
        return;
    }
    if (!content || strlen(content) == 0)
    {
        printf("Error: No content to write.\n");
        return;
    }

    FileNode *f = FindNode(name);
    if (!f || f->isDir)
    {
        printf("Error: File not found.\n");
        return;
    }

    int size = strlen(content);
    int blocksNeeded = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    for (int i = 0; i < blocksNeeded; i++)
    {
        FreeBlock *b = AllocateBlock();
        if (!b)
        {
            printf("Disk full.\n");
            return;
        }
        f->blocks[f->blockCount++] = b->index;
        strncpy(disk[b->index], content + i * BLOCK_SIZE, BLOCK_SIZE);
        free(b);
    }
}

void read(char *name)
{
    FileNode *f = FindNode(name);
    if (!f || f->isDir)
    {
        printf("Error: File not found.\n");
        return;
    }
    for (int i = 0; i < f->blockCount; i++)
        printf("%s", disk[f->blocks[i]]);
    printf("\n");
}

void delete(char *name)
{
    FileNode *f = FindNode(name);
    if (!f || f->isDir)
    {
        printf("Error: File not found.\n");
        return;
    }

    for (int i = 0; i < f->blockCount; i++)
        FreeDiskBlock(f->blocks[i]);

    if (f->next == f)
        cwd->children = NULL;
    else
    {
        f->prev->next = f->next;
        f->next->prev = f->prev;
        if (cwd->children == f)
            cwd->children = f->next;
    }
    free(f);
}

void rmdir(char *name)
{
    FileNode *d = FindNode(name);
    if (!d || !d->isDir)
    {
        printf("Error: Directory not found.\n");
        return;
    }
    if (d->children)
    {
        printf("Directory not empty.\n");
        return;
    }

    if (d->next == d)
        cwd->children = NULL;
    else
    {
        d->prev->next = d->next;
        d->next->prev = d->prev;
        if (cwd->children == d)
            cwd->children = d->next;
    }
    free(d);
}

void RecursiveDelete(FileNode *node)
{
    if (!node)
        return;
    while (node->isDir && node->children)
        RecursiveDelete(node->children);
    for (int i = 0; i < node->blockCount; i++)
        FreeDiskBlock(node->blocks[i]);
    if (node->next == node)
        node->parent->children = NULL;
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if (node->parent->children == node)
            node->parent->children = node->next;
    }
    free(node);
}

void forcermdir(char *name)
{
    FileNode *d = FindNode(name);
    if (!d || !d->isDir)
    {
        printf("Error: Directory not found.\n");
        return;
    }
    RecursiveDelete(d);
}

void ls()
{
    if (!cwd->children)
    {
        printf("Empty\n");
        return;
    }
    FileNode *cur = cwd->children;
    do
    {
        printf("%s%s\n", cur->name, cur->isDir ? "/" : "");
        cur = cur->next;
    } while (cur != cwd->children);
}

void cd(char *name)
{
    if (strcmp(name, "..") == 0)
        cwd = cwd->parent;
    else
    {
        FileNode *d = FindNode(name);
        if (d && d->isDir)
            cwd = d;
        else
            printf("Directory not found.\n");
    }
}

void pwd(FileNode *dir)
{
    if (dir == root)
    {
        printf("/");
        return;
    }
    pwd(dir->parent);
    printf("%s/", dir->name);
}

void df()
{
    int freeCount = 0;
    if (freeList)
    {
        FreeBlock *cur = freeList;
        do
        {
            freeCount++;
            cur = cur->next;
        } while (cur != freeList);
    }
    printf("Free: %d, Used: %d\n", freeCount, NUM_BLOCKS - freeCount);
}

void ExitSystem()
{
    if (root->children)
        RecursiveDelete(root->children);
    printf("Exiting system...\n");
    exit(0);
}

int main()
{
    BlockListIntialization();

    char cmd[50], arg1[50], arg2[512];

    while (1)
    {
        printf("%s> ", cwd->name);
        int n = scanf("%s", cmd);
        if (n == EOF)
            break;

        if (!strcmp(cmd, "mkdir"))
            scanf("%s", arg1), mkdir(arg1);
        else if (!strcmp(cmd, "create"))
            scanf("%s", arg1), create(arg1);
        else if (!strcmp(cmd, "write"))
        {
            scanf("%s", arg1);
            getchar();
            fgets(arg2, sizeof(arg2), stdin);
            arg2[strcspn(arg2, "\n")] = 0;
            write(arg1, arg2);
        }
        else if (!strcmp(cmd, "read"))
            scanf("%s", arg1), read(arg1);
        else if (!strcmp(cmd, "delete"))
            scanf("%s", arg1), delete(arg1);
        else if (!strcmp(cmd, "rmdir"))
            scanf("%s", arg1), rmdir(arg1);
        else if (!strcmp(cmd, "frmdir"))
            scanf("%s", arg1), forcermdir(arg1);
        else if (!strcmp(cmd, "ls"))
            ls();
        else if (!strcmp(cmd, "cd"))
            scanf("%s", arg1), cd(arg1);
        else if (!strcmp(cmd, "pwd"))
            pwd(cwd), printf("\n");
        else if (!strcmp(cmd, "df"))
            df();
        else if (!strcmp(cmd, "exit"))
            ExitSystem();
        else
            printf("Unknown command.\n");
    }
    return 0;
}
