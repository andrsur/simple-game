use pancurses::*;
use rand::Rng;

struct Player {
    posx: i32,
    posy: i32,
    symbol: String
}

struct Object {
    posx: i32,
    posy: i32,
    symbol: String,
    colision: bool
}

fn refresh_map(display: &Window, map_size: [i32; 2], lcp: &Player, objects: &[Object], mutables: &[i32]) {
    display.clear();
    for border_upper in 0..map_size[0]+1 {
        display.mvaddstr(0, border_upper, "▁");
    }

    for row in 1..map_size[1]  {
        display.mvaddstr(row, 0, "▏");
        display.mvaddstr(row, map_size[0], "▕");
        for collumn in 1..map_size[0] {
            display.mvaddstr(row, collumn, " ");
        }
    }
    display.mvaddstr(lcp.posy, lcp.posx, &lcp.symbol);

    for object in objects {
        display.mvaddstr(object.posy, object.posx, &object.symbol);
        if object.posx == lcp.posx && object.posy == lcp.posy {
            display.mvaddstr(object.posy-1, object.posx, "↡");
        }
    }

    for border_down in 0..map_size[0]+1 {
        display.mvaddstr(map_size[1], border_down, "▔");
    }

    display.mvaddstr(map_size[1]+2, 0, "[Q] - exit, [F] - interact, [arrows] - move");
    display.mvaddstr(map_size[1]+3, 0, format!("Score: {}", mutables[0]));

    display.refresh();
}

fn add_player_posy(plus: bool, lcp: &mut Player, objects: &[Object], map_size: [i32; 2]) {
    let coliderepair = lcp.posy;
    if plus {
        lcp.posy += 1;
        if lcp.posy >= map_size[1] {
            lcp.posy = map_size[1]-1;
        }
    }
    else {
        lcp.posy -= 1;
        if lcp.posy <= 1 {
            lcp.posy = 1;
        }
    }
    for object in objects {
        if object.posx == lcp.posx && object.posy == lcp.posy && object.colision {
            lcp.posy = coliderepair;
        }
    }
}

fn add_player_posx(plus: bool, lcp: &mut Player, objects: &[Object], map_size: [i32; 2]) {
    let coliderepair = lcp.posx;
    if plus {
        lcp.posx += 1;
        if lcp.posx >= map_size[0] {
            lcp.posx = map_size[0]-1;
        }
    }
    else {
        lcp.posx -= 1;
        if lcp.posx <= 1 {
            lcp.posx = 1;
        }
    }
    for object in objects {
        if object.posx == lcp.posx && object.posy == lcp.posy && object.colision {
            lcp.posx = coliderepair;
        }
    }
}

fn interact(lcp: &Player, objects: &mut [Object], mutables: &mut [i32]) {
    if lcp.posx == objects[0].posx && lcp.posy == objects[0].posy {
        let mut rng = rand::rng();
        objects[0].posx = rng.random_range(2..=49);
        objects[0].posy = rng.random_range(2..=19);
        mutables[0] += 1;
    }
}

pub fn start_game() {
    let display = initscr();
    display.keypad(true);
    noecho();
    curs_set(0);
    display.timeout(0);

    let map_size= [50, 20]; // [x, y]

    let mut localplayer = Player{
        posx: 10, 
        posy: 5, 
        symbol: "●".to_string()
    };

    let mut objects: Vec<Object> = vec![
        Object  {
            posx: 5,
            posy: 5,
            symbol: "◎".to_string(),
            colision: false
        }
    ];

    let mut mutables: Vec<i32> = vec![
        0
    ];

    let mut playing = true;
    refresh_map(&display, map_size, &localplayer, &objects, &mutables);
    while playing {
        let key = display.getch();
        if key == Some(Input::Character('q')) {
            playing = false;
        }
        if key == Some(Input::KeyUp) {
            add_player_posy(false, &mut localplayer, &objects, map_size);
            refresh_map(&display, map_size, &localplayer, &objects, &mutables);
        }
        if key == Some(Input::KeyDown) {
            add_player_posy(true, &mut localplayer, &objects, map_size);
            refresh_map(&display, map_size, &localplayer, &objects, &mutables);
        }
        if key == Some(Input::KeyRight) {
            add_player_posx(true, &mut localplayer, &objects, map_size);
            refresh_map(&display, map_size, &localplayer, &objects, &mutables);
        }
        if key == Some(Input::KeyLeft) {
            add_player_posx(false, &mut localplayer, &objects, map_size);
            refresh_map(&display, map_size, &localplayer, &objects, &mutables);
        }
        if key == Some(Input::Character('f')) || key == Some(Input::Character('F')) {
            interact(&localplayer, &mut objects, &mut mutables);
            refresh_map(&display, map_size, &localplayer, &objects, &mutables);
        }
    }
    endwin();
}
