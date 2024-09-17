#define pBOOKS "resources\\books\\"
#define pFONTS "resources\\fonts\\"
#define pGAMES "resources\\games\\"
#define pRULES "resources\\rules\\"
#define pLABELS "resources\\labels\\"
#define pSETTINGS "resources\\settings.txt"

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

#include <windows.h>

std::string executable_directory_path()
{
    std::vector<char> full_path_exe(MAX_PATH);

    for (;;)
    {
        const DWORD result = GetModuleFileName(NULL,
                                               &full_path_exe[0],
                                               full_path_exe.size());

        if (result == 0)
        {
            // Report failure to caller.
        }
        else if (full_path_exe.size() == result)
        {
            // Buffer too small: increase size.
            full_path_exe.resize(full_path_exe.size() * 2);
        }
        else
        {
            // Success.
            break;
        }
    } 

    // Remove executable name.
    std::string result(full_path_exe.begin(), full_path_exe.end());
    std::string::size_type i = result.find_last_of("\\/");
    if (std::string::npos != i) result.erase(i);

    return result;
}

int loadAvailable(std::string dir, std::string ext, std::string available[], std::string &name)
{	
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	
	int i=0;
    bool tl=0;
	string path=executable_directory_path()+dir+"*"+ext;
	if((hFind = FindFirstFile(path, &FindFileData)) != INVALID_HANDLE_VALUE){
		do{
			available[i]=FindFileData.cFileName;
			available[i]=available[i].substr(0, available[i].size()-ext.size());
			tl=tl||available[i]==name;
			i++;
		}while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	if(!tl && i>0) name=available[0];
	return i;
}
	
