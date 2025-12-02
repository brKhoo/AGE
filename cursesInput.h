#ifndef CURSES_INPUT_H
#define CURSES_INPUT_H

#include "input.h"
#include <ncurses.h>

class CursesInput : public Input {
public:
    CursesInput() {
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
    }

    Action readAction() override {
        int ch = getch();
        switch (ch) {
            case KEY_UP:    return Action::UP;
            case KEY_DOWN:  return Action::DOWN;
            case KEY_LEFT:  return Action::LEFT;
            case KEY_RIGHT: return Action::RIGHT;
            case 'w':       return Action::UP;
            case 's':       return Action::DOWN;
            case 'a':       return Action::LEFT;
            case 'd':       return Action::RIGHT;
            case 'q':       return Action::INVALID;
            case ERR:       return Action::NONE;
            default:        return Action::NONE;
        }
    }
};

#endif
