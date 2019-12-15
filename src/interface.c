//a c chat application because i like to torture myself
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <termios.h>
#include "fulux-interface.h"
#define CTRL_COMBINATION(x) (x & 0x1f)

typedef unsigned char byte;

unsigned short current_print_message_row = 3;

typedef enum {
    up = 0,
    down = 1,
    right = 2,
    left = 3
}CURSORMOVEDIRECTION;

void move_cursor(CURSORMOVEDIRECTION direction, byte ammount) {
    printf("\033[%u%c", ammount, 'A' + direction);
}

void exits(char* string, int code) {
    puts(string);
    exit(code);
}

void print_message(char* user, char* message) {
    struct winsize vts;
    unsigned short lines_moved;
    message[strlen(message)-1] = 0;
    ioctl(0,  TIOCGWINSZ, &vts);
    move_cursor(up, vts.ws_row);
    move_cursor(down, current_print_message_row - 1);
    lines_moved = (strlen(user) + strlen(message)) / vts.ws_col;
    current_print_message_row += ++lines_moved;
    printf("%s: ", user);
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] != '\n') putc(message[i], stdout);
        else continue;
    }
}

char* wait_for_message_input(char* username) {
    char message[MAXMSGLEN];
    fgets(message, MAXMSGLEN, stdin);
    putc(127, stdin);
    print_message(username, message);
}

void draw_interface(char* username) {
    printf("\033[2J\033[H"); //clear screen
    struct winsize vts;
    if (ioctl(0,  TIOCGWINSZ, &vts) == -1) exits("fuck you", 1) ;
    //make sure we're on 1,1
    move_cursor(up, vts.ws_row);
    move_cursor(left, vts.ws_col);
    printf("Username: %s\033[%uC| Fulux", username, vts.ws_col - 17 - strlen(username)); //FORMAT STRING VULNURABILITY!!!!!!
    putc('\r', stdout); 
    move_cursor(down, 1);
    for (int i = 0; i < vts.ws_col; i++)
        putc('#', stdout);
    putc('\r', stdout);
    move_cursor(down, vts.ws_row - 4);
    for (int i = 0; i < vts.ws_col; i++)
        putc('#', stdout);
    printf("\n\r> ");

    for (;;) {
        move_cursor(up, vts.ws_row);
        move_cursor(left, vts.ws_col);
        move_cursor(down, vts.ws_row-2);
        move_cursor(right, 2);
        fflush(stdin);
        wait_for_message_input(username);
    }
    
}

#ifdef _INTERFACE_DEBUG
int main(int argc, char* argv[]) {
    draw_interface("ass hole");

}
#endif
