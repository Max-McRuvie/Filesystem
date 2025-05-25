#ifndef FILESYSTEM_TYPES_H
#define FILESYSTEM_TYPES_H

typedef enum {FILE_NODE, FOLDER_NODE } NodeType;

typedef struct Node {
    char name[256];
    NodeType type;
    struct Node* parent;
    struct Node* next_sibling;
    struct Node* first_child; // Only meaningful for folders
    char *content; // Only meaningful for files
} Node;

#endif // FILESYSTEM_TYPES_H