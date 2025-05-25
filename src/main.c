#include <stdlib.h>
#include "file_system.h"
#include "lsh.h"

int main(int args, char **argv){
    fs_init();
    lsh_loop();
    return EXIT_SUCCESS;
}
