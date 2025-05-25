#include "lsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_system.h"

// Defines the initial buffer size for reading input lines
#define LSH_RL_BUFSIZE 1024

// Defines the initial buffer size and delimiters for tokenizing input
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char *lsh_read_line(void){
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate
        if (position >= bufsize) {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **lsh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        // Resize token array if needed
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int lsh_execute(char **args) {
    if (args[0] == NULL) {
        return 1; // Empty command
    }

    // FS commands
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    } 
    
    if (strcmp(args[0], "cd") == 0) { 
        if (args[1]) {
            fs_cd(args[1]);
        } else {
            printf("cd: missing argument\n");
        }
        return 1;
    }

    if (strcmp(args[0], "mkdir") == 0) { 
        if (args[1]) {
            fs_mkdir(args[1]);
        } else {
            printf("mkdir: missing argument\n");
        }
        return 1;
    }

    
    printf("lsh: unknown command: %s\n", args[0]);
    return 1;
}

void lsh_loop(void){
    char path[1024];
    char *line;
    char **args;
    int status;

    do {
        fs_get_path(path, sizeof(path));
        printf("%s > ", path);

        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}



