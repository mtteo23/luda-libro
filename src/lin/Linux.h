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

#include <filesystem>

int readTitles(std::string titolo[])
{
	int i=0;
	for (const auto& entry : std::filesystem::directory_iterator(pBOOKS))
    {
		if (entry.is_regular_file() && entry.path().extension() == ".txt") 
        {
            titolo[i]=entry.path().filename();
            titolo[i]=titolo[i].substr(0, titolo[i].size()-4);
            i++;
        }
    }
    return i;
}

int loadAvailable(std::string dir, std::string ext, std::string available[], std::string &name)
{
	int i=0;
    bool tl=0;
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		if (entry.is_regular_file() && (entry.path().extension() == ext || ext==""))
		{
			available[i]=entry.path().filename();
			available[i]=available[i].substr(0, available[i].size()-ext.size());
			tl=tl||available[i]==name;
			i++;
		}
	}
	if(!tl && i>0) name=available[0];
	return i;
}
	
