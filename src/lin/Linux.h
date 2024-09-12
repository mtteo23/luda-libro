#define pBOOKS "resources/books/"
#define pFONTS "resources/fonts/"
#define pGAMES "resources/games/"
#define pRULES "resources/rules/"
#define pLABELS "resources/labels/"
#define pSETTINGS "resources/settings.txt"

#include <X11/Xlib.h>
sf::Vector2f getProportions()
{
	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	sf::Vector2f ScreenSize=sf::Vector2f(s->width, s->height-60);
	
	return sf::Vector2f(ScreenSize.x/1200.f, ScreenSize.y/600.f);
}
