#include <ncurses.h>
#include <stdbool.h>
#include "mainmenu.c"

#define MAPX_SIZE 30
#define MAPY_SIZE 10
#define MAX_OBJECTS 15 
#define MAX_COLIDE_OBJECTS 15 

struct Player {
    int px;
    int py;
    char player;
};

struct Object {
    int px;
    int py;
    char object;
};

struct ColideObject {
    int px;
    int py;
    char object;
    bool has_symbol;
};

void move_posx_plus(int *px, int *py, struct ColideObject** co, int coc) {
    *px += 1;
    if (*px >= MAPX_SIZE) {
        *px = MAPX_SIZE - 1;
    }
    for (int obj = 0; obj < coc; obj++) {
        if (co[obj]->py == *py && co[obj]->px == *px) {
            *px = co[obj]->px - 1;
        }
    }
}

void move_posx_minus(int *px, int *py, struct ColideObject** co, int coc) {
    *px -= 1;
    if (*px <= 0) {
        *px = 0;
    }
    for (int obj = 0; obj < coc; obj++) {
        if (co[obj]->px == *px && co[obj]->py == *py) {
            *px = co[obj]->px + 1;
        }
    }
}

void move_posy_minus(int *px, int *py, struct ColideObject** co, int coc) {
    *py -= 1;
    if (*py <= 0) {
        *py = 0;
    }
    for (int obj = 0; obj < coc; obj++) {
        if (co[obj]->py == *py && co[obj]->px == *px) {
            *py = co[obj]->py + 1;
        }
    }
}

void move_posy_plus(int *px, int *py, struct ColideObject** co, int coc) {
    *py += 1;
    if (*py >= MAPY_SIZE) {
        *py = MAPY_SIZE - 1;
    }
    for (int obj = 0; obj < coc; obj++) {
        if (co[obj]->py == *py && co[obj]->px == *px) {
            *py = co[obj]->py - 1;
        }
    }
}

void update_map(struct Player *p, struct Object** objects, int objects_count, struct ColideObject** cob, int cobc, char map) { 
    clear();
    printw(" ");
    for (int border_up = 0; border_up < MAPX_SIZE; border_up++) {
        printw("_");
    }
    printw("\n");
    for (int row = 0; row < MAPY_SIZE; row++) {
        printw("|");
        for (int collumn = 0; collumn < MAPX_SIZE; collumn++) {
            char symbol = map;
            if (row == p->py && collumn == p->px) {
                symbol = p->player;
            }
            for (int object = 0; object < objects_count; object++) {
                if (row+1 == objects[object]->py && collumn == objects[object]->px) {
                    if (objects[object]->py == p->py && objects[object]->px == p->px) {
                        symbol = '.';
                    }
                }
                if (row == objects[object]->py && collumn == objects[object]->px) {
                    symbol = objects[object]->object;
                }
            }
            for (int c_object = 0; c_object < cobc; c_object++) {
                if (row == cob[c_object]->py && collumn == cob[c_object]->px) {
                    if (cob[c_object]->has_symbol) {
                        symbol = cob[c_object]->object;
                    }
                }
            }
            printw("%c", symbol);
        }
        printw("|");
        printw("\n");
    }
    printw(" ");
    for (int border_down = 0; border_down < MAPX_SIZE; border_down++) {
        printw("_");
    }
    
    printw("\nUse arrows to move, [q] - quit, [f] - interact");
    printw("\nX: %d\nY: %d", p->px, p->py);

    printw("\n\n");
    refresh();
}

void to_password_interact(struct Object **objects) {
    int sfw = 0;
    if (objects[sfw]->object == '1') {
        objects[sfw]->object = '0';
    }
    else {
        objects[sfw]->object = '1';
    }
}

void to_password1_interact(struct Object **objects) {
    int sfw = 1;
    if (objects[sfw]->object == '1') {
        objects[sfw]->object = '0';
    }
    else {
        objects[sfw]->object = '1';
    }
}

void object_interact(struct Player *p, struct Object **objects) { 
    if (p->px == objects[0]->px && p->py == objects[0]->py) {
        to_password_interact(objects);
    }
    if (p->px == objects[1]->px && p->py == objects[1]->py) {
        to_password1_interact(objects);
    }
}

int start_game() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    struct Player p;
    p.px = 0;
    p.py = 0;
    p.player = '#';
    char map_ris = ' ';

    struct Object topassword;
    topassword.px = 12;
    topassword.py = 5;
    topassword.object = '0';

    struct Object topassword1;
    topassword1.px = 11;
    topassword1.py = 5;
    topassword1.object = '0';

    struct ColideObject block;
    block.px = 11;
    block.py = 3;
    block.object = 'C';
    block.has_symbol = true;

    struct Object* objects[MAX_OBJECTS];
    int objects_count = 0;

    struct ColideObject* colide_objects[MAX_COLIDE_OBJECTS];
    int colide_objects_count = 0;

    objects[objects_count++] = &topassword; 
    objects[objects_count++] = &topassword1;

    colide_objects[colide_objects_count++] = &block;

    bool playing = true;
    update_map(&p, objects, objects_count, colide_objects, colide_objects_count, map_ris);
    while (playing) {
        int key = getch();
        if (key != ERR) {
            if (key == 'Q' || key == 'q') {
                playing = false;
            }
            else {
                switch (key) {
                    case KEY_RIGHT:
                        move_posx_plus(&p.px, &p.py, colide_objects, colide_objects_count);
                        update_map(&p, objects, objects_count, colide_objects, colide_objects_count, map_ris);
                        break;
                    case KEY_LEFT:
                        move_posx_minus(&p.px, &p.py, colide_objects, colide_objects_count);
                        update_map(&p, objects, objects_count, colide_objects, colide_objects_count, map_ris);
                        break;
                    case KEY_DOWN:
                        move_posy_plus(&p.px, &p.py, colide_objects, colide_objects_count);
                        update_map(&p, objects, objects_count, colide_objects, colide_objects_count, map_ris);
                        break;
                    case KEY_UP:
                        move_posy_minus(&p.px, &p.py, colide_objects, colide_objects_count);
                        update_map(&p, objects, objects_count, colide_objects, colide_objects_count, map_ris);
                        break;
                    case 'F':
                    case 'f':
                        object_interact(&p, objects);
                        update_map(&p, objects, objects_count, colide_objects, colide_objects_count, map_ris);
                        break;
                }
            }
        }
        napms(10);
    }
    endwin();
    return 0;
}

int main() {
    while (true) {
        int runsel = mainMenu();
        if (runsel == 10) {
            return 0;
        }
        else {
            start_game();
        }
    }
}
