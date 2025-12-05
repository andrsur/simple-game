#ifndef MAINMENU_C
#define MAINMENU_C

#include <ncurses.h>
#include <stdbool.h>

void move_selection(int sel, int *selected, char* points[]) {
    clear();
    if (sel == 2) {
        *selected -= 1;
        if (*selected < 0) {
            *selected = 0;
        }
    }
    else if (sel == 1) {
        *selected += 1;
        if (*selected > 1) {
            *selected = 1;
        }
    }
    
    for (int point = 0; point < 2; point++) {
        if (point == *selected) {
            printw("%s <---\n", points[point]);
        } else {
            printw("%s\n", points[point]);
        }
    }
    printw("\npress [f] to select");
    printw("\n\n");
    refresh();
}  

int mainMenu() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    char* points[4] = {"Play", "Quit"};
    int selected = 0;
    int selectionreturn;
    bool in_menu = true;
    move_selection(0, &selected, points);
    while (in_menu) {
        int key = getch();
        if (key != ERR) {
            switch (key) {
                case KEY_UP:
                    move_selection(2, &selected, points);
                    break;
                case KEY_DOWN:
                    move_selection(1, &selected, points);
                    break;
                case 'F':
                case 'f':
                    if (selected == 0) {
                        selectionreturn = 1;
                    } else {
                        selectionreturn = 10;
                    }
                    in_menu = false;
                    break;
            }
        }
        napms(10);
    }
    endwin();
    return selectionreturn;
}

#endif