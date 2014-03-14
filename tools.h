 #ifndef TOOLS_H
 #define TOOLS_H
 

#include <QImage>
#include <QWidget>

#include <SDL/SDL.h>
#undef main

/* PIEGE: main est redéfini par SDL.h comme SDL_Main.
		  On retire donc la définition puisque Qt le gère
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
		// Permet d'accéder en lecture seulement à la surface principale et sa configuration
		const SDL_Surface* const getScreen() const;
		
	private:
		// Initialise un contexte SDL. Dans le cas d'un échec,
		// l'application est terminée, avec la raison dans la sortie d'erreur
		SDLContext();
	
		// Libère le contexte SDL
		~SDLContext();
	
	private:
		SDL_Surface *screen;
	};
	
	// Création d'une surface SDL rapidement convertible en QImage
	SDL_Surface* createQImageCompliantSurface(Uint32 flags, int width, int height);
	
	/* Fonctions de conversion entre SDL_Surface et QImage
	    SDLSurfaceToQImage **DOIT** recevoir une surface
	    construite avec createQImageCompliantSurface
	*/
	QImage SDLSurfaceToQImage(SDL_Surface *s);
 }
 
 #endif
