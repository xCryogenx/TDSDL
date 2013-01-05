#ifndef SPRITE_H
#define SPRITE_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <QString>

class Sprite
{
private:
    SDL_Surface* surface;
    int x, y;
    QString path;
public:
    Sprite();
    ~Sprite();
    SDL_Surface* Load(QString path);
    bool Draw(SDL_Surface* dest, int x, int y);
};

#endif // SPRITE_H
