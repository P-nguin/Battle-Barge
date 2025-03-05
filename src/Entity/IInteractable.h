#ifndef IINTERACTABLE_H
#define IINTERACTABLE_H

#include "HitBox/HitBox.h"

class IInteractable {
    public:
        virtual void interact() = 0;
        ~IInteractable() = default;
};

#endif