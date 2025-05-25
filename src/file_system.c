#include "file_system.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "filesystem_types.h"

static Node* root = NULL;
static Node* current_dir = NULL;

void fs_init(void) {
    root = malloc(sizeof(Node));
    strcpy(root->name, "/");
    root->type = FOLDER_NODE;
    root->parent = NULL;
    root->next_sibling = NULL;
    root->first_child = NULL;

    current_dir = root;
}

void fs_mkdir(char *name) {
    Node *new_dir = malloc(sizeof(Node));
    if (!new_dir) {
        fprintf(stderr, "fs: allocation error\n");
        exit(EXIT_FAILURE);
    }

    strncpy(new_dir->name, name, sizeof(new_dir->name) - 1);
    new_dir->name[sizeof(new_dir->name) - 1] = '\0';

    new_dir->type = FOLDER_NODE;
    new_dir->parent = current_dir;
    new_dir->first_child = NULL;
    new_dir->next_sibling = NULL;

    if (current_dir->first_child == NULL) {
        current_dir->first_child = new_dir;
    } else {
        Node *sibling = current_dir->first_child;
        while (sibling->next_sibling != NULL) {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = new_dir;
    }
}

void fs_cd(char *name) {
    if(strcmp(name, "..") == 0) {
        if(current_dir->parent != NULL) {
            current_dir = current_dir->parent;
        }
        return;
    }

    Node *child = current_dir->first_child;
    while (child != NULL) {
        if (child->type == FOLDER_NODE && strcmp(child->name, name) == 0) {
            current_dir = child;
            return;
        }
        child = child->next_sibling;
    }

    printf("cd: No such directory: %s\n", name);
}

void fs_get_path(char *buffer, size_t size) {
    if(!current_dir) {
        snprintf(buffer, size, "/");
        return;
    }

    const char *segments[128];
    int count = 0;
    Node *node = current_dir;

    while (node != NULL && node->parent != NULL) {
        segments[count++] = node->name;
        node = node->parent;
    }

    buffer[0] = '/';
    buffer[1] = '\0';

    for(int i = count - 1; i >= 0; i-- ) {
        strncat(buffer, segments[i], size - strlen(buffer) - 1);
        if(i > 0) {
            strncat(buffer, "/", size - strlen(buffer) - 1);
        }
    }

    if (strlen(buffer) == 0) {
        strncpy(buffer, "/", size);
    }
}