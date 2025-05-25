#include "file_system.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "filesystem_types.h"

static Node* current_dir = NULL;

Node* fs_init(){
    Node* root = malloc(sizeof(Node));
    strcpy(root->name, "/");
    root->type = FOLDER_NODE;
    root->parent = NULL;
    root->next_sibling = NULL;
    root->first_child = NULL;
    return root;
}

void fs_get_path(char *buffer, size_t size) {
    if(!current_dir) {
        snprintf(buffer, size, "/");
        return;
    }
}