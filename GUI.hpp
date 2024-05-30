#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <fstream>
#include <iostream>

#define AltezzaSchermo 700
#define LarghezzaSchermo 1370

using namespace std;



float prop=AltezzaSchermo/700;

class Impostazioni{
    public:
    string lingvo="Italiano";
    string lingue_disponibili[50]={"English", "Esperanto", "Italiano"};
    int volume=50;
    sf::Color colore[3]=
    {sf::Color::Black, sf::Color::White, sf::Color(200, 200, 200)};

    sf::Font font;
    int AltezzaCarattere=24;

    void scarica()
    {
        ifstream fin("impostazioni.txt");

        getline(fin, lingvo);
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

        fin.open("tradukajxoj\\enhavo.txt");
        for(int i=0; i<50 && !fin.eof(); i++)
            getline(fin, lingue_disponibili[i]);
        fin.close();

        if (!font.loadFromFile("arial.ttf"))
            cout<<"Errore: font non trovato";
    }

    void salva()
    {
        ofstream fout("impostazioni.txt");

        fout<<lingvo<<endl;
        fout<<volume<<endl;

        for(int i=0; i<3; i++)
        {
            fout<<to_string(colore[i].r)<<" ";
            fout<<to_string(colore[i].g)<<" ";
            fout<<to_string(colore[i].b)<<endl;
        }
        fout.close();
    }

}impozado;


class Pulsante
{
    public:
    bool *pushed;
    int valore;
    int stato=0;
    string etic="";

    sf::Vector2f pos{0.f, 0.f};
    sf::Vector2f larg{60.f, 60.f};
    sf::Vector2f diff{3.f, 3.f};

    sf::RectangleShape Layer1;
    sf::RectangleShape Layer2;
    sf::RectangleShape Layer3;

    sf::Text etichetta;

    Pulsante(string inpEtic, float x, float y, float k, bool *inpPushed)
    {
        pushed=inpPushed;

        etic=inpEtic;

        pos=sf::Vector2f(x, y)*prop;
        larg=sf::Vector2f(larg.y*k, larg.y)*prop;
        diff*=prop;

        Layer1.setSize(larg);
        Layer1.setPosition(pos);

        Layer2.setSize(larg-2.f*diff);
        Layer2.setPosition(pos+diff);

        Layer3.setSize(larg-4.f*diff);
        Layer3.setPosition(pos+2.f*diff);

        etichetta.setFont(impozado.font);
        etichetta.setPosition(pos+4.f*diff);
        etichetta.setCharacterSize(impozado.AltezzaCarattere+5);
    }

    int state(sf::RenderWindow* window) //0 normale, 1 puntatore mouse, 2 premuto, 3 rilasciato
    {
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
            case 0:
                Layer1.setFillColor(impozado.colore[0]);
                Layer2.setFillColor(impozado.colore[1]);
                Layer3.setFillColor(impozado.colore[0]);
                etichetta.setFillColor(impozado.colore[1]);
            break;

            case 1:
                Layer1.setFillColor(impozado.colore[1]);
                Layer2.setFillColor(impozado.colore[0]);
                Layer3.setFillColor(impozado.colore[1]);
                etichetta.setFillColor(impozado.colore[0]);
            break;

            case 2:
                Layer1.setFillColor(impozado.colore[1]);
                Layer2.setFillColor(impozado.colore[0]);
                Layer3.setFillColor(impozado.colore[1]);
                etichetta.setFillColor(impozado.colore[0]);
            break;

            case 3:
                Layer1.setFillColor(impozado.colore[0]);
                Layer2.setFillColor(impozado.colore[1]);
                Layer3.setFillColor(impozado.colore[0]);
                etichetta.setFillColor(impozado.colore[1]);
            break;
        }
        etichetta.setString(etic);
        window->draw(Layer1);
        window->draw(Layer2);
        window->draw(Layer3);
        window->draw(etichetta);

        return stato;
    }
};

class Barra
{
    public:
    float k;
    int var;
    int range;
    string etic="";

    sf::Vector2f pos{0.f, 0.f};
    sf::Vector2f larg{60.f, 60.f};
    sf::Vector2f diff{3.f, 3.f};

    sf::RectangleShape Layer1;
    sf::RectangleShape Layer2;
    sf::RectangleShape Layer3;

    sf::Text etichetta;

    Barra(string inpEtic, float x, float y, float K, int varControllata, int inpRange)
    {

        var=varControllata;

        etic=to_string(var)+inpEtic;

        range=inpRange;

        k=K;

        pos=sf::Vector2f(x, y)*prop;
        larg=sf::Vector2f(larg.y*k, larg.y)*prop;
        diff*=prop;

        Layer1.setSize(larg);
        Layer1.setPosition(pos);

        Layer2.setSize(larg-2.f*diff);
        Layer2.setPosition(pos+diff);

        Layer3.setSize(larg-4.f*diff);
        Layer3.setPosition(pos+2.f*diff);

        etichetta.setFont(impozado.font);
        etichetta.setPosition(pos+4.f*diff);
        etichetta.setCharacterSize(impozado.AltezzaCarattere+5);

        Layer1.setFillColor(impozado.colore[1]);
        Layer2.setFillColor(impozado.colore[0]);
        Layer3.setFillColor(impozado.colore[1]);
        etichetta.setFillColor(impozado.colore[0]);
    }


    int draw(sf::RenderWindow* window)
    {

        if((Layer1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))&&(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
            var=((((sf::Vector2f(sf::Mouse::getPosition(*window))-(pos+2.f*diff)).x)*range)/((60.f*k-4.f*diff.x)*prop));

        var=max(0, var);
        var=min(range, var);
        Layer3.setSize(sf::Vector2f((60.f*k-4.f*diff.x)*var/range*prop, larg.y-4.f*diff.y));

        etichetta.setString(etic);
        window->draw(Layer1);
        window->draw(Layer2);
        window->draw(Layer3);
        window->draw(etichetta);
        return var;
    }
};


