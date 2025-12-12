#ifndef CURSES_INPUT_H
#define CURSES_INPUT_H

#include "input.h"
#include <ncurses.h>

class CursesInput: public Input{
public:
    CursesInput(){
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
    }

    void waitForAnyKey() override {
        nodelay(stdscr, false);
        getch();
        nodelay(stdscr, false);
    }
    
    Action readAction() override {
        int ch;
        Action last = Action::NONE;

        // Read all pending keys
        while((ch = getch()) != ERR){
            switch(ch){
                case 'w': last = Action::UP; break;
                case 's': last = Action::DOWN; break;
                case 'a': last = Action::LEFT; break;
                case 'd': last = Action::RIGHT; break;
                case 'q': last = Action::INVALID; break;
                case KEY_UP:    last = Action::UP; break;
                case KEY_DOWN:  last = Action::DOWN; break;
                case KEY_LEFT:  last = Action::LEFT; break;
                case KEY_RIGHT: last = Action::RIGHT; break;
            }
        }
        return last;  // NONE if nothing pressed
    }
};

#endif
