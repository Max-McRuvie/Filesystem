#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>
#include "filesystem_types.h"

// Function declarations
void fs_init(void);
void fs_mkdir(char *folder_directory);
void fs_cd(char *target_directory);
void fs_ls(void);
void fs_touch(char *file_name);
void fs_write(char *file_name, char *content);
void fs_read(char *file_name);
void fs_rm(char *name);
void fs_get_path(char *path_buffer, size_t buffer_size);

#endif // FILESYSTEM_H