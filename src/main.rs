use pancurses::*;

mod local_game;

fn refresh_menu(display: &Window, selection: i32) {
    display.clear();
    display.mvaddstr(0, 1, "1. Singleplayer");
    display.mvaddstr(1, 1, "2. Multiplayer");
    display.mvaddstr(2, 1, "3. Exit");
    display.mvaddstr(selection, 0, "*");
    display.refresh();
}

fn add_select(plus: bool, selection: &mut i32) {
    if plus {
        *selection += 1;
        if *selection > 2 {
            *selection = 2;
        }
    }
    else {
        *selection -= 1;
        if *selection < 0 {
            *selection = 0;
        }
    }
}

fn main() {
    let display = initscr();
    display.keypad(true);
    noecho();
    curs_set(0);
    display.timeout(0);

    let mut selection: i32 = 0;
    refresh_menu(&display, selection);
    let mut inmenu = true;
    while inmenu {
        let key = display.getch();
        if key == Some(Input::KeyUp) {
            add_select(false, &mut selection);
            refresh_menu(&display, selection);
        }
        if key == Some(Input::KeyDown) {
            add_select(true, &mut selection);
            refresh_menu(&display, selection);
        }

        if key == Some(Input::Character('f')) || key == Some(Input::Character('F')) {
            if selection == 0 {
                inmenu = false;
                local_game::start_game();
            }
            else if selection == 1 {
                display.clear();
                display.mvaddstr(0, 0, "Not working sorry...");
            } else if selection == 2 {
                inmenu = false;
            }
        }
    }
    endwin(); 
}