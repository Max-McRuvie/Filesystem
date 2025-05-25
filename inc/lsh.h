#ifndef LSH_H
#define LSH_H

// Function declarations
void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_split_line(char *line);
int lsh_execute(char **args);

#endif // LSH_H