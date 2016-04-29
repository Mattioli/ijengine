#include "sdl2kernel.h"
#include "sdl2window.h"
#include "exception.h"
#include "event.h"
#include "system_event.h"
#include "keyboard_event.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace ijengine;


SDL2Kernel::SDL2Kernel()
{
    int rc = SDL_Init(SDL_INIT_VIDEO);

    if (rc)
        throw Exception("Error on SDL2Kernel()");
}

SDL2Kernel::~SDL2Kernel()
{
    if (SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Quit();
}

Window *
SDL2Kernel::create_window(const string& title, int w, int h)
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer))
        return nullptr;

    SDL_SetWindowTitle(window, title.c_str());

    return new SDL2Window(window, renderer);
}

list<Event *>
SDL2Kernel::pending_events(unsigned now)
{
    SDL_Event event;
    list<Event *> events;

    SDL_PumpEvents();

    while (SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0)
    {
        unsigned timestamp = event.quit.timestamp;

        if (timestamp > now)
            break;

        SDL_PollEvent(&event);
        
        switch (event.type) {
        case SDL_QUIT:
            {
                auto p = new SystemEvent(timestamp, SystemEvent::Action::QUIT);

                if (p)
                {
                    events.push_back(p);
printf("SystemEvent added on %u\n", timestamp);
                }
            }

            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            {
                auto p = new KeyboardEvent(timestamp,
                    KeyboardEvent::State::PRESSED,
                    KeyboardEvent::Key::ESCAPE,
                    KeyboardEvent::Modifier::NONE);

                if (p)
                {
                    events.push_back(p);
printf("KeyboardEvent added on %u\n", timestamp);
                }
            }
            break;

        default:
            break;
        }

        SDL_PumpEvents();
    }

    return events;
}

