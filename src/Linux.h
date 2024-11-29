/*
   ludlibrilo, a program to run gamebooks
   Copyright (C) 2024 Matteo Pascali (makte0)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/


#define pBOOKS "resources/books/"
#define pFONTS "resources/fonts/"
#define pGAMES "resources/games/"
#define pRULES "resources/rules/"
#define pLABELS "resources/labels/"
#define pCREDITS "resources/credits/"
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
	
