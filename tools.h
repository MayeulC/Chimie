 #ifndef TOOLS_H
 #define TOOLS_H
 

#include <QImage>
#include <QWidget>

#include <SDL/SDL.h>
#undef main

/* IT'S A TRAP! main is redefined by SDL.h as SDL_Main. So we undefine it here */
 
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
		//allows us to access read-only to the main surface and its configuration
		const SDL_Surface* const getScreen() const;
		
	private:
		// Initialize a SDL context. If it fails, app is terminated, with an error in the output
		SDLContext();
	
		// Free the SDL context
		~SDLContext();
	
	private:
		SDL_Surface *screen;
	};
	
	// Creating a SDL surface that can be rapidly converted to a QImage
	SDL_Surface* createQImageCompliantSurface(Uint32 flags, int width, int height);
	
	/* Convert functions between SDL_Surface and QImage.
	 * SDLSurfaceToQImage **MUST** receive a surface 
	 * constructed using createQImageCompliantSurface 
	 */
	
	QImage SDLSurfaceToQImage(SDL_Surface *s);
 }
 
 #endif
