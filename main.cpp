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
int AltezzaCarattere=24;
float MargA=100;
float MargS=300;
float LarghezzaPagina=LarghezzaSchermo-2*MargS;
float prop=AltezzaSchermo/700;

int cronoPagine[10000]={0};
int indCP=0, nCP=0;

enum {home, gioca, impostazioni};

struct Sezione
{
    string testo="";//principale, a capo
    float lung=0;
    sf::Vector2f posizione;
    sf::Color colore=sf::Color::White;
    sf::Text::Style stile=sf::Text::Regular;
    int arg=0;
    char tipo='a';
};


///./////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int collassa(unsigned char poss)
{
    int c=0;
    for(int i=0; i<8; i++)  c+=poss>>i&1;
    if(c>1)     return 0;
    if(c==1)    for(int i=0; i<8; i++)  if(poss>>i&1) return i+1;
    return -1;
}
void caricaTabAdatto(bool adatto[8][4][8])
{
    bool caratt[8][4]={{1, 0, 1, 0}, {0, 1, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 0, 0}, {1, 0, 0, 1}, {0, 1, 1, 0}, {0, 0, 1, 1}};
    for(int i=0; i<8; i++)  for(int j=0; j<4; j++)  for(int k=0; k<8; k++)  adatto[i][j][k]=(caratt[i][j]==caratt[k][(j+2)%4]);
}
bool riduci(unsigned char poss[100][100], int r, int c, int x, int y, bool adatto[8][4][8], bool first)
{
    bool camb=0;
    bool errore=0;
    for(int i=0; i<8; i++)
    {
        bool dir[4]={0, 0, 0, 0};
        if(y>0)
        {
            for(int k=0; k<8; k++)
            {
                if(poss[y-1][x]>>k&1)
                    dir[0]=dir[0]||adatto[i][0][k];
            }
        }
        else    dir[0]=1;
        if(x<c-1)
        {
            for(int k=0; k<8; k++)
            {
                if(poss[y][x+1]>>k&1)
                    dir[1]=dir[1]||adatto[i][1][k];
            }
        }
        else    dir[1]=1;
        if(y<r-1)
        {
            for(int k=0; k<8; k++)
            {
                if(poss[y+1][x]>>k&1)
                    dir[2]=dir[2]||adatto[i][2][k];
            }
        }
        else    dir[2]=1;
        if(x>0)
        {
            for(int k=0; k<8; k++)
            {
                if(poss[y][x-1]>>k&1)
                    dir[3]=dir[3]||adatto[i][3][k];
            }
        }
        else    dir[3]=1;

        if(!(dir[0]&&dir[1]&&dir[2]&&dir[3])&&(poss[y][x]>>i&1))
        {
            poss[y][x]-=1<<i;
            camb=1;
        }
    }

    int cont=0;
    for(int i=0; i<8; i++)
    {
        if(poss[y][x]>>i&1)
            cont++;
    }
    errore=cont<1;

    if(camb || first)
    {
        if(y>0) errore=errore||riduci(poss, r, c, x, y-1, adatto, 0);
        if(x<c-1) errore=errore||riduci(poss, r, c, x+1, y, adatto, 0);
        if(y<r-1) errore=errore||riduci(poss, r, c, x, y+1, adatto, 0);
        if(x>0) errore=errore||riduci(poss, r, c, x-1, y, adatto, 0);
    }
    return errore;
}
void inizializza(unsigned char poss[100][100], int r, int c, bool adatto[8][4][8])
{
    for(int i=0; i<r; i++)  for(int j=0; j<c; j++)  poss[i][j]=255;
    for(int i=0; i<r-2; i++)
    {
        poss[i+1][0]=1;
        poss[i+1][c-1]=1;

        riduci(poss, r, c, 0, i+1, adatto, 1);
        riduci(poss, r, c, c-1, i+1, adatto, 1);
    }
    for(int i=0; i<c-2; i++)
    {
        poss[0][i+1]=2;
        poss[r-1][i+1]=2;

        riduci(poss, r, c, i+1, 0, adatto, 1);
        riduci(poss, r, c, i+1, r-1, adatto, 1);
    }

    poss[0][0]=64;
    riduci(poss, r, c, 0, 0, adatto, 1);

    poss[0][c-1]=128;
    riduci(poss, r, c, c-1, 0, adatto, 1);

    poss[r-1][0]=16;
    riduci(poss, r, c, 0, r-1, adatto, 1);

    poss[r-1][c-1]=32;
    riduci(poss, r, c, c-1, r-1, adatto, 1);
}
bool fine(unsigned char poss[100][100], int r, int c)
{
    bool ret=1;
    for(int i=0; i<r; i++)  for(int j=0; j<c; j++)  ret=ret&&(collassa(poss[i][j])!=0);
    return ret;
}
bool randCollassa(unsigned char poss[100][100], int r, int c, bool adatto[8][4][8])
{
    int contMin=8;
    int x=0, y=0;

    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            int cont=0;
            for(int k=0; k<8; k++)
            {
                if(poss[i][j]>>k&1)
                {
                    cont++;
                }
            }
            if(cont>1 && cont<=contMin)
            {
                contMin=cont;
                x=j;
                y=i;
            }
        }
    }

    int nPoss=0;
    unsigned char elPoss[8];
    for(int i=0; i<8; i++)
    {
        if(poss[y][x]>>i&1)
        {
            elPoss[nPoss]=i;
            nPoss++;
        }
    }
    poss[y][x]=1<<elPoss[rand()%nPoss];
    return riduci(poss, r, c, x, y, adatto, 1);
}
bool risolvi(unsigned char poss[100][100], int r, int c, bool adatto[8][4][8])
{
    while(!fine(poss, r, c)){if(randCollassa(poss, r, c, adatto)) {return 0;cout<<"\nErrore";}}
    return 1;
}

sf::Texture disegnaIntreccio(sf::Vector2f dimIntrSchermata)
{
    #define LargTile 8
    int r=dimIntrSchermata.y/LargTile;
    int c=dimIntrSchermata.x/LargTile;
    unsigned char poss[100][100];
    bool adatto[8][4][8];//tileA, dir, tileB
    int tile[100][100];
    srand(time(0));
    caricaTabAdatto(adatto);
    int i=0;
    do{inizializza(poss, r, c, adatto);}while(!risolvi(poss, r, c, adatto));
    for(int i=0; i<r; i++)  for(int j=0; j<c; j++)  tile[i][j]=collassa(poss[i][j]);

    sf::RenderTexture tavola;
    tavola.create((unsigned int) c*LargTile, (unsigned int) r*LargTile);
    tavola.clear();

    sf::RectangleShape quad(sf::Vector2f(LargTile/4.f, LargTile/4.f));
    quad.setFillColor(sf::Color::White);

    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            switch(tile[i][j])
            {
                case 0:

                break;

                case 1:
                    quad.setPosition((j+3/8.f)*LargTile, (i+0/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+2/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+4/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+6/8.f)*LargTile);
                    tavola.draw(quad);
                break;

                case 2:
                    quad.setPosition((j+0/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+2/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+4/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+6/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                break;

                case 3:
                    quad.setPosition((j+3/8.f)*LargTile, (i+0/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+2/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+4/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+6/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+0/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+6/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                break;
                case 4:
                    quad.setPosition((j+0/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+2/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+4/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+6/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+3/8.f)*LargTile, (i+0/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+6/8.f)*LargTile);
                    tavola.draw(quad);
                break;

                case 5:
                    quad.setPosition((j+3/8.f)*LargTile, (i+0/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+2/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+4/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+6/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+3/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                break;
                case 6:
                    quad.setPosition((j+3/8.f)*LargTile, (i+0/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+2/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+0/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+2/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+3/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                break;
                case 7:
                    quad.setPosition((j+3/8.f)*LargTile, (i+4/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+6/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+4/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+6/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+3/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                break;
                case 8:
                    quad.setPosition((j+3/8.f)*LargTile, (i+4/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+3/8.f)*LargTile, (i+6/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+0/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                    quad.setPosition((j+2/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);

                    quad.setPosition((j+3/8.f)*LargTile, (i+3/8.f)*LargTile);
                    tavola.draw(quad);
                break;
            }
        }
    }

    return tavola.getTexture();
}
///./////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int visGioca(sf::RenderWindow* window, Sezione sezione[], int nSez, int &indPagina, sf::Texture intreccio, string nomePagina[10000], int nota[100], int &nNote, int &sorte, int scroll)
{
    float lC[256]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.2, 1.2, 1.1, 1.1, 2, 1.3, 1.3, 1.4, 1.4, 1.4, 1.2, 1.5, 1.4, 0.5, 1, 1.3, 1.1, 1.6, 1.4, 1.5, 1.2, 1.5, 1.4, 1.3, 1.2, 1.5, 1.3, 1.9, 1.3, 1.3, 1.2, 0.5, 0.5, 0.5, 0.9, 1.1, 0.7, 1, 1, 1, 1.1, 1.1, 0.6, 1.1, 1.1, 0.4, 0.4, 1, 0.4, 1.6, 1.1, 1.1, 1.1, 1, 0.7, 0.9, 0.6, 1.1, 1, 1.4, 1, 1, 1, 0.7, 0.5, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    sf::Text testoPagina;
    testoPagina.setFont(font);
    testoPagina.setCharacterSize(AltezzaCarattere);
    testoPagina.setStyle(sf::Text::Regular);

    for(int i=0; i<nSez; i++)
    {
        float c=0;
        testoPagina.setFillColor(sezione[i].colore);
        testoPagina.setStyle(sezione[i].stile);
        for(int j=0; j<sezione[i].testo.size(); j++)
        {
            testoPagina.setString(sezione[i].testo.substr(j, 1));
            testoPagina.setPosition(sezione[i].posizione+sf::Vector2f(c*AltezzaCarattere/2, AltezzaCarattere*scroll/4.f));
            window->draw(testoPagina);
            c+=lC[sezione[i].testo[j]];
        }
    }


    ///---------------Sorto-----------------------

    sf::Text testoSorte;
    testoSorte.setFillColor(sf::Color::White);
    testoSorte.setFont(font);
    testoSorte.setCharacterSize(AltezzaCarattere);
    testoSorte.setStyle(sf::Text::Bold);

    testoSorte.setString("Sorto: "+to_string(sorte));
    testoSorte.setPosition(sf::Vector2f(MargS*10/50, MargA/2));
    window->draw(testoSorte);

    static bool premutoP=0;

    testoSorte.setString("+");
    testoSorte.setPosition(sf::Vector2f(MargS*30/50, MargA/2));
    if(testoSorte.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSorte.setFillColor(sf::Color::White);
        if(premutoP)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoP=0;
                sorte++;
            }
        }
        else    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premutoP=1;
    }
    else
    {
        testoSorte.setFillColor(sf::Color(200, 200, 200));
        premutoP=0;
    }
    window->draw(testoSorte);


    testoSorte.setPosition(sf::Vector2f(MargS*35/50, MargA/2));
    static bool premutoM=0;
    if(testoSorte.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSorte.setFillColor(sf::Color::White);
        if(premutoM)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoM=0;
                sorte--;
            }
        }
        else    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premutoM=1;
    }
    else
    {
        testoSorte.setFillColor(sf::Color(200, 200, 200));
        premutoM=0;
    }
    testoSorte.setString("-");
    window->draw(testoSorte);


    sf::Sprite barraS;
    barraS.setTexture(intreccio);
    barraS.setPosition(0, 0);
    if(!barraS.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        window->draw(barraS);


    ///-----------------Notoj-----------------

    sf::Text testoNote;
    testoNote.setFont(font);
    testoNote.setCharacterSize(AltezzaCarattere);
    testoNote.setStyle(sf::Text::Bold);

    testoNote.setString("------Notoj-----");
    testoNote.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5, MargA/2));
    window->draw(testoNote);

    testoNote.setStyle(sf::Text::Regular);
    static int notaAperta=nNote;
    for(int i=0; i<nNote; i++)
    {
        testoNote.setFillColor(sf::Color(200, 200, 200));
        testoNote.setString(nomePagina[nota[i]]+" ["+to_string(nota[i])+"]");
        testoNote.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5, MargA+(i+(notaAperta<i))*AltezzaCarattere));
        if(testoNote.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            testoNote.setFillColor(sf::Color::White);
            notaAperta=i;
        }
        if(notaAperta==i)
        {
            sf::Vector2f pos(testoNote.getGlobalBounds().left, testoNote.getGlobalBounds().top), siz(MargS*8/10, testoNote.getGlobalBounds().height*2);
            if(sf::FloatRect(pos, siz).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                testoNote.setFillColor(sf::Color::White);
                {//opzioni nota

                    sf::Text testoOpzioni;
                    testoOpzioni.setFont(font);
                    testoOpzioni.setCharacterSize(AltezzaCarattere*4/5);
                    testoOpzioni.setStyle(sf::Text::Regular);

                    //vidu
                    testoOpzioni.setString("vidu");
                    testoOpzioni.setFillColor(sf::Color(200, 200, 200));
                    testoOpzioni.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5, MargA+(i+1)*AltezzaCarattere));

                    static bool viduPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(sf::Color::White);
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) viduPremuto=1;
                        else{
                            if(viduPremuto)
                            {
                                viduPremuto=0;
                                indPagina=nota[i];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                    //uzu
                    testoOpzioni.setString("uzu");
                    testoOpzioni.setStyle(sf::Text::Regular);
                    testoOpzioni.setFillColor(sf::Color(200, 200, 200));
                    testoOpzioni.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5+2*AltezzaCarattere, MargA+(i+1)*AltezzaCarattere));

                    static bool uzuPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(sf::Color::White);
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) uzuPremuto=1;
                        else{
                            if(uzuPremuto)
                            {
                                uzuPremuto=0;
                                indPagina+=nota[i];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                    //visxu
                    testoOpzioni.setString("visxu");
                    testoOpzioni.setStyle(sf::Text::Regular);
                    testoOpzioni.setFillColor(sf::Color(200, 200, 200));
                    testoOpzioni.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5+4*AltezzaCarattere, MargA+(i+1)*AltezzaCarattere));

                    static bool visxuPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(sf::Color::White);
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) visxuPremuto=1;
                        else{
                            if(visxuPremuto)
                            {
                                visxuPremuto=0;
                                nNote--;
                                for(int j=i; j<nNote; j++)  nota[j]=nota[j+1];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                }
            }
            else
            {
                notaAperta=nNote;
            }
        }
        window->draw(testoNote);
    }

    sf::Sprite barraD;
    barraD.setTexture(intreccio);
    barraD.scale(-1.f, 1.f);
    barraD.setPosition(LarghezzaSchermo, 0);
    if(!barraD.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        window->draw(barraD);

    ///-----------------Barra Bassa-----------------

    sf::RectangleShape barraB(sf::Vector2f(LarghezzaSchermo-2*MargS, MargA));
    barraB.setFillColor(sf::Color::Black);
    barraB.setPosition(MargS, AltezzaSchermo-MargA);
    window->draw(barraB);


    {///Paragrafo precedente
        sf::Vector2f pos(MargS, AltezzaSchermo-(MargA*9/10+60.f*prop)/2);
        sf::Vector2f diff(3.f, 3.f);

        sf::RectangleShape PulsantePP1(sf::Vector2f(60.f, 60.f)*prop);
        PulsantePP1.setPosition(pos*prop);
        PulsantePP1.setFillColor(sf::Color::Black);

        sf::RectangleShape PulsantePP2(sf::Vector2f(54.f, 54.f)*prop);
        PulsantePP2.setPosition((pos+diff)*prop);
        PulsantePP2.setFillColor(sf::Color::White);

        sf::RectangleShape PulsantePP3(sf::Vector2f(48.f, 48.f)*prop);
        PulsantePP3.setPosition((pos+2.f*diff)*prop);
        PulsantePP3.setFillColor(sf::Color::Black);

        static bool PulsanteParagrafoPrecedentePremuto=0;
        if(PulsantePP1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)))&& indPagina!=0)
        {
            PulsantePP1.setFillColor(sf::Color::White);
            PulsantePP2.setFillColor(sf::Color::Black);
            PulsantePP3.setFillColor(sf::Color::White);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) PulsanteParagrafoPrecedentePremuto=1;
            else
            {
                if(PulsanteParagrafoPrecedentePremuto)
                {
                    PulsanteParagrafoPrecedentePremuto=0;
                    indCP--;
                    indPagina=cronoPagine[indCP];
                    indCP--;
                }
            }
        }
        window->draw(PulsantePP1);
        window->draw(PulsantePP2);
        window->draw(PulsantePP3);
    }

    {///Paragrafo scelto
        sf::Vector2f pos(MargS+70*prop, AltezzaSchermo-(MargA*9/10+60.f*prop)/2);
        sf::Vector2f diff(3.f, 3.f);

        sf::RectangleShape PulsantePP1(sf::Vector2f(120.f, 60.f)*prop);
        PulsantePP1.setPosition(pos*prop);
        PulsantePP1.setFillColor(sf::Color::Black);

        sf::RectangleShape PulsantePP2(sf::Vector2f(114.f, 54.f)*prop);
        PulsantePP2.setPosition((pos+diff)*prop);
        PulsantePP2.setFillColor(sf::Color::White);

        sf::RectangleShape PulsantePP3(sf::Vector2f(108.f, 48.f)*prop);
        PulsantePP3.setPosition((pos+2.f*diff)*prop);
        PulsantePP3.setFillColor(sf::Color::Black);

        static bool PulsanteParagrafoPrecedentePremuto=0;
        static bool staDigitando=0;
        static int prossimoParagrafo=0;
        if(staDigitando)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                staDigitando=0;
                indPagina=prossimoParagrafo;
                prossimoParagrafo=0;
            }
        }
        if(PulsantePP1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            PulsantePP1.setFillColor(sf::Color::White);
            PulsantePP2.setFillColor(sf::Color::Black);
            PulsantePP3.setFillColor(sf::Color::White);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) PulsanteParagrafoPrecedentePremuto=1;
            else
            {
                if(PulsanteParagrafoPrecedentePremuto)
                {
                    PulsanteParagrafoPrecedentePremuto=0;
                    staDigitando=1;
                }
            }
        }
        window->draw(PulsantePP1);
        window->draw(PulsantePP2);
        window->draw(PulsantePP3);
    }



    ///-----------------Barra Alta-----------------

    barraB.setFillColor(sf::Color::Black);
    barraB.setPosition(MargS, 0);
    window->draw(barraB);
    {///Home
        sf::Vector2f pos(MargS, (MargA-60.f)/2);
        sf::Vector2f diff(3.f, 3.f);

        sf::RectangleShape PulsanteH1(sf::Vector2f(60.f, 60.f)*prop);
        PulsanteH1.setPosition(pos*prop);
        PulsanteH1.setFillColor(sf::Color::Black);

        sf::RectangleShape PulsanteH2(sf::Vector2f(54.f, 54.f)*prop);
        PulsanteH2.setPosition((pos+diff)*prop);
        PulsanteH2.setFillColor(sf::Color::White);

        sf::RectangleShape PulsanteH3(sf::Vector2f(48.f, 48.f)*prop);
        PulsanteH3.setPosition((pos+diff*2.f)*prop);
        PulsanteH3.setFillColor(sf::Color::Black);

        static bool PulsanteHomePremuto=0;
        if(PulsanteH1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            PulsanteH1.setFillColor(sf::Color::White);
            PulsanteH2.setFillColor(sf::Color::Black);
            PulsanteH3.setFillColor(sf::Color::White);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) PulsanteHomePremuto=1;
            else
            {
                if(PulsanteHomePremuto)
                {
                    PulsanteHomePremuto=0;
                    return home;
                }
            }
        }
        window->draw(PulsanteH1);
        window->draw(PulsanteH2);
        window->draw(PulsanteH3);
    }

    return gioca;
}

int visHome(sf::RenderWindow* window, sf::Texture intreccio)
{
    sf::Sprite barraS;
    barraS.setTexture(intreccio);
    barraS.setPosition(0, 0);
    window->draw(barraS);

    sf::Sprite barraD;
    barraD.setTexture(intreccio);
    barraD.scale(-1.f, 1.f);
    barraD.setPosition(LarghezzaSchermo, 0);
    window->draw(barraD);

    {///Gioca
        sf::Vector2f pos((LarghezzaSchermo-120)/2, (AltezzaSchermo-60)/2);
        sf::Vector2f diff(3.f, 3.f);

        sf::RectangleShape PulsanteH1(sf::Vector2f(120.f, 60.f)*prop);
        PulsanteH1.setPosition(pos*prop);
        PulsanteH1.setFillColor(sf::Color::Black);

        sf::RectangleShape PulsanteH2(sf::Vector2f(114.f, 54.f)*prop);
        PulsanteH2.setPosition((pos+diff)*prop);
        PulsanteH2.setFillColor(sf::Color::White);

        sf::RectangleShape PulsanteH3(sf::Vector2f(108.f, 48.f)*prop);
        PulsanteH3.setPosition((pos+diff*2.f)*prop);
        PulsanteH3.setFillColor(sf::Color::Black);

        static bool PulsanteHomePremuto=0;
        if(PulsanteH1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            PulsanteH1.setFillColor(sf::Color::White);
            PulsanteH2.setFillColor(sf::Color::Black);
            PulsanteH3.setFillColor(sf::Color::White);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) PulsanteHomePremuto=1;
            else
            {
                if(PulsanteHomePremuto)
                {
                    PulsanteHomePremuto=0;
                    return gioca;
                }
            }
        }
        window->draw(PulsanteH1);
        window->draw(PulsanteH2);
        window->draw(PulsanteH3);
    }
    return home;
}

void visualizza(sf::RenderWindow* window, int &schermata, Sezione sezione[], int nSez, int &indPagina, sf::Texture intreccio, string nomePagina[10000], int nota[100], int &nNote, int &sorte, int scroll)
{
    float lC[256]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.2, 1.2, 1.1, 1.1, 2, 1.3, 1.3, 1.4, 1.4, 1.4, 1.2, 1.5, 1.4, 0.5, 1, 1.3, 1.1, 1.6, 1.4, 1.5, 1.2, 1.5, 1.4, 1.3, 1.2, 1.5, 1.3, 1.9, 1.3, 1.3, 1.2, 0.5, 0.5, 0.5, 0.9, 1.1, 0.7, 1, 1, 1, 1.1, 1.1, 0.6, 1.1, 1.1, 0.4, 0.4, 1, 0.4, 1.6, 1.1, 1.1, 1.1, 1, 0.7, 0.9, 0.6, 1.1, 1, 1.4, 1, 1, 1, 0.7, 0.5, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    switch(schermata)
    {
        case home:
            schermata=visHome(window, intreccio);
        break;

        case gioca:
            schermata=visGioca(window, sezione, nSez, indPagina, intreccio, nomePagina, nota, nNote, sorte, scroll);
        break;

        case impostazioni:

        break;
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
                if(testo[i]==' ' || testo[i]=='\n' || testo[i]==',' || testo[i]=='.' || testo[i]==':' || testo[i]==';'|| testo[i]=='!'|| testo[i]=='?')
                {
                    sezione[indSez].stile=sf::Text::Regular;
                    sezione[indSez].tipo='a';
                    sezione[indSez].arg=0;
                    sezione[indSez].colore=sf::Color::White;
                    sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2+MargS, r*AltezzaCarattere+MargA);
                    i--;
                }
                else//speciale
                {
                    sezione[indSez].stile=sf::Text::Regular;
                    sezione[indSez].tipo=testo[i];
                    i++;
                    int k=testo.substr(i).find(" ");
                    sezione[indSez].arg=stoi(testo.substr(i, k));
                    i+=k;
                    sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2+MargS, r*AltezzaCarattere+MargA);
                    sezione[indSez].colore=sf::Color(200, 200, 200);
                }
            }
            else//#
            {
                i++;
                sezione[indSez].stile=sf::Text::Bold;
                sezione[indSez].tipo='p';
                int k=testo.substr(i).find(" ");
                sezione[indSez].arg=stoi(testo.substr(i, k));
                sezione[indSez].testo+=to_string(sezione[indSez].arg)+" - ";
                i+=k;
                sezione[indSez].colore=sf::Color::White;
                sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2+MargS, r*AltezzaCarattere+MargA);
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
                    sezione[indSez].stile=sezione[indSez-1].stile;
                    sezione[indSez].tipo=sezione[indSez-1].tipo;
                    sezione[indSez].arg=sezione[indSez-1].arg;
                    sezione[indSez].colore=sezione[indSez-1].colore;
                    sezione[indSez].posizione=sf::Vector2f(c*AltezzaCarattere/2+MargS, r*AltezzaCarattere+MargA);
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

void azionaIpertesto(Sezione sezione[], int nSez, sf::RenderWindow* window, int &indPagina, int nota[100], int &nNote, int &sorte, int scroll)
{
    static bool premuto[1000]={};
    for(int i=0; i<nSez; i++)
    {
        if(sezione[i].tipo=='l')
        {
            if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*AltezzaCarattere/4.f, sezione[i].lung*AltezzaCarattere/2, AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                sezione[i].colore=sf::Color::White;
                sezione[i].stile=sf::Text::Bold;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    indPagina=sezione[i].arg;
                }
            }
            else
            {
                sezione[i].stile=sf::Text::Regular;
                sezione[i].colore=sf::Color(100, 100, 100);
                premuto[i]=0;
            }
        }
        if(sezione[i].tipo=='n')
        {
            if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*AltezzaCarattere/4.f, sezione[i].lung*AltezzaCarattere/2, AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                sezione[i].colore=sf::Color::White;
                sezione[i].stile=sf::Text::Bold;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    bool presente=0;
                    for(int j=0; j<nNote; j++)
                        presente=presente || (nota[j]==sezione[i].arg);
                    if(!presente)
                    {
                        nota[nNote]=sezione[i].arg;
                        nNote++;
                    }
                }
            }
            else
            {
                sezione[i].stile=sf::Text::Regular;
                sezione[i].colore=sf::Color(100, 100, 100);
                premuto[i]=0;
            }
        }
        if(sezione[i].tipo=='s')
        {
            if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*AltezzaCarattere/4.f, sezione[i].lung*AltezzaCarattere/2, AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                sezione[i].colore=sf::Color::White;
                sezione[i].stile=sf::Text::Bold;
                if(premuto[i])
                {
                    if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        premuto[i]=0;
                        sorte+=sezione[i].arg;
                    }
                }
                else
                {
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premuto[i]=1;
                }
            }
            else
            {
                sezione[i].stile=sf::Text::Regular;
                sezione[i].colore=sf::Color(100, 100, 100);
                premuto[i]=0;
            }
        }
    }
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

void caricamento(sf::RenderWindow* window, string pagina[10000], string nomePagina[10000], Sezione sezione[100], int &nPagine, int &indPagina, int &nSez, string &testo)
{
    if (!font.loadFromFile("arial.ttf"))
        cout<<"Errore";// error...

    window->clear();
    sf::Text testoCaricamento;
    testoCaricamento.setFont(font);
    testoCaricamento.setString("Caricamento in corso\nAttendere prego");
    testoCaricamento.setPosition((LarghezzaSchermo-testoCaricamento.getGlobalBounds().width)/2, (AltezzaSchermo-testoCaricamento.getGlobalBounds().height)/2);
    window->draw(testoCaricamento);
    window->display();

    testo=caricaLibro("Libro0");
    unsigned int inizio=0, fine=0, lunghezza=0;
    for(int i=0; fine!=testo.size()-1; i++)
    {
        inizio=testo.find("#"+to_string(i)+" ");
        fine=testo.find("#"+to_string(i+1)+" ");
        if(fine==string::npos)  fine=testo.size()-1;
        lunghezza=fine-inizio;
        nomePagina[i]=testo.substr(inizio).substr(testo.find(" ")+2, testo.substr(inizio).find("/")-4);
        pagina[i]=testo.substr(inizio, lunghezza);
        nPagine=i+1;
    }
    nSez=disseziona(pagina[indPagina], sezione);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(LarghezzaSchermo, AltezzaSchermo), "Display");
    window.setPosition(sf::Vector2i(-10, 0));

    string testo="#0/";

    Sezione sezione[100];
    int nSez=0;

    string nomePagina[10000];
    string pagina[10000];
    int nPagine=0;
    int indPagina=0;

    int nota[100];
    int nNote=0;

    int sorte=2;

    int scroll=0;

    caricamento(&window, pagina, nomePagina, sezione, nPagine, indPagina, nSez, testo);

    sf::Vector2f dimIntrSchermata[5];
    dimIntrSchermata[home]=sf::Vector2f(MargS*15/10, AltezzaSchermo);
    dimIntrSchermata[gioca]=sf::Vector2f(MargS*9/10, AltezzaSchermo);

    sf::Texture intreccio=disegnaIntreccio(dimIntrSchermata[home]);
    sf::Texture intreccioR[2]={disegnaIntreccio(dimIntrSchermata[home]), disegnaIntreccio(dimIntrSchermata[gioca])};


    int schermata=home, schermataS=home;

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                scroll=min(event.mouseWheelScroll.delta+scroll, 0.f);
            }
        }
        window.clear();
        if(indCP<0)
        {
            indCP=0;
            nCP=1;
            cronoPagine[indCP]=0;
            nSez=disseziona(pagina[indPagina], sezione);
        }
        bool cambioPagina=0;
        if(cronoPagine[indCP]!=indPagina)
        {
            cambioPagina=1;
            indCP++;
            nCP=indCP+1;
            cronoPagine[indCP]=indPagina;
            nSez=disseziona(pagina[indPagina], sezione);
        }

        if(cambioPagina || schermata!=schermataS)
        {
            schermataS=schermata;
            intreccio=intreccioR[schermata];
            azionaIpertesto(sezione, nSez, &window, indPagina, nota, nNote, sorte, scroll);
            visualizza(&window, schermata, sezione, nSez, indPagina, intreccio, nomePagina, nota, nNote, sorte, scroll);
            window.display();
            intreccioR[schermata]=disegnaIntreccio(dimIntrSchermata[schermata]);
        }
        else
        {
            azionaIpertesto(sezione, nSez, &window, indPagina, nota, nNote, sorte, scroll);
            visualizza(&window, schermata, sezione, nSez, indPagina, intreccio, nomePagina, nota, nNote, sorte, scroll);
            window.display();
        }
    }

    return 0;
}


