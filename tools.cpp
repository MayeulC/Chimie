#include "tools.h"

SDLHelpers::SDLContext::SDLContext()
:screen(0)
{
	char windowid[64] = {0};
#ifdef Q_WS_WIN
	sprintf(windowid, "SDL_WINDOWID=0x%lx", reinterpret_cast<qlonglong>(winId()));
#elif defined Q_WS_X11
	sprintf(windowid, "SDL_WINDOWID=0x%lx", winId());
#else
        qFatal("Fatal: impossible de caster winId() pour votre système!");
#endif
	SDL_putenv(windowid);

        SDL_Init(SDL_INIT_VIDEO);
        //screen = SDL_SetVideoMode(width(), height(), 32, SDL_SWSURFACE);
        screen = SDL_SetVideoMode(width(), height(), 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	if(!screen)
            qFatal("In SDLContext::SDLContext(): SDL_SetVideoMode failed\n%s",SDL_GetError());
}

SDLHelpers::SDLContext::~SDLContext()
{
	SDL_Quit();
}

const SDL_Surface* const SDLHelpers::SDLContext::getScreen() const
{
	return screen;
}

SDL_Surface* SDLHelpers::createQImageCompliantSurface(Uint32 flags, int width, int height)
{
	static const int Rmask = 0x00FF0000;
	static const int Gmask = 0x0000FF00;
	static const int Bmask = 0x000000FF;
	static const int Amask = 0xFF000000;
	
	return SDL_CreateRGBSurface(flags, width, height, 32, Rmask, Gmask, Bmask, Amask);
}

QImage SDLHelpers::SDLSurfaceToQImage(SDL_Surface *s)
{
	SDL_LockSurface(s);
		QImage im(static_cast<uchar*>(s->pixels), s->w, s->h, QImage::Format_RGB32);
        SDL_UnlockSurface(s);
	
	return im;
}
