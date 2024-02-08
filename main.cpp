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
//Paragrafo A(normale) Link Colore Nota Oggetto
sf::Font font;
int AltezzaCarattere=20;
float MargA=100;
float MargS=300;
float LarghezzaPagina=LarghezzaSchermo-2*MargS;

struct Sezione
{
    string testo="";//principale, a capo
    int lung=0;
    sf::Vector2f posizione;
    sf::Color colore=sf::Color::White;
    int arg=0;
    char tipo='a';
};

void visualizza(sf::RenderWindow* window, Sezione sezione[], int nSez)
{
    sf::Text testoPagina;
    testoPagina.setFont(font);
    testoPagina.setCharacterSize(AltezzaCarattere);

    for(int i=0; i<nSez; i++)
    {
        testoPagina.setString(sezione[i].testo);
        testoPagina.setFillColor(sezione[i].colore);
        testoPagina.setPosition(sezione[i].posizione+sf::Vector2f(MargS, MargA));
        window->draw(testoPagina);
    }
}

int disseziona(string testo, Sezione sezione[])
{
    float lC[256]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.2, 1.2, 1.1, 1.1, 2, 1.3, 1.3, 1.4, 1.4, 1.4, 1.2, 1.5, 1.4, 0.5, 1, 1.3, 1.1, 1.6, 1.4, 1.5, 1.2, 1.5, 1.4, 1.3, 1.2, 1.5, 1.3, 1.9, 1.3, 1.3, 1.2, 0.5, 0.5, 0.5, 0.9, 1.1, 0.7, 1, 1, 1, 1.1, 1.1, 0.6, 1.1, 1.1, 0.4, 0.4, 1, 0.4, 1.6, 1.1, 1.1, 1.1, 1, 0.7, 0.9, 0.6, 1.1, 1, 1.4, 1, 1, 1, 0.7, 0.5, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    float c=0;
    int r=0;
    int indSez=0;
    for(int i=0; i<100; i++)
    {
        sezione[i].testo="";
        sezione[i].arg=0;
        sezione[i].tipo='a';
        sezione[i].colore=sf::Color::White;
        sezione[i].posizione=sf::Vector2f(0.f, 0.f);
        sezione[i].lung=0;
    }
    for(unsigned int i=0; i<testo.size(); i++)
    {
        if(testo[i]=='/' || testo[i]=='#')
        {
            if(sezione[indSez].lung>0 || sezione[indSez].tipo!='a')
                indSez++;
            if(testo[i]=='/')
            {
                i++;
                if(testo[i]==' ' || testo[i]=='\n' || testo[i]==',' || testo[i]=='.' || testo[i]==':' || testo[i]==';')
                {
                    sezione[indSez].tipo='a';
                    sezione[indSez].arg=0;
                    sezione[indSez].colore=sf::Color::White;
                    sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2, r*AltezzaCarattere);
                    i--;
                }
                else//speciale
                {
                    sezione[indSez].tipo=testo[i];
                    i++;
                    int k=testo.substr(i).find(" ");
                    sezione[indSez].arg=stoi(testo.substr(i, k));
                    i+=k;
                    sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2, r*AltezzaCarattere);
                    sezione[indSez].colore=sf::Color::Red;
                }
            }
            else//#
            {
                i++;
                sezione[indSez].tipo='p';
                int k=testo.substr(i).find(" ");
                sezione[indSez].arg=stoi(testo.substr(i, k));
                i+=k;
                sezione[indSez].colore=sf::Color::Red;
                sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2, r*AltezzaCarattere);
            }
        }
        else
        {
            int lungProsParola=testo.substr(i+1).find(" ");
            if(testo[i]=='\n' || (c+lungProsParola)*AltezzaCarattere/2>LarghezzaPagina)
            {
                c=0;
                r++;
                if(sezione[indSez].tipo=='a' || 1)
                {
                    indSez++;
                    sezione[indSez].tipo='a';
                    sezione[indSez].arg=0;
                    sezione[indSez].colore=sf::Color::White;
                    sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2, r*AltezzaCarattere);
                }
                else
                {
                    //Work in progress
                }
            }
            else
            {
                sezione[indSez].testo+=testo.substr(i, 1);
                sezione[indSez].lung+=lC[testo[i]];
                c+=lC[testo[i]];
            }
        }
    }
    return indSez+1;
}

bool azionaIpertesto(Sezione sezione[], int nSez, sf::Vector2f mouse, int &indPagina)
{
    bool cPag=0;
    for(int i=0; i<nSez; i++)
    {
        if(sezione[i].tipo=='l')
        {
            if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y, sezione[i].lung*AltezzaCarattere/2, AltezzaCarattere).contains(mouse))
            {
                sezione[i].colore=sf::Color::Green;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    indPagina=sezione[i].arg;
                    cPag=1;
                }
            }
            else
            {
                sezione[i].colore=sf::Color::Red;
            }
        }
    }
    return cPag;
}

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
    int indPagina=0;
    int cronoPagine[100000];
    int indCP=0, nCP=0;

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
    Sezione sezione[100];
    int nSez=disseziona(pagina[indPagina], sezione);

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        if(azionaIpertesto(sezione, nSez, sf::Vector2f(sf::Mouse::getPosition(window))-sf::Vector2f(MargS, MargA), indPagina))
        {
            visualizza(&window, sezione, nSez);
            nSez=disseziona(pagina[indPagina], sezione);
        }
        else
            visualizza(&window, sezione, nSez);
        window.display();
    }

    return 0;
}
