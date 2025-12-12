#include "cursesView.h"
#include "shape.h"

using namespace std;

// Helper for formatting status lines
static void printStatusLine(int row, int slot, const GameState &state, const string &value){
    const string *key = state.getStatusKeyForSlot(slot);
    if(key)
        mvprintw(row, 0, "%s: %s", key->c_str(), value.c_str());
    else
        mvprintw(row, 0, "%s", value.c_str());
}


void CursesView::draw(const GameBoard &board, const GameState &state) {
    clear();

    int rows = board.rows();
    int cols = board.cols();

    // Play area: bordered rectangle with 3 status lines at bottom:
    // Rows: 0..rows-1, status at rows-3, rows-2, rows-1
    int playBottom = rows - 4;

    // Top and bottom border of play area
    for (int c = 0; c < cols; ++c) {
        mvaddch(0, c, (c == 0 || c == cols - 1) ? '+' : '-');
        mvaddch(playBottom, c, (c == 0 || c == cols - 1) ? '+' : '-');
    }

    // Vertical borders
    for (int r = 1; r < playBottom; ++r) {
        mvaddch(r, 0, '|');
        mvaddch(r, cols - 1, '|');
    }

    // Draw entities
    for (auto &up : board.entities()) {
        Entity *e = up.get();
        Position base = e->position();
        for (auto px : e->shape().pixels()) {
            int r = base.row + px.rowOffset;
            int c = base.col + px.colOffset;
            if (r > 0 && r < playBottom &&
                c > 0 && c < cols - 1) {
                mvaddch(r, c, px.ch);
            }
        }
    }

    // Status lines
    int status1 = playBottom + 1;
    int status2 = playBottom + 2;
    int status3 = playBottom + 3;

    // Print the three status slots
    printStatusLine(status1, 1, state, state.status1);
    printStatusLine(status2, 2, state, state.status2);
    printStatusLine(status3, 3, state, state.status3);

    refresh();
}
