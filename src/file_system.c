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
    current_dir->first_child = new_dir;
    new_dir->first_child = NULL;

    new_dir->next_sibling = current_dir->first_child;
    current_dir->first_child = new_dir;
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