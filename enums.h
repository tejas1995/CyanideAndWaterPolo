#ifndef ENUMS_H
#define ENUMS_H

enum PlayerCodes
{
    USER,
    COMPUTER
};

enum KeyCodes
{
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_SHIFT,
    KEY_K,
    KEY_L,
    KEY_TOTAL
};

enum Mode
{   NONE,
    WADE,
    SWIM,
    HIT
};

enum ButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP
};

#endif