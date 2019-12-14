#include "fulux-interface.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char username[MAXUSERNAME];
    printf("\e[2J\e[HUsername: ");
    fgets(username, MAXUSERNAME, stdin);
    fflush(stdin);
    username[strlen(username)-1] = 0;
    draw_interface(username);
}