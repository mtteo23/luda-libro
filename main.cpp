

#include <time.h>
#include <cmath>
#include "GUI.hpp"




//Paragrafo: Azzera_barre Barra X(normale) Link Colore Nota Oggetto Sorte


float MargA=100;
float MargS=300;
float LarghezzaPagina=LarghezzaSchermo-2*MargS;

int indCP=0, nCP=0;

enum {home, gioca, impostazioni, elencoLibri, elencoGiochi};

struct Sezione
{
    string testo="";//principale, a capo
    float lung=0;
    sf::Vector2f posizione;
    int colore=1;
    sf::Text::Style stile=sf::Text::Regular;
    int arg=0;
    char tipo='x';
};

class Partita{
    public:
    int indPagina=0;
    int cronoPagine[10000]={0};

    int nota[100];
    int nNote=0;

    int sorte=2;
    int barre=0;

}partita;

string scritta[100];


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
    int r=dimIntrSchermata.y/LargTile/2+4;
    int c=dimIntrSchermata.x/LargTile+2;
    unsigned char poss[100][100];
    bool adatto[8][4][8];//tileA, dir, tileB
    int tile[100][100];
    srand(time(0));
    caricaTabAdatto(adatto);

    do{inizializza(poss, r, c, adatto);}while(!risolvi(poss, r, c, adatto));
    for(int i=0; i<r; i++)  for(int j=0; j<c; j++)  tile[i][j]=collassa(poss[i][j]);

    sf::RenderTexture tavola;
    tavola.create((unsigned int) c*LargTile, (unsigned int) (r-2)*2*LargTile);
    tavola.clear(impozado.colore[0]);

    sf::RectangleShape quad(sf::Vector2f(LargTile/4.f, LargTile/4.f));
    quad.setFillColor(impozado.colore[1]);

    for(int i=1; i<r-1; i++)
    {
        for(int j=1; j<c-1; j++)
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





    sf::Sprite barraD;
    barraD.setTexture(tavola.getTexture());
    barraD.setTextureRect(sf::IntRect(0, r*LargTile, c*LargTile, r*LargTile));

    barraD.setPosition(0, 0+r*LargTile);
    tavola.draw(barraD);

    barraD.scale(1.f, -1.f);
    barraD.setPosition(0, 0+r*LargTile);
    tavola.draw(barraD);

    return tavola.getTexture();
}
///./////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
        sezione[i].tipo='x';
        sezione[i].colore=1;
        sezione[i].posizione=sf::Vector2f(0.f, 0.f);
        sezione[i].lung=0;
    }
    for(unsigned int i=0; i<testo.size(); i++)
    {
        if(testo[i]=='/' || testo[i]=='#')
        {
            if(sezione[indSez].lung>0 || sezione[indSez].tipo!='x')
                indSez++;
            if(testo[i]=='/')
            {
                i++;
                if(testo[i]==' ' || testo[i]=='\n' || testo[i]==',' || testo[i]=='.' || testo[i]==':' || testo[i]==';'|| testo[i]=='!'|| testo[i]=='?')
                {
                    sezione[indSez].stile=sf::Text::Regular;
                    sezione[indSez].tipo='x';
                    sezione[indSez].arg=0;
                    sezione[indSez].colore=1;
                    sezione[indSez].posizione=sf::Vector2f(c*impozado.AltezzaCarattere/2+MargS, r*impozado.AltezzaCarattere+MargA);
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
                    sezione[indSez].posizione=sf::Vector2f(c*impozado.AltezzaCarattere/2+MargS, r*impozado.AltezzaCarattere+MargA);
                    sezione[indSez].colore=2;
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
                sezione[indSez].colore=1;
                sezione[indSez].posizione=sf::Vector2f(c*impozado.AltezzaCarattere/2+MargS, r*impozado.AltezzaCarattere+MargA);
            }
        }
        else
        {
            int lungProsParola=testo.substr(i+1).find(" ");
            if(testo[i]=='\n' || (c+lungProsParola)*impozado.AltezzaCarattere/2>LarghezzaPagina)
            {
                c=0;
                r++;
                if(sezione[indSez].tipo=='x' || 1)
                {
                    indSez++;
                    sezione[indSez].stile=sezione[indSez-1].stile;
                    sezione[indSez].tipo=sezione[indSez-1].tipo;
                    sezione[indSez].arg=sezione[indSez-1].arg;
                    sezione[indSez].colore=sezione[indSez-1].colore;
                    sezione[indSez].posizione=sf::Vector2f(c*impozado.AltezzaCarattere/2+MargS, r*impozado.AltezzaCarattere+MargA);
                }
                else
                {
                    //Work in progress
                }
            }
            else
            {
                sezione[indSez].testo+=testo.substr(i, 1);
                sezione[indSez].lung+=lC[(int) testo[i]];
                c+=lC[(int) testo[i]];
            }
        }
    }
    return indSez+1;
}

string caricaLibro(string titolo)
{
    ifstream fin("libroj/"+titolo+".txt");
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

void caricaLingua()
{
    ifstream fin("tradukajxoj\\"+impozado.lingvo+".txt");
    for(int i=0; i<100 && !fin.eof(); i++)
    {
        getline(fin, scritta[i]);
    }
}

int visGioca(sf::RenderWindow* window, Sezione sezione[], int nSez, sf::Texture intreccio, string nomePagina[10000], int scroll)
{
    float lC[256]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.2, 1.2, 1.1, 1.1, 2, 1.3, 1.3, 1.4, 1.4, 1.4, 1.2, 1.5, 1.4, 0.5, 1, 1.3, 1.1, 1.6, 1.4, 1.5, 1.2, 1.5, 1.4, 1.3, 1.2, 1.5, 1.3, 1.9, 1.3, 1.3, 1.2, 0.5, 0.5, 0.5, 0.9, 1.1, 0.7, 1, 1, 1, 1.1, 1.1, 0.6, 1.1, 1.1, 0.4, 0.4, 1, 0.4, 1.6, 1.1, 1.1, 1.1, 1, 0.7, 0.9, 0.6, 1.1, 1, 1.4, 1, 1, 1, 0.7, 0.5, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    sf::Text testoPagina;
    testoPagina.setFont(impozado.font);
    testoPagina.setCharacterSize(impozado.AltezzaCarattere);
    testoPagina.setStyle(sf::Text::Regular);

    for(int i=0; i<nSez; i++)
    {
        float c=0;
        testoPagina.setFillColor(impozado.colore[sezione[i].colore]);
        testoPagina.setStyle(sezione[i].stile);
        for(unsigned int j=0; j<sezione[i].testo.size(); j++)
        {
            testoPagina.setString(sezione[i].testo.substr(j, 1));
            testoPagina.setPosition(sezione[i].posizione+sf::Vector2f(c*impozado.AltezzaCarattere/2, impozado.AltezzaCarattere*scroll/4.f));
            window->draw(testoPagina);
            c+=lC[(int) sezione[i].testo[j]];
        }
    }


    ///---------------Sorto-----------------------
    {

    sf::Text testoSorte;
    testoSorte.setFillColor(impozado.colore[1]);
    testoSorte.setFont(impozado.font);
    testoSorte.setCharacterSize(impozado.AltezzaCarattere);
    testoSorte.setStyle(sf::Text::Bold);

    testoSorte.setString(scritta[9]+": "+to_string(partita.sorte));
    testoSorte.setPosition(sf::Vector2f(MargS*10/50, MargA/2));
    window->draw(testoSorte);

    static bool premutoP=0;

    testoSorte.setString("+");
    testoSorte.setPosition(sf::Vector2f(MargS*30/50, MargA/2));
    if(testoSorte.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSorte.setFillColor(impozado.colore[1]);
        if(premutoP)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoP=0;
                partita.sorte++;
            }
        }
        else    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premutoP=1;
    }
    else
    {
        testoSorte.setFillColor(impozado.colore[2]);
        premutoP=0;
    }
    window->draw(testoSorte);


    testoSorte.setPosition(sf::Vector2f(MargS*35/50, MargA/2));
    static bool premutoM=0;
    if(testoSorte.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSorte.setFillColor(impozado.colore[1]);
        if(premutoM)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoM=0;
                partita.sorte=max(0, partita.sorte-1);
            }
        }
        else    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premutoM=1;
    }
    else
    {
        testoSorte.setFillColor(impozado.colore[2]);
        premutoM=0;
    }
    testoSorte.setString("-");
    window->draw(testoSorte);

    }

    ///-----------------strekoj------------------
    {

    sf::Text testoSbarre;
    testoSbarre.setFillColor(impozado.colore[1]);
    testoSbarre.setFont(impozado.font);
    testoSbarre.setCharacterSize(impozado.AltezzaCarattere);
    testoSbarre.setStyle(sf::Text::Bold);

    testoSbarre.setPosition(sf::Vector2f(MargS*10/50, MargA/2+impozado.AltezzaCarattere*1.5f));
    string txtSbarre="";
    for(int i=0; i<partita.barre; i++)
    {
        if(i%10==0 && i) txtSbarre+="\n";
        else
        if(i%5==0 && i) txtSbarre+=" ";
        txtSbarre+="/";

    }
    testoSbarre.setString(txtSbarre);
    window->draw(testoSbarre);


    static bool premutoP=0;

    testoSbarre.setString("+");
    testoSbarre.setPosition(sf::Vector2f(MargS*30/50, MargA/2+impozado.AltezzaCarattere*1.5f));
    if(testoSbarre.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSbarre.setFillColor(impozado.colore[1]);
        if(premutoP)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoP=0;
                partita.barre++;
            }
        }
        else    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premutoP=1;
    }
    else
    {
        testoSbarre.setFillColor(impozado.colore[2]);
        premutoP=0;
    }
    window->draw(testoSbarre);


    testoSbarre.setPosition(sf::Vector2f(MargS*35/50, MargA/2+impozado.AltezzaCarattere*1.5f));
    static bool premutoM=0;
    if(testoSbarre.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSbarre.setFillColor(impozado.colore[1]);
        if(premutoM)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoM=0;
                partita.barre=max(0, partita.barre-1);
            }
        }
        else    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) premutoM=1;
    }
    else
    {
        testoSbarre.setFillColor(impozado.colore[2]);
        premutoM=0;
    }
    testoSbarre.setString("-");
    window->draw(testoSbarre);

    }

    ///------------intreccio Sinistro
    sf::Sprite barraS;
    barraS.setTexture(intreccio);
    barraS.setPosition(0, 0);
    if(!barraS.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        window->draw(barraS);


    ///-----------------Notoj-----------------

    {

    sf::Text testoNote;
    testoNote.setFont(impozado.font);
    testoNote.setCharacterSize(impozado.AltezzaCarattere);
    testoNote.setStyle(sf::Text::Bold);

    testoNote.setString("------"+scritta[10]+"-----");
    testoNote.setFillColor(impozado.colore[1]);
    testoNote.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5, MargA/2));
    window->draw(testoNote);

    testoNote.setStyle(sf::Text::Regular);
    static int notaAperta=partita.nNote;
    for(int i=0; i<partita.nNote; i++)
    {
        testoNote.setFillColor(impozado.colore[2]);
        testoNote.setString(nomePagina[partita.nota[i]]+" ["+to_string(partita.nota[i])+"]");
        testoNote.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5, MargA+(i+(notaAperta<i))*impozado.AltezzaCarattere));
        if(testoNote.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            testoNote.setFillColor(impozado.colore[1]);
            notaAperta=i;
        }
        if(notaAperta==i)
        {
            sf::Vector2f pos(testoNote.getGlobalBounds().left, testoNote.getGlobalBounds().top), siz(MargS*8/10, testoNote.getGlobalBounds().height*2);
            if(sf::FloatRect(pos, siz).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                testoNote.setFillColor(impozado.colore[1]);
                {//opzioni nota

                    sf::Text testoOpzioni;
                    testoOpzioni.setFont(impozado.font);
                    testoOpzioni.setCharacterSize(impozado.AltezzaCarattere*4/5);
                    testoOpzioni.setStyle(sf::Text::Regular);

                    //vidu
                    testoOpzioni.setString(scritta[11]);
                    testoOpzioni.setFillColor(impozado.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5, MargA+(i+1)*impozado.AltezzaCarattere));

                    static bool viduPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(impozado.colore[1]);
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) viduPremuto=1;
                        else{
                            if(viduPremuto)
                            {
                                viduPremuto=0;
                                partita.indPagina=partita.nota[i];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                    //uzu
                    testoOpzioni.setString(scritta[12]);
                    testoOpzioni.setStyle(sf::Text::Regular);
                    testoOpzioni.setFillColor(impozado.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5+3*impozado.AltezzaCarattere, MargA+(i+1)*impozado.AltezzaCarattere));

                    static bool uzuPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(impozado.colore[1]);
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) uzuPremuto=1;
                        else{
                            if(uzuPremuto)
                            {
                                uzuPremuto=0;
                                partita.indPagina+=partita.nota[i];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                    //visxu
                    testoOpzioni.setString(scritta[13]);
                    testoOpzioni.setStyle(sf::Text::Regular);
                    testoOpzioni.setFillColor(impozado.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(LarghezzaSchermo-MargS*4/5+5*impozado.AltezzaCarattere, MargA+(i+1)*impozado.AltezzaCarattere));

                    static bool visxuPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(impozado.colore[1]);
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) visxuPremuto=1;
                        else{
                            if(visxuPremuto)
                            {
                                visxuPremuto=0;
                                partita.nNote--;
                                for(int j=i; j<partita.nNote; j++)  partita.nota[j]=partita.nota[j+1];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                }
            }
            else
            {
                notaAperta=partita.nNote;
            }
        }
        window->draw(testoNote);
    }

    }

    ///-----------------Intreccio Destro
    sf::Sprite barraD;
    barraD.setTexture(intreccio);
    barraD.scale(-1.f, 1.f);
    barraD.setPosition(LarghezzaSchermo, 0);
    if(!barraD.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        window->draw(barraD);

    ///-----------------Barra Bassa-----------------

    sf::RectangleShape barraB(sf::Vector2f(LarghezzaSchermo-2*MargS, MargA));
    barraB.setFillColor(impozado.colore[0]);
    barraB.setPosition(MargS, AltezzaSchermo-MargA);
    window->draw(barraB);

    {///Paragrafo precedente
        static bool pHome=0;
        Pulsante PHome("<<", MargS, AltezzaSchermo-(MargA*9/10+60.f*prop)/2, 1, &pHome);
        if(PHome.draw(window)==3)
        {
            partita.indPagina=partita.cronoPagine[indCP-1];
            indCP-=2;
        }
    }

    {///Paragrafo scelto
        static bool staDigitando=0;
        static int prossimoParagrafo=0;
        static bool numeroPremuto[10];

        static bool pHome=0;
        Pulsante PHome(" _ _ _", MargS+70, AltezzaSchermo-(MargA*9/10+60.f*prop)/2, 2, &pHome);

        if(staDigitando)
        {
            PHome.etic=" "+to_string((int) prossimoParagrafo/100)+" "+to_string(((int) prossimoParagrafo%100)/10)+" "+to_string((int) prossimoParagrafo%10);

            sf::Keyboard::Key numKey[10]={sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9};
            for(int i=0; i<10; i++)
            {
                if(sf::Keyboard::isKeyPressed(numKey[i]))   numeroPremuto[i]=1;
                else
                {
                    if(numeroPremuto[i])
                    {
                        numeroPremuto[i]=0;
                        prossimoParagrafo=(prossimoParagrafo*10+i)%1000;
                    }
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                staDigitando=0;
                partita.indPagina=prossimoParagrafo;
                prossimoParagrafo=0;
            }
        }

        if(PHome.draw(window)==3)
        {
            staDigitando=1;
        }
    }

    {///Lancio moneta
        static bool faccia=0;
        static int numero=0;
        static sf::Time tempoLancio;
        static sf::Clock clock;

        sf::Vector2f pos(LarghezzaSchermo-MargS-60*prop, AltezzaSchermo-(MargA*9/10+60.f*prop)/2);
        sf::Vector2f diff(3.f, 3.f);

        sf::CircleShape PulsantePP1(32.f*prop);
        PulsantePP1.setPosition(pos*prop);
        PulsantePP1.setFillColor(impozado.colore[0]);


        sf::CircleShape PulsantePP2((32.f-diff.x)*prop);
        PulsantePP2.setPosition((pos+diff)*prop);
        PulsantePP2.setFillColor(impozado.colore[1]);

        sf::CircleShape PulsantePP3((32.f-2*diff.x)*prop);
        PulsantePP3.setPosition((pos+2.f*diff)*prop);
        PulsantePP3.setFillColor(impozado.colore[0]);

        sf::Text etichetta;
        etichetta.setFont(impozado.font);
        etichetta.setString(" M");
        if(faccia) etichetta.setString(" "+to_string(numero));
        etichetta.setFillColor(impozado.colore[1]);
        etichetta.setPosition((pos+4.f*diff)*prop);
        etichetta.setCharacterSize(impozado.AltezzaCarattere+5);

        if(clock.getElapsedTime()<tempoLancio)
        {
            float fase=cos(sqrt(clock.getElapsedTime().asMilliseconds()));
            PulsantePP1.setScale(1.f, abs(fase));
            PulsantePP2.setScale(1.f, abs(fase));
            PulsantePP3.setScale(1.f, abs(fase));
            etichetta.setScale(1.f, abs(fase));

            PulsantePP1.setPosition(pos*prop+sf::Vector2f(0, (32.f+0*diff.y)*(1-abs(fase))));
            PulsantePP2.setPosition(pos*prop+sf::Vector2f(diff.x, (32.f+0.5*diff.y)*(1-abs(fase))+0.5*diff.y));
            PulsantePP3.setPosition(pos*prop+sf::Vector2f(2*diff.x, (32.f+1*diff.y)*(1-abs(fase))+1*diff.y));
            etichetta.setPosition(pos*prop+sf::Vector2f(4*diff.x, (32.f+2*diff.y)*(1-abs(fase))+2*diff.y));
            faccia=fase>0;
            if(!faccia) numero=rand()%6+1;
        }

        if(faccia)
        {
            PulsantePP1.setFillColor(impozado.colore[1]);
            PulsantePP2.setFillColor(impozado.colore[0]);
            PulsantePP3.setFillColor(impozado.colore[1]);
            etichetta.setFillColor(impozado.colore[0]);
        }


        static bool PulsanteLancioMoneta=0;
        if(PulsantePP1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            PulsantePP1.setFillColor(impozado.colore[1]);
            PulsantePP2.setFillColor(impozado.colore[0]);
            PulsantePP3.setFillColor(impozado.colore[1]);
            etichetta.setFillColor(impozado.colore[0]);

            if(faccia)
            {
                PulsantePP1.setFillColor(impozado.colore[0]);
                PulsantePP2.setFillColor(impozado.colore[1]);
                PulsantePP3.setFillColor(impozado.colore[0]);
                etichetta.setFillColor(impozado.colore[1]);
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) PulsanteLancioMoneta=1;
            else
            {
                if(PulsanteLancioMoneta)
                {
                    PulsanteLancioMoneta=0;
                    tempoLancio=sf::milliseconds(rand()%1000+4000);
                    clock.restart();
                }
            }
        }
        window->draw(PulsantePP1);
        window->draw(PulsantePP2);
        window->draw(PulsantePP3);
        window->draw(etichetta);
    }

    ///-----------------Barra Alta-----------------

    barraB.setFillColor(impozado.colore[0]);
    barraB.setPosition(MargS, 0);
    window->draw(barraB);
    {///Home
        static bool pHome=0;
        Pulsante PHome(" H", MargS, (MargA-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return home;
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
        static bool pHome=0;
        Pulsante PHome(scritta[4], (LarghezzaSchermo-120)/2, (AltezzaSchermo-60)/2, 2, &pHome);
        if(PHome.draw(window)==3) return gioca;
    }

    {///Impostazioni
        static bool pHome=0;
        Pulsante PHome(scritta[5], (LarghezzaSchermo-120)/2, (AltezzaSchermo-60)/2+60, 2, &pHome);
        if(PHome.draw(window)==3) return impostazioni;
    }

    {///Elenco Libri
        static bool pHome=0;
        Pulsante PHome(scritta[6], (LarghezzaSchermo-120)/2, (AltezzaSchermo-60)/2+120, 2, &pHome);
        if(PHome.draw(window)==3) return elencoLibri;
    }

    {///Elenco Giochi
        static bool pHome=0;
        Pulsante PHome(scritta[7], (LarghezzaSchermo-120)/2, (AltezzaSchermo-60)/2+180, 2, &pHome);
        if(PHome.draw(window)==3) return elencoGiochi;
    }
    {///Esci
        static bool pHome=0;
        Pulsante PHome(scritta[8], (LarghezzaSchermo-120)/2, (AltezzaSchermo-60)/2+240, 2, &pHome);
        if(PHome.draw(window)==3)
        {
            window->close();
            impozado.salva();
            return -1;
        }
    }

    return home;
}

int visImpostazioni(sf::RenderWindow* window, sf::Texture intreccio)
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

    {///Lingua

        sf::Text titolo;
        titolo.setFont(impozado.font);
        titolo.setString(scritta[14]);
        titolo.setFillColor(impozado.colore[1]);
        titolo.setPosition(sf::Vector2f(300, 130.f)*prop);
        titolo.setCharacterSize(impozado.AltezzaCarattere+5);
        window->draw(titolo);

        static bool pHome=0;
        Pulsante PHome(impozado.lingvo, 300.f, 180.f, 3, &pHome);
        if(PHome.draw(window)==3)
        {
            int I=0, i=0;
            for(; i<50 && impozado.lingue_disponibili[i]!=""; i++)
                if(impozado.lingue_disponibili[i]==impozado.lingvo)
                    I=i;
            impozado.lingvo=impozado.lingue_disponibili[(I+1)%i];
        }
    }

    Barra bVolume("%", 500.f, 180.f, 3, impozado.volume, 100);
    impozado.volume=bVolume.draw(window);

    {///Colori
        sf::Vector2f pos(300.f, 250.f);
        sf::Vector2f diff(5.f, 5.f);
        sf::Vector2f larg(60.f, 60.f);
        static int indColore=0;
        for(int i=0; i<3; i++)
            {//C1
            sf::RectangleShape PulsanteH1(larg*prop);
            PulsanteH1.setPosition((pos+(sf::Vector2f(larg.x-diff.x, 0))*float(i))*prop);
            PulsanteH1.setFillColor(impozado.colore[1]);

            sf::RectangleShape PulsanteH2((larg-2.f*diff)*prop);
            PulsanteH2.setPosition((pos+(sf::Vector2f(larg.x-diff.x, 0))*float(i)+diff)*prop);
            PulsanteH2.setFillColor(impozado.colore[i]);

            static bool PulsanteHomePremuto=0;
            if(PulsanteH1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                window->draw(PulsanteH1);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) PulsanteHomePremuto=1;
                else
                {
                    if(PulsanteHomePremuto)
                    {
                        PulsanteHomePremuto=0;
                        indColore=i;
                    }
                }
            }
            if(indColore==i)    window->draw(PulsanteH1);
            window->draw(PulsanteH2);
        }

            {///R
                Barra bRed("", 300.f, 320.f, 3,impozado.colore[indColore].r, 255);
                impozado.colore[indColore].r=bRed.draw(window);
            }
            {///G
                Barra bGreen("", 300.f, 390.f, 3,impozado.colore[indColore].g, 255);
                impozado.colore[indColore].g=bGreen.draw(window);
            }
            {///B
                Barra bBlue("", 300.f, 460.f, 3,impozado.colore[indColore].b, 255);
                impozado.colore[indColore].b=bBlue.draw(window);
            }
    }

    {///reset
        static bool pHome=0;
        Pulsante PHome(scritta[16], 500.f, 560.f, 3, &pHome);
        if(PHome.draw(window)==3)
        {
            Impostazioni i;
            i.salva();
            impozado.scarica();
        }
    }


    {///Home
        static bool pHome=0;
        Pulsante PHome(" H", MargS, (MargA-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return home;
    }

    caricaLingua();
    return impostazioni;
}

int visElencoLibri(sf::RenderWindow* window, sf::Texture intreccio, string pagina[10000], string nomePagina[10000], Sezione sezione[100], int &nPagine, int &nSez, string &testo)
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

    {//home
        static bool pHome=0;
        Pulsante PHome(" H", MargS, (MargA-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return home;
    }


    ///carica elenco libri
    ifstream fin;
    fin.open("libroj/enhavo.txt");
    string titolo[100];
    int i=0;
    while(!fin.eof() && i<100)
    {
        fin>>titolo[i];
        i++;
    }
    ///visualizza
    for(int j=0; j<i; j++)
    {
        static bool pLibro=0;
        Pulsante PLibro(titolo[j], (LarghezzaSchermo-120)/2, 2*MargA+60*j, 2, &pLibro);
        if(PLibro.draw(window)==3)
        {
            testo=caricaLibro(titolo[j]);
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
            nSez=disseziona(pagina[partita.indPagina], sezione);
            return gioca;
        }
    }
    return elencoLibri;
}

int visElencoGiochi(sf::RenderWindow* window, sf::Texture intreccio)
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

    sf::Text WIP;
    WIP.setFont(impozado.font);
    WIP.setString("WORK IN PROGRESS");
    WIP.setFillColor(impozado.colore[1]);
    WIP.setCharacterSize(impozado.AltezzaCarattere+5);
    WIP.setPosition((LarghezzaSchermo-WIP.getGlobalBounds().width)/2.f, AltezzaSchermo/3.f);
    window->draw(WIP);

    static bool pHome=0;
    Pulsante PHome(" H", MargS, (MargA-60)/2, 1, &pHome);
    if(PHome.draw(window)==3) return home;

    return elencoGiochi;
}

void visualizza(sf::RenderWindow* window, int &schermata, Sezione sezione[], sf::Texture intreccio, string pagina[10000], string nomePagina[10000], int &nPagine, int &nSez, string &testo, int scroll)
{
    switch(schermata)
    {
        case home:
            schermata=visHome(window, intreccio);
        break;

        case gioca:
            schermata=visGioca(window, sezione, nSez, intreccio, nomePagina, scroll);
        break;

        case impostazioni:
            schermata=visImpostazioni(window, intreccio);
        break;

        case elencoLibri:
            schermata=visElencoLibri(window, intreccio, pagina, nomePagina, sezione, nPagine, nSez, testo);
        break;

        case elencoGiochi:
            schermata=visElencoGiochi(window, intreccio);
        break;
    }
}

void azionaIpertesto(Sezione sezione[], int nSez, sf::RenderWindow* window, int scroll)
{
    static bool premuto[1000]={};
    for(int i=0; i<nSez; i++)
    {
        switch(sezione[i].tipo)
        {
            case 'l':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*impozado.AltezzaCarattere/4.f, sezione[i].lung*impozado.AltezzaCarattere/2, impozado.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        partita.indPagina=sezione[i].arg;
                    }
                }
                else
                {
                    sezione[i].stile=sf::Text::Regular;
                    sezione[i].colore=2;
                    premuto[i]=0;
                }
            break;

            case 'n':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*impozado.AltezzaCarattere/4.f, sezione[i].lung*impozado.AltezzaCarattere/2, impozado.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        bool presente=0;
                        for(int j=0; j<partita.nNote; j++)
                            presente=presente || (partita.nota[j]==sezione[i].arg);
                        if(!presente)
                        {
                            partita.nota[partita.nNote]=sezione[i].arg;
                            partita.nNote++;
                        }
                    }
                }
                else
                {
                    sezione[i].stile=sf::Text::Regular;
                    sezione[i].colore=2;
                    premuto[i]=0;
                }
            break;

            case 's':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*impozado.AltezzaCarattere/4.f, sezione[i].lung*impozado.AltezzaCarattere/2, impozado.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(premuto[i])
                    {
                        if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            premuto[i]=0;
                            partita.sorte+=sezione[i].arg;
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
                    sezione[i].colore=2;
                    premuto[i]=0;
                }
            break;

            case 'b':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*impozado.AltezzaCarattere/4.f, sezione[i].lung*impozado.AltezzaCarattere/2, impozado.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(premuto[i])
                    {
                        if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            premuto[i]=0;
                            partita.barre+=sezione[i].arg;
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
                    sezione[i].colore=2;
                    premuto[i]=0;
                }
            break;

            case 'a':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*impozado.AltezzaCarattere/4.f, sezione[i].lung*impozado.AltezzaCarattere/2, impozado.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(premuto[i])
                    {
                        if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            premuto[i]=0;
                            partita.barre=sezione[i].arg;
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
                    sezione[i].colore=2;
                    premuto[i]=0;
                }
            break;
        }
    }
}


int main()
{
    impozado.scarica();
    caricaLingua();

    sf::RenderWindow window(sf::VideoMode(LarghezzaSchermo, AltezzaSchermo), "Display");
    string testo="#0/";

    Sezione sezione[100];
    int nSez=0;

    string nomePagina[1000];
    string pagina[1000];
    int nPagine=0;

    int scroll=0;

    int schermata=home, schermataS=home;
    sf::Vector2f dimIntrSchermata[5];
    sf::Texture intreccioR[5];
    sf::Texture intreccio;

    {
        window.setPosition(sf::Vector2i(-10, 0));

        window.clear(impozado.colore[0]);
        sf::Text testoCaricamento;
        testoCaricamento.setFont(impozado.font);
        testoCaricamento.setString(scritta[0]+"\n"+scritta[1]);
        testoCaricamento.setFillColor(impozado.colore[1]);
        testoCaricamento.setPosition((LarghezzaSchermo-testoCaricamento.getGlobalBounds().width)/2, (AltezzaSchermo-testoCaricamento.getGlobalBounds().height)/2);
        window.draw(testoCaricamento);


        sf::RectangleShape barraA(sf::Vector2f(LarghezzaSchermo/3.f, AltezzaSchermo/15.f));
        barraA.setFillColor(impozado.colore[1]);
        barraA.setPosition(LarghezzaSchermo/3.f, AltezzaSchermo*12/15.f);
        window.draw(barraA);

        sf::RectangleShape barraB(sf::Vector2f(LarghezzaSchermo/3.f-8, AltezzaSchermo/15.f-8));
        barraB.setFillColor(impozado.colore[0]);
        barraB.setPosition(LarghezzaSchermo/3.f+4, AltezzaSchermo*12/15.f+4);
        window.draw(barraB);

        sf::RectangleShape barraC(sf::Vector2f(LarghezzaSchermo/3.f-14, AltezzaSchermo/15.f-14));
        barraC.setFillColor(impozado.colore[1]);
        barraC.setPosition(LarghezzaSchermo/3.f+7, AltezzaSchermo*12/15.f+7);
        barraC.setScale(1/16.f, 1);
        window.draw(barraC);
        window.display();

        testo="#0 "+scritta[2]+"/\n\n"+scritta[3];
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
        nSez=disseziona(pagina[partita.indPagina], sezione);



        dimIntrSchermata[home]=sf::Vector2f(MargS*15/10, AltezzaSchermo);
        dimIntrSchermata[gioca]=sf::Vector2f(MargS*9/10, AltezzaSchermo);
        dimIntrSchermata[impostazioni]=sf::Vector2f(MargS*9/10, AltezzaSchermo);
        dimIntrSchermata[elencoLibri]=sf::Vector2f(MargS*9/10, AltezzaSchermo);
        dimIntrSchermata[elencoGiochi]=sf::Vector2f(MargS*9/10, AltezzaSchermo);

        intreccio=disegnaIntreccio(dimIntrSchermata[home]);
        barraC.setScale(2/8.f, 1);
        window.clear(impozado.colore[0]);
            window.draw(testoCaricamento);
            window.draw(barraA);
            window.draw(barraB);
            window.draw(barraC);
            window.draw(testoCaricamento);
            window.display();

        for(int i=0; i<5; i++)
        {
            intreccioR[i]=disegnaIntreccio(dimIntrSchermata[i]);
            window.clear(impozado.colore[0]);
            barraC.setScale((i+3)/8.f, 1);
            window.draw(testoCaricamento);
            window.draw(barraA);
            window.draw(barraB);
            window.draw(barraC);
            window.draw(testoCaricamento);
            window.display();
        }
    }


    bool rilascio=1;
    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                impozado.salva();
            }

            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                scroll=min(event.mouseWheelScroll.delta+scroll, 0.f);
            }
        }
        window.clear(impozado.colore[0]);
        if(indCP<0)
        {
            indCP=0;
            nCP=1;
            partita.cronoPagine[indCP]=0;
            nSez=disseziona(pagina[partita.indPagina], sezione);
        }
        bool cambioPagina=0;
        if(partita.cronoPagine[indCP]!=partita.indPagina)
        {
            cambioPagina=1;
            rilascio=0;
            indCP++;
            nCP=indCP+1;
            partita.cronoPagine[indCP]=partita.indPagina;
            nSez=disseziona(pagina[partita.indPagina], sezione);

            scroll=0;
        }

        if(cambioPagina || schermata!=schermataS)
        {
            schermataS=schermata;
            intreccio=intreccioR[schermata];
            if(rilascio) azionaIpertesto(sezione, nSez, &window, scroll);
            visualizza(&window, schermata, sezione, intreccio, pagina, nomePagina, nPagine, nSez, testo, scroll);
            window.display();
            intreccioR[schermata]=disegnaIntreccio(dimIntrSchermata[schermata]);
        }
        else
        {
            if(rilascio) azionaIpertesto(sezione, nSez, &window, scroll);
            visualizza(&window, schermata, sezione, intreccio, pagina, nomePagina, nPagine, nSez, testo, scroll);
            window.display();
        }
        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) rilascio=1;
    }

    return 0;
}
