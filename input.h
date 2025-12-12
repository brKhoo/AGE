#ifndef INPUT_H
#define INPUT_H

#include "action.h"

class Input {
public:
    virtual ~Input() = default;
    virtual Action readAction() = 0;
    virtual void waitForAnyKey() = 0;
};

#endif
