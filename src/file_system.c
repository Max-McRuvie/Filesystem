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
    root->content = NULL;

    current_dir = root;
}

void fs_mkdir(char *folder_directory) {
    char path_copy[256];
    strncpy(path_copy, folder_directory, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *token = strtok(path_copy, "/");
    Node *current = current_dir;

    while (token != NULL) {
        Node *child = current_dir->first_child;
        Node *existing = NULL;

        while (child) {
            if(strcmp(child->name, token) == 0 && child->type == FOLDER_NODE) {
                existing = child;
                break;
            }
            child = child->next_sibling;
        }

        if (existing) {
            current = existing;
        } else {
            Node *new_dir = malloc(sizeof(Node));
            if (!new_dir) {
                fprintf(stderr, "fs: allocation error\n");
                exit(EXIT_FAILURE);
            }

            strncpy(new_dir->name, token, sizeof(new_dir->name));
            new_dir->name[sizeof(new_dir->name) - 1] = '\0';

            new_dir->type = FOLDER_NODE;
            new_dir->parent = current;
            new_dir->first_child = NULL;
            new_dir->next_sibling = NULL;
            new_dir->content = NULL;

            if(current->first_child == NULL) {
            current->first_child = new_dir;
            } else {
                Node *sibling = current->first_child;
                while (sibling->next_sibling != NULL) {
                    sibling = sibling->next_sibling;
                }
                sibling->next_sibling = new_dir;
            }
            current = new_dir;
        }
        token = strtok(NULL, "/");
    }
}

void fs_cd(char *target_directory) {
    if(strcmp(target_directory, "..") == 0) {
        if(current_dir->parent != NULL) {
            current_dir = current_dir->parent;
        }
        return;
    }

    Node *child = current_dir->first_child;
    while (child != NULL) {
        if (child->type == FOLDER_NODE && strcmp(child->name, target_directory) == 0) {
            current_dir = child;
            return;
        }
        child = child->next_sibling;
    }

    printf("cd: no such directory: %s\n", target_directory);
}

void fs_ls(void) {
    if(current_dir->first_child == NULL) {
        return;
    }

    Node *child = current_dir->first_child;

    while(child) {
        if (child->type == FOLDER_NODE) {
            printf("%s/ ", child->name);
        } else {
            printf("%s ", child->name);
        }
        child = child->next_sibling;
    }
    printf("\n");
}

void fs_touch(char *file_name) {
    Node *new_file = malloc(sizeof(Node));
    if (!new_file) {
        fprintf(stderr, "fs: allocation error\n");
        exit(EXIT_FAILURE);
    }

    strncpy(new_file->name, file_name, sizeof(new_file->name) - 1);
    new_file->name[sizeof(new_file->name) - 1] = '\0';

    new_file->type = FILE_NODE;
    new_file->parent = current_dir;
    new_file->first_child = NULL;
    new_file->next_sibling = NULL;
    new_file->content = NULL;

    if (current_dir->first_child == NULL) {
        current_dir->first_child = new_file;
    } else {
        Node *sibling = current_dir->first_child;
        while (sibling->next_sibling != NULL) {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = new_file;
    }
}

void fs_write(char *file_name, char *text) {
    Node *child = current_dir->first_child;

    while (child) {
        if(child->type == FILE_NODE && strcmp(child->name, file_name) == 0){
            free(child->content);
            child->content = strdup(text);
            return;
        }
        child = child->next_sibling;
    }
    printf("write: file '%s' not found\n", file_name);
}

void fs_read(char *file_name) {
    Node *child = current_dir->first_child;

    while (child) {
        if(child->type == FILE_NODE && strcmp(child->name, file_name) == 0){
            printf("%s \n", child->content);
            return;
        } else {
            printf("[empty file]\n");
        }
        child = child->next_sibling;
    }
    printf("read: file '%s' not found\n", file_name);
}

void fs_rm(char *name) {
    Node *child = current_dir->first_child;

    while (child) {
        if(child->type == FILE_NODE && strcmp(child->name, name) == 0) {
            free(child);
            return;
        } else if (child->type == FOLDER_NODE && strcmp(child->name, name) == 0) {
            if(child->first_child == NULL) {
                free(child);
                return;
            } else {
                printf("rm: directory not empty\n");
                return;
            }
        }
        child = child->next_sibling;
    }
    printf("rm: file '%s' not found\n", name);
}

void fs_get_path(char *path_buffer, size_t buffer_size) {
    if(!current_dir) {
        snprintf(path_buffer, buffer_size, "/");
        return;
    }

    const char *segments[128];
    int count = 0;
    Node *node = current_dir;

    while (node != NULL && node->parent != NULL) {
        segments[count++] = node->name;
        node = node->parent;
    }

    path_buffer[0] = '/';
    path_buffer[1] = '\0';

    for(int i = count - 1; i >= 0; i-- ) {
        strncat(path_buffer, segments[i], buffer_size - strlen(path_buffer) - 1);
        if(i > 0) {
            strncat(path_buffer, "/", buffer_size - strlen(path_buffer) - 1);
        }
    }

    if (strlen(path_buffer) == 0) {
        strncpy(path_buffer, "/", buffer_size);
    }
}