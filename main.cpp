#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#define AltezzaSchermo 700
#define LarghezzaSchermo 1370

using namespace std;
///p=numero di pagina, c=cambia colore, l=link
sf::Font font;
float lCar[256];
int indPag=1;

void specSeqColore(char tipo, char arg, sf::Text* testoPagina, sf::RenderWindow* window)
{
    const sf::Color colore[5]={sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Black};
    switch(tipo)
    {
        case 'c':
            testoPagina->setFillColor(colore[arg-'a']);
        break;

        case 'p':
            testoPagina->setFillColor(colore[0]);
        break;

        case 'l':
            testoPagina->setFillColor(colore[0]);
            {
                sf::Vector2f posMouse=sf::Vector2f(sf::Mouse::getPosition(*window));
                sf::FloatRect box=testoPagina->getGlobalBounds();
                if(box.contains(posMouse))
                {
                    testoPagina->setFillColor(colore[1]);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        indPag=2;
                }
            }

        break;
    }
}

void visualizza(sf::RenderWindow* window, string testo)
{
    bool specSeq=0;
    char tipo=0, arg=0;
    float c=0;//colonna
    int r=0;//riga
    float margA=100;
    float margS=300;
    sf::Text testoPagina;
    testoPagina.setFont(font);
    testoPagina.setCharacterSize(32);
    testoPagina.setFillColor(sf::Color(255, 255, 255));

    for(int i=0; i<testo.size(); i++)
    {
        if(specSeq)
        {
            if(testo[i]=='/')
            {
                specSeq=0;
                arg=0;
                tipo=0;
                testoPagina.setFillColor(sf::Color(255, 255, 255));
            }
            else
            {
                if(arg!=0 && tipo!=0)
                {
                    testoPagina.setPosition(margS+(c*16), margA+(r*32));
                    specSeqColore(tipo, arg, &testoPagina, window);
                    c+=lCar[testo[i]];
                    testoPagina.setString(testo.substr(i, 1));
                    window->draw(testoPagina);
                }
                else
                {
                    if(tipo!=0)
                    {
                        arg=testo[i];
                        specSeqColore(tipo, arg, &testoPagina, window);
                    }
                    else
                    {
                        tipo=testo[i];
                    }
                }
            }
        }
        else
        {
            if(testo[i]=='/')
            {
                specSeq=1;
            }
            if(testo[i]=='#')
            {
                specSeq=1;
                tipo='p';
                arg='z';
            }

            if(testo[i]=='\n' || c>20)
            {
                r++;
                c=0;
            }

            if(testo[i]!='\n' && !specSeq)
            {
                testoPagina.setPosition(margS+(c*16), margA+(r*32));
                c+=lCar[testo[i]];
                testoPagina.setString(testo.substr(i, 1));
                window->draw(testoPagina);
            }
        }
    }

}
string caricaPag(int indPagina)
{
    ifstream fin("pag"+to_string(indPagina)+".txt");
    string ret="";
    while(!fin.eof())
    {
        string buff;
        getline(fin, buff);
        ret+=buff+"\n";
    }
    return ret;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(LarghezzaSchermo, AltezzaSchermo), "Display");
    window.setPosition(sf::Vector2i(-10, 0));
    string testo="#0/";

    for(int i=0; i<256; i++) lCar[i]=1;
    {
        lCar['\t']=2;
        lCar['\n']=0;

        lCar['a']=1;
        lCar['e']=1.1;
        lCar['i']=0.6;
        lCar['o']=1.1;
        lCar['u']=0.6;

        lCar['l']=0.5;
        lCar['t']=0.8;
        lCar['r']=0.8;
        lCar['w']=1.8;
        lCar['m']=1.8;

        lCar['C']=1.2;
    }


    if (!font.loadFromFile("arial.ttf"))
        cout<<"Errore";// error...

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        if(indPag!=stoi(testo.substr(1, 1)))
        {
            testo=caricaPag(indPag);
        }
        visualizza(&window, testo);
        window.display();
    }

    return 0;
}
