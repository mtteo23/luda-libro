#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <fstream>
#include <iostream>

#include <locale>
#include <codecvt>
#include <string>

#include <filesystem>

sf::Vector2f operator*(const sf::Vector2f& left, const sf::Vector2f& right) {
    return sf::Vector2f(left.x * right.x, left.y * right.y);
}

sf::Vector2f ScreenSize=sf::Vector2f(1200.f, 600.f);
sf::Vector2f MarginSize=sf::Vector2f(300.f, 100.f);
sf::Vector2f prop=sf::Vector2f(ScreenSize.x/1200.f, ScreenSize.y/600.f);

float MargA;
float MargS;

using namespace std;




wstring Str2Wstr(const string& str)
{
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

class Settings{
    public:
    
    string language="English";
    string availableLang[50]={};
    
    string fontName="arial.ttf";
    string availableFont[50]={};
    
    int volume=50;
    sf::Color colore[3]=
    {sf::Color::Black, sf::Color::White, sf::Color::Red};
    sf::Font font;
    
    int mainCharSize=23;
    int labelCharSize=30;

    void scarica()
    {
        ifstream fin("settings.txt");

        getline(fin, language);
        getline(fin, fontName);
        fin>>volume;
        for(int i=0; i<3; i++)
        {
            int a, b, c;
            fin>>a;
            fin>>b;
            fin>>c;
            colore[i]=sf::Color(a, b, c);
        }
        fin.close();
        
        int i=0;
        bool tl=0;
		for (const auto& entry : filesystem::directory_iterator("./labels")) //portability issue
		{
			if (entry.is_regular_file() && entry.path().extension() == ".txt") 
			{
				availableLang[i]=entry.path().filename();
				availableLang[i]=availableLang[i].substr(0, availableLang[i].size()-4);
				tl=tl||availableLang[i]==language;
				i++;
			}
		}
		if(!tl) language=availableLang[0];
		
		i=0;
		bool tF=0;
		for (const auto& entry : filesystem::directory_iterator("./fonts")) //portability issue
		{
			if (entry.is_regular_file()) 
			{
				availableFont[i]=entry.path().filename();
				tF=tF||availableFont[i]==fontName;
				i++;
			}
		}
		if(!tF) fontName=availableFont[0];
		
        font.loadFromFile("fonts/"+fontName);//portability issue
    }

    void salva()
    {
        ofstream fout("settings.txt");

        fout<<language<<endl;
        fout<<fontName<<endl;
        fout<<volume<<endl;

        for(int i=0; i<3; i++)
        {
            fout<<to_string(colore[i].r)<<" ";
            fout<<to_string(colore[i].g)<<" ";
            fout<<to_string(colore[i].b)<<endl;
        }
        fout.close();
    }

}settings;


class Pulsante
{
    public:
    bool *pushed;
    int valore;
    int stato=0;
    wstring etic=L"";

    sf::Vector2f pos{0.f, 0.f};
    sf::Vector2f larg{60.f, 60.f};
    sf::Vector2f diff{3.f, 3.f};

    sf::RectangleShape Layer1;
    sf::RectangleShape Layer2;
    sf::RectangleShape Layer3;

    sf::Text etichetta;

    Pulsante(wstring inpEtic, float x, float y, float k, bool *inpPushed)
    {
        pushed=inpPushed;

        etic=inpEtic;

        pos=sf::Vector2f(x, y)*prop;
        larg=sf::Vector2f(larg.y*k, larg.y)*prop;
        diff=diff*prop;

        Layer1.setSize(larg);
        Layer1.setPosition(pos);

        Layer2.setSize(larg-2.f*diff);
        Layer2.setPosition(pos+diff);

        Layer3.setSize(larg-4.f*diff);
        Layer3.setPosition(pos+2.f*diff);

        etichetta.setFont(settings.font);
        etichetta.setCharacterSize(settings.labelCharSize);
        etichetta.setString(etic);
        sf::Vector2f posLabel=pos+4.f*diff;
        {float width=etichetta.getGlobalBounds().width; posLabel.x+=(larg.x-8.f*diff.x-width)/2.f;}
        etichetta.setPosition(posLabel);
    }

    int state(sf::RenderWindow* window) //0 normale, 1 puntatore mouse, 2 premuto, 3 rilasciato
    {
		if(stato==-1) return -1;
        if(Layer1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                *pushed=1;
                return 2;
            }
            else
            {
                if(*pushed)
                {
                    *pushed=0;
                    return 3;
                }
                else return 1;
            }
        }
        else return 0;
    }

    int draw(sf::RenderWindow* window)
    {
        stato=state(window);
        switch(stato)
        {
			
            case -1:
				
                Layer1.setFillColor(settings.colore[0]-sf::Color(0, 0, 0, 128));
                Layer2.setFillColor(settings.colore[1]-sf::Color(0, 0, 0, 128));
                Layer3.setFillColor(settings.colore[0]);
                etichetta.setFillColor(settings.colore[1]-sf::Color(0, 0, 0, 128));
            break;

            case 0:
                Layer1.setFillColor(settings.colore[0]);
                Layer2.setFillColor(settings.colore[1]);
                Layer3.setFillColor(settings.colore[0]);
                etichetta.setFillColor(settings.colore[1]);
            break;

            case 1:
                Layer1.setFillColor(settings.colore[1]);
                Layer2.setFillColor(settings.colore[0]);
                Layer3.setFillColor(settings.colore[1]);
                etichetta.setFillColor(settings.colore[0]);
            break;

            case 2:
                Layer1.setFillColor(settings.colore[1]);
                Layer2.setFillColor(settings.colore[0]);
                Layer3.setFillColor(settings.colore[1]);
                etichetta.setFillColor(settings.colore[0]);
            break;

            case 3:
                Layer1.setFillColor(settings.colore[0]);
                Layer2.setFillColor(settings.colore[1]);
                Layer3.setFillColor(settings.colore[0]);
                etichetta.setFillColor(settings.colore[1]);
            break;
        }
        window->draw(Layer1);
        window->draw(Layer2);
        window->draw(Layer3);
        etichetta.setString(etic);
        window->draw(etichetta);
		
        return stato;
    }
    
    void drawManual(sf::RenderWindow* window)
    {
        switch(stato)
        {
            case 0:
                Layer1.setFillColor(settings.colore[0]);
                Layer2.setFillColor(settings.colore[1]);
                Layer3.setFillColor(settings.colore[0]);
                etichetta.setFillColor(settings.colore[1]);
            break;

            case 1:
                Layer1.setFillColor(settings.colore[1]);
                Layer2.setFillColor(settings.colore[0]);
                Layer3.setFillColor(settings.colore[1]);
                etichetta.setFillColor(settings.colore[0]);
            break;

            case 2:
                Layer1.setFillColor(settings.colore[1]);
                Layer2.setFillColor(settings.colore[0]);
                Layer3.setFillColor(settings.colore[1]);
                etichetta.setFillColor(settings.colore[0]);
            break;

            case 3:
                Layer1.setFillColor(settings.colore[0]);
                Layer2.setFillColor(settings.colore[1]);
                Layer3.setFillColor(settings.colore[0]);
                etichetta.setFillColor(settings.colore[1]);
            break;
        }
        window->draw(Layer1);
        window->draw(Layer2);
        window->draw(Layer3);
        etichetta.setString(etic);
        window->draw(etichetta);
    }
};

class Barra
{
    public:
    float k;
    int var;
    int range;
    wstring etic=L"";

    sf::Vector2f pos{0.f, 0.f};
    sf::Vector2f larg{60.f, 60.f};
    sf::Vector2f diff{3.f, 3.f};

    sf::RectangleShape Layer1;
    sf::RectangleShape Layer2;
    sf::RectangleShape Layer3;

    sf::Text etichetta;

    Barra(wstring inpEtic, float x, float y, float K, int varControllata, int inpRange)
    {

        var=varControllata;

        etic=Str2Wstr(to_string(var))+inpEtic;

        range=inpRange;

        k=K;

		
        pos=sf::Vector2f(x, y)*prop;
        larg=sf::Vector2f(larg.y*k, larg.y)*prop;
        diff=diff*prop;

        Layer1.setSize(larg);
        Layer1.setPosition(pos);

        Layer2.setSize(larg-2.f*diff);
        Layer2.setPosition(pos+diff);

        Layer3.setSize(larg-4.f*diff);
        Layer3.setPosition(pos+2.f*diff);

        etichetta.setFont(settings.font);
        etichetta.setPosition(pos+4.f*diff);
        etichetta.setCharacterSize(settings.labelCharSize);

        Layer1.setFillColor(settings.colore[1]);
        Layer2.setFillColor(settings.colore[0]);
        Layer3.setFillColor(settings.colore[1]);
        etichetta.setFillColor(settings.colore[0]);
    }


    int draw(sf::RenderWindow* window)
    {

        if((Layer1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))&&(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
            var=(((sf::Vector2f(sf::Mouse::getPosition(*window))-(pos+2.f*diff)).x)*range)/((60.f*k-4.f*diff.x)*prop.x);

        var=max(0, var);
        var=min(range, var);
        Layer3.setSize(sf::Vector2f((60.f*k-4.f*diff.x)*var/range*prop.x, larg.y-4.f*diff.y));

        etichetta.setString(etic);
        window->draw(Layer1);
        window->draw(Layer2);
        window->draw(Layer3);
        window->draw(etichetta);
        return var;
    }
};


