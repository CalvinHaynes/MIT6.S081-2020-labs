#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        fprintf(2, "Please enter the parameters!\n");
        exit(1);
    }

    if (argc > 2) {
        fprintf(2, "Too many args!\n");
        exit(1);
    }

    uint32 duration = atoi(argv[1]);
    sleep(duration);

    exit(0);
}