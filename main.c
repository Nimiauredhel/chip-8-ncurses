#include "common.h"

int main(int argc, char *argv[])
{
    initscr();
    printw("Hello world!");
    refresh();
    usleep(1000000);
    endwin();
    return EXIT_SUCCESS;
}
