 #ifndef TOOLS_H
 #define TOOLS_H
 

#include <QImage>
#include <QWidget>

#include <SDL/SDL.h>
#undef main

/* PIEGE: main est red�fini par SDL.h comme SDL_Main.
		  On retire donc la d�finition puisque Qt le g�re
*/

 
 namespace SDLHelpers
 {
    template<typename T>
    class Singleton
    {
    public:
        static T& instance()
        {
            static T inst;
            return inst;
        }

    protected:
        Singleton(){}
        ~Singleton(){}
        Singleton(const Singleton &){}
        Singleton operator=(const Singleton &){}
    };
	 
	class SDLContext: public QWidget, public Singleton<SDLContext>
	{
        friend class Singleton<SDLContext>;
			
	public:
		// Permet d'acc�der en lecture seulement � la surface principale et sa configuration
		const SDL_Surface* const getScreen() const;
		
	private:
		// Initialise un contexte SDL. Dans le cas d'un �chec,
		// l'application est termin�e, avec la raison dans la sortie d'erreur
		SDLContext();
	
		// Lib�re le contexte SDL
		~SDLContext();
	
	private:
		SDL_Surface *screen;
	};
	
	// Cr�ation d'une surface SDL rapidement convertible en QImage
	SDL_Surface* createQImageCompliantSurface(Uint32 flags, int width, int height);
	
	/* Fonctions de conversion entre SDL_Surface et QImage
	    SDLSurfaceToQImage **DOIT** recevoir une surface
	    construite avec createQImageCompliantSurface
	*/
	QImage SDLSurfaceToQImage(SDL_Surface *s);
 }
 
 #endif
