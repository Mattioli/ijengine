#ifndef IJENGINE_SDL2CANVAS_H
#define IJENGINE_SDL2CANVAS_H

#include <SDL2/SDL.h>
#include "canvas.h"
#include "rectangle.h"

using namespace ijengine;


class SDL2Canvas : public Canvas {
public:
    SDL2Canvas(SDL_Renderer *renderer);

    void draw(const Texture *texture, int x, int y);
    void draw(const Texture *texture, const Rectangle& section, int x, int y);

    void set_color(unsigned char r, unsigned char g, unsigned char b,
        unsigned char a);

    void drawRect(Rectangle rectangle, int r, int g, int b);
    void drawPoint(Point point, int r, int g, int b);
    void drawLine(Line line, int r, int g, int b);

    void clear();
    void update();

    SDL_Renderer * renderer() const;

private:
    SDL_Renderer *m_renderer;
};

#endif
