#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>
#include "filesystem_types.h"

// Function declarations
void fs_init();
void fs_get_path(char *buffer, size_t size);
void fs_mkdir(char *name);
void fs_cd(char *name);

#endif // FILESYSTEM_H