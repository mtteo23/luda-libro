#define pBOOKS "resources\books\"
#define pFONTS "resources\fonts\"
#define pGAMES "resources\games\"
#define pRULES "resources\rules\"
#define pLABELS "resources\labels\"
#define pSETTINGS "resources\settings.txt"

#include "wtypes.h"
sf::Vector2f getProportions()
{
	RECT desktop;
   	// Get a handle to the desktop window
   	const HWND hDesktop = GetDesktopWindow();
   	// Get the size of screen to the variable desktop
   	GetWindowRect(hDesktop, &desktop);
   	// The top left corner will have coordinates (0,0)
   	// and the bottom right corner will have coordinates
   	// (horizontal, vertical)
   	horizontal = desktop.right;
   	vertical = desktop.bottom;
	sf::Vector2f ScreenSize=sf::Vector2f(horizontal, vertical-60);
	
	return sf::Vector2f(ScreenSize.x/1200.f, ScreenSize.y/600.f);
}
