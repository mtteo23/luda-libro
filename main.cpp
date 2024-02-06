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
int indPagina=0;
int AltezzaCarattere=20;
float MargA=100;
float MargS=300;
float LarghezzaPagina=LarghezzaSchermo-2*MargS;


void specSeqColore(char tipo, int arg, sf::Text* testoPagina, sf::RenderWindow* window)
{
    const sf::Color colore[5]={sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Black};
    switch(tipo)
    {
        case 'c':
            testoPagina->setFillColor(colore[arg]);
        break;

        case 'p':
            testoPagina->setFillColor(colore[0]);
        break;

        case 'l':
            testoPagina->setFillColor(colore[0]);
            testoPagina->setStyle(sf::Text::Underlined);
            {
                sf::Vector2f posMouse=sf::Vector2f(sf::Mouse::getPosition(*window));
                sf::FloatRect box=testoPagina->getGlobalBounds();
                if(box.contains(posMouse))
                {
                    testoPagina->setFillColor(colore[1]);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        indPagina=arg;
                    }
                }
            }

        break;
    }
}

void visualizza(sf::RenderWindow* window, string testo)
{
    bool specSeq=0;
    char tipo=0;
    int arg=0;
    float c=0;//colonna
    int r=0;//riga
    sf::Text testoPagina;
    testoPagina.setFont(font);
    testoPagina.setCharacterSize(AltezzaCarattere);
    testoPagina.setFillColor(sf::Color(255, 255, 255));

    for(unsigned int i=0; i<testo.size(); i++)
    {
        if(specSeq)
        {
            if(testo[i]=='/')
            {
                specSeq=0;
                arg=-1;
                tipo=0;
                testoPagina.setFillColor(sf::Color(255, 255, 255));
                testoPagina.setStyle(sf::Text::Regular);
            }
        }
        else
        {
            if(testo[i]=='/')
            {
                specSeq=1;
                tipo=testo[i+1];
                int k=testo.substr(i).find(" ");
                arg=stoi(testo.substr(i+2, k-2));
                i+=k+1;
            }

            if(testo[i]=='#')
            {
                specSeq=1;
                tipo='p';
                arg=0;
            }

            if(testo[i]=='\n' || c*AltezzaCarattere/2+150*(testo[i]==' ')>LarghezzaPagina)
            {
                r++;
                c=0;
            }
        }
        if(testo[i]!='\n' && testo[i]!='/' && testo[i]!='#' && !(testo[i]==' ' && c==0))
        {
            testoPagina.setPosition(MargS+(c*AltezzaCarattere/2), MargA+(r*AltezzaCarattere));
            c+=lCar[testo[i]];
            if(i>testo.size()-1) break;
            testoPagina.setString(testo.substr(i, 1));
            specSeqColore(tipo, arg, &testoPagina, window);
            window->draw(testoPagina);
        }
    }
}


/*


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
*/
string caricaLibro(string titolo)
{
    ifstream fin(titolo+".txt");
    string ret="";
    while(!fin.eof())
    {
        string buff;
        getline(fin, buff);
        ret+=buff+"\n";
    }
    fin.close();
    return ret;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(LarghezzaSchermo, AltezzaSchermo), "Display");
    window.setPosition(sf::Vector2i(-10, 0));
    string testo="#0/";
    string pagina[10000];
    int nPagine=0;

    for(int i=0; i<256; i++) lCar[i]=1;
    {
        lCar['\t']=2;
        lCar['\n']=0;
        lCar[',']=0.3;
        lCar['.']=0.4;

        lCar['a']=1.1;
        lCar['e']=1.1;
        lCar['i']=0.4;
        lCar['o']=1.1;
        lCar['u']=1.2;
        lCar['j']=0.4;

        lCar['f']=0.7;
        lCar['g']=1.2;
        lCar['l']=0.5;
        lCar['t']=0.7;
        lCar['r']=0.8;
        lCar['w']=1.8;
        lCar['m']=1.8;
        lCar['n']=1.2;

        lCar['A']=1.3;
        lCar['E']=1.3;
        lCar['I']=0.5;
        lCar['O']=1.3;
        lCar['U']=1.3;

        lCar['B']=1.3;
        lCar['C']=1.4;
        lCar['D']=1.4;
        lCar['F']=1.4;
        lCar['G']=1.5;
        lCar['H']=1.4;
        lCar['V']=1.5;
        lCar['R']=1.6;

    }

    if (!font.loadFromFile("arial.ttf"))
        cout<<"Errore";// error...

    testo=caricaLibro("Libro0");
    unsigned int inizio=0, fine=0, lunghezza=0;
    for(int i=0; fine!=testo.size()-1; i++)
    {
        inizio=testo.find("#"+to_string(i)+" ");
        fine=testo.find("#"+to_string(i+1)+" ");
        if(fine==string::npos)  fine=testo.size()-1;
        lunghezza=fine-inizio;
        pagina[i]=testo.substr(inizio, lunghezza);
        nPagine=i+1;
    }

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        visualizza(&window, pagina[indPagina]);
        window.display();
    }

    return 0;
}
