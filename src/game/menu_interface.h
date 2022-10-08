#ifndef EVIL_CAT_GAME_MENU_INTERFACE_H
#define EVIL_CAT_GAME_MENU_INTERFACE_H

class MenuInterface {
public:
    virtual void show() = 0;

    virtual ~MenuInterface() = default;
};

#endif //EVIL_CAT_GAME_MENU_INTERFACE_H
