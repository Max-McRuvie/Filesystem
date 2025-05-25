#include "file_system.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "filesystem_types.h"

static Node* root = NULL;
static Node* current_dir = NULL;

void fs_init() {
    root = malloc(sizeof(Node));
    strcpy(root->name, "/");
    root->type = FOLDER_NODE;
    root->parent = NULL;
    root->next_sibling = NULL;
    root->first_child = NULL;

    current_dir = root;
}

void fs_get_path(char *buffer, size_t size) {
    if(!current_dir) {
        snprintf(buffer, size, "/");
        return;
    }

    char temp[1024] = "";
    Node* node = current_dir;

    while(node && node->parent){
        char segment[1024];
        snprintf(segment, sizeof(segment), "%s", node->name);
        memmove(temp + strlen(segment), temp, strlen(temp) + 1);
        memcpy(temp, segment, strlen(segment));
        node = node->parent;
    }

    snprintf(buffer, size, "%s", *temp ? temp : "/");
}