#include "cursesView.h"
#include "shape.h"
#include "movement.h"

using namespace std;

// Helper for formatting status lines
static void printStatusLine(int row, int slot, const GameState &state, const string &value){
    const string *key = state.getStatusKeyForSlot(slot);
    if(key)
        mvprintw(row, 0, "%s: %s", key->c_str(), value.c_str());
    else
        mvprintw(row, 0, "%s", value.c_str());
}

void CursesView::draw(const GameBoard &board, const GameState &state){
    clear();
    int rows = board.rows();
    int cols = board.cols();

    // Status lines reserved
    int playBottom = rows - 4;

    // Top and bottom border of play area
    for(int c = 0; c < cols; ++c){
        mvaddch(0, c, (c == 0 || c == cols - 1) ? '+' : '-');
        mvaddch(playBottom, c, (c == 0 || c == cols - 1) ? '+' : '-');
    }

    // Vertical borders
    for(int r = 1; r < playBottom; ++r){
        mvaddch(r, 0, '|');
        mvaddch(r, cols - 1, '|');
    }

    // Build draw list
    vector<Entity*> drawList;
    for(auto &up:board.entities()){
        drawList.push_back(up.get());
    }

    // Sort by height: lower first, higher last (allows higher entities to draw over lower ones)
    sort(drawList.begin(), drawList.end(),
        [](Entity *a, Entity *b){
            return a->height() < b->height();
        }
    );

    // Draw entities by height , so lower entities draw below higher ones
    for(Entity *e:drawList){
        Position base = e->position();
        for(auto px:e->shape().pixels()){
            int r = base.row + px.rowOffset;
            int c = base.col + px.colOffset;
            if(r > 0 && r < playBottom &&
                c > 0 && c < cols - 1){
                mvaddch(r, c, px.ch);
            }
        }
    }

    for(auto &e:board.entities()){
        if(e->tag() == "snake"){
            for(auto &m:e->movementComponents()){
                if(auto sm = dynamic_cast<SnakeMovement*>(m.get())){
                    const auto &body = sm->segments();
                    // Skip head, which is index 0
                    for(size_t i = 1; i < body.size(); ++i){
                        mvaddch(body[i].row, body[i].col, 'o');
                    }
                }
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
