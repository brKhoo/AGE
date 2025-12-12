#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "action.h"
#include <string>
#include <unordered_map>

//using StatusValue = variant<int, float, string, bool, char>;
using namespace std;

struct GameState {
    bool win = false;
    bool gameOver = false;
    int score = 0;
    Action lastInput = Action::NONE;
    // Initiate the 3 status lines
    string status1 = "0";
    string status2 = "0";
    string status3 = "0";

private:
    // Initiate the map that will hold the values
    unordered_map<string, int> statusMap;

    // Initial command to name the statuses
    void bindStatus(const string &name, int slot){
        if(slot >= 1 && slot <= 3){
            statusMap[name] = slot;
        }
    }

    string &getSlot(const string &name){
        int slot = statusMap[name]; // assumes name was bound
        if (slot == 1) return status1;
        if (slot == 2) return status2;
        return status3;
    }

public:
    friend class GameEngine;
    
    // Setters
    void setStatus(const string &name, int v) {
        getSlot(name) = to_string(v);
    }

    void setStatus(const string &name, float v) {
        getSlot(name) = to_string(v);
    }

    void setStatus(const string &name, bool v) {
        getSlot(name) = v ? "true" : "false";
    }

    void setStatus(const string &name, char v) {
        getSlot(name) = string(1, v);
    }

    void setStatus(const string &name, const string &v) {
        getSlot(name) = v;
    }

    // Helper to find the key for a slot to print it
    const string *getStatusKeyForSlot(int slot) const {
        for(const auto &p:statusMap){
            if(p.second == slot)
                return &p.first;
        }
        return nullptr;
    }
};

#endif
