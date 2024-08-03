#include <time.h>
#include <cmath>

#include "GUI.h"
#include "knots.h"
#include "parser.h"

#include <X11/Xlib.h>//portability issue



//Paragrafo: Azzera_barre Barra X(normale) Link Colore Nota Oggetto Sorte

int indCP=0, nCP=0;

enum {IdHome, IdPlay, IdSettings, IdBooks, IdGame};

class Partita{
    public:
    int indPagina=0;
    int cronoPagine[10000]={0};

    int nota[100];
    int nNote=0;

    int sorte=2;
    int barre=0;
}partita;

wstring scritta[100];



wstring X2EO(string X){
    wstring C = L"";
    wstring sost[256] = {}; // eĥoŝanĝo ĉiuĵaŭde
    for(int i='A'; i<'z'; i++) {string tmp="a"; tmp[0]=(char) i; sost[i]=Str2Wstr(tmp);}
    sost['c'] = L"ĉ";
    sost['g'] = L"ĝ";
    sost['j'] = L"ĵ";
    sost['s'] = L"ŝ";
    sost['h'] = L"ĥ";
    sost['u'] = L"ŭ";
    sost['C'] = L"Ĉ";
    sost['G'] = L"Ĝ";
    sost['J'] = L"Ĵ";
    sost['S'] = L"Ŝ";
    sost['H'] = L"Ĥ";
    sost['U'] = L"Ŭ";

    for (size_t i = 0; i < X.size(); i++)
    {
        if (i < X.size() - 1 && X[i + 1] == 'x')
        {
            C += sost[(int)X[i]];
            i++; // Skip the 'x'
        }
        else
        {
            C += Str2Wstr(X.substr(i, 1));
        }
    }

    return C;
}

void caricaLingua()
{
    ifstream fin("labels/"+settings.language+".txt");//portability issue
    
    for(int i=0; i<100 && !fin.eof(); i++)
    {
        string tmp;
        getline(fin, tmp);
        if(settings.language=="Esperanto")
        {
            scritta[i]=X2EO(tmp);
        }
        else
        {
            scritta[i]=Str2Wstr(tmp);
        }
    }
}

int visGioca(sf::RenderWindow* window, Sezione sezione[], int nSez, sf::Texture intreccio, string nomePagina[10000], int scroll)
{
	string linguaLibro="";
    float lC[256]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.2, 1.2, 1.1, 1.1, 2, 1.3, 1.3, 1.4, 1.4, 1.4, 1.2, 1.5, 1.4, 0.5, 1, 1.3, 1.1, 1.6, 1.4, 1.5, 1.2, 1.5, 1.4, 1.3, 1.2, 1.5, 1.3, 1.9, 1.3, 1.3, 1.2, 0.5, 0.5, 0.5, 0.9, 1.1, 0.7, 1, 1, 1, 1.1, 1.1, 0.6, 1.1, 1.1, 0.4, 0.4, 1, 0.4, 1.6, 1.1, 1.1, 1.1, 1, 0.7, 0.9, 0.6, 1.1, 1, 1.4, 1, 1, 1, 0.7, 0.5, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	initLC(lC);
    sf::Text testoPagina;
    testoPagina.setFont(settings.font);
    testoPagina.setCharacterSize(settings.AltezzaCarattere);
    testoPagina.setStyle(sf::Text::Regular);

    for(int i=0; i<nSez; i++)
    {
        float c=0;
        testoPagina.setFillColor(settings.colore[sezione[i].colore]);
        testoPagina.setStyle(sezione[i].stile);
        int contX=0;
        for(unsigned int j=0; j<sezione[i].testo.size(); j++)
        {
			if(sezione[i].lingua=="eo")
            {
				if(sezione[i].testo[j]=='x' ){j++; contX++;}
				testoPagina.setString(X2EO(sezione[i].testo).substr(j-contX, 1));
			}
			else
			{
				testoPagina.setString(sezione[i].testo.substr(j, 1));
			}
            testoPagina.setPosition(sezione[i].posizione+sf::Vector2f(c*settings.AltezzaCarattere, settings.AltezzaCarattere*scroll/4.f));
            window->draw(testoPagina);
            c+=lC[(int) sezione[i].testo[j]];
        }
    }


    ///---------------Sorto-----------------------
    {

    sf::Text testoSorte;
    testoSorte.setFillColor(settings.colore[1]);
    testoSorte.setFont(settings.font);
    testoSorte.setCharacterSize(settings.AltezzaCarattere);
    testoSorte.setStyle(sf::Text::Bold);

    testoSorte.setString(scritta[9]+L": "+Str2Wstr(to_string(partita.sorte)));
    testoSorte.setPosition(sf::Vector2f(MarginSize.x*10/50, MarginSize.y/2)*prop);
    window->draw(testoSorte);

    static bool premutoP=0;

    testoSorte.setString("+");
    testoSorte.setPosition(sf::Vector2f(MarginSize.x*30/50, MarginSize.y/2)*prop);
    if(testoSorte.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSorte.setFillColor(settings.colore[1]);
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
        testoSorte.setFillColor(settings.colore[2]);
        premutoP=0;
    }
    window->draw(testoSorte);


    testoSorte.setPosition(sf::Vector2f(MarginSize.x*35/50, MarginSize.y/2)*prop);
    static bool premutoM=0;
    if(testoSorte.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSorte.setFillColor(settings.colore[1]);
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
        testoSorte.setFillColor(settings.colore[2]);
        premutoM=0;
    }
    testoSorte.setString("-");
    window->draw(testoSorte);

    }

    ///-----------------strekoj------------------
    {

    sf::Text testoSbarre;
    testoSbarre.setFillColor(settings.colore[1]);
    testoSbarre.setFont(settings.font);
    testoSbarre.setCharacterSize(settings.AltezzaCarattere);
    testoSbarre.setStyle(sf::Text::Bold);

    testoSbarre.setPosition(sf::Vector2f(MarginSize.x*10/50*prop.x, MarginSize.y/2*prop.y+settings.AltezzaCarattere*1.5f));
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
    testoSbarre.setPosition(sf::Vector2f(MarginSize.x*30/50*prop.x, MarginSize.y/2*prop.y+settings.AltezzaCarattere*1.5f));
    if(testoSbarre.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSbarre.setFillColor(settings.colore[1]);
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
        testoSbarre.setFillColor(settings.colore[2]);
        premutoP=0;
    }
    window->draw(testoSbarre);


    testoSbarre.setPosition(sf::Vector2f(MarginSize.x*35/50*prop.x, MarginSize.y/2*prop.y+settings.AltezzaCarattere*1.5f));
    static bool premutoM=0;
    if(testoSbarre.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSbarre.setFillColor(settings.colore[1]);
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
        testoSbarre.setFillColor(settings.colore[2]);
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
    testoNote.setFont(settings.font);
    testoNote.setCharacterSize(settings.AltezzaCarattere);
    testoNote.setStyle(sf::Text::Bold);

    testoNote.setString(L"------"+scritta[10]+L"-----");
    testoNote.setFillColor(settings.colore[1]);
    testoNote.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*4/5, MarginSize.y/2));
    window->draw(testoNote);

    testoNote.setStyle(sf::Text::Regular);
    static int notaAperta=partita.nNote;
    for(int i=0; i<partita.nNote; i++)
    {
        testoNote.setFillColor(settings.colore[2]);
        testoNote.setString(nomePagina[partita.nota[i]]+" ["+to_string(partita.nota[i])+"]");
        testoNote.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*4/5*prop.x, MarginSize.y*prop.y+(i+(notaAperta<i))*settings.AltezzaCarattere));
        if(testoNote.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            testoNote.setFillColor(settings.colore[1]);
            notaAperta=i;
        }
        if(notaAperta==i)
        {
            sf::Vector2f pos(testoNote.getGlobalBounds().left, testoNote.getGlobalBounds().top), siz(MarginSize.x*8/10*prop.x, testoNote.getGlobalBounds().height*2);
            if(sf::FloatRect(pos, siz).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
            {
                testoNote.setFillColor(settings.colore[1]);
                {//opzioni nota

                    sf::Text testoOpzioni;
                    testoOpzioni.setFont(settings.font);
                    testoOpzioni.setCharacterSize(settings.AltezzaCarattere*4/5);
                    testoOpzioni.setStyle(sf::Text::Regular);

                    //vidu
                    testoOpzioni.setString(scritta[11]);
                    testoOpzioni.setFillColor(settings.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*4/5*prop.x, MarginSize.y*prop.y+(i+1)*settings.AltezzaCarattere));

                    static bool viduPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(settings.colore[1]);
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
                    testoOpzioni.setFillColor(settings.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*prop.x*4/5+3*settings.AltezzaCarattere, MarginSize.y*prop.y+(i+1)*settings.AltezzaCarattere));

                    static bool uzuPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(settings.colore[1]);
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
                    testoOpzioni.setFillColor(settings.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*prop.x*4/5+5*settings.AltezzaCarattere, MarginSize.y*prop.y+(i+1)*settings.AltezzaCarattere));

                    static bool visxuPremuto=0;
                    if(testoOpzioni.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        testoOpzioni.setStyle(sf::Text::Bold);
                        testoOpzioni.setFillColor(settings.colore[1]);
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
    barraD.setPosition(ScreenSize.x, 0);
    if(!barraD.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        window->draw(barraD);

    ///-----------------Barra Bassa-----------------

    sf::RectangleShape barraB(sf::Vector2f(ScreenSize.x-2*MarginSize.x*prop.x, MarginSize.y*prop.y));
    barraB.setFillColor(settings.colore[0]);
    barraB.setPosition(MarginSize.x*prop.x, ScreenSize.y-MarginSize.y*prop.y);
    window->draw(barraB);

    {///Paragrafo precedente
        static bool pHome=0;
        Pulsante PHome(L"<<  ", MarginSize.x, ScreenSize.y/prop.y-(MarginSize.y+60.f)/2, 1, &pHome);
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
        Pulsante PHome(L"_ _ _", MarginSize.x+70, ScreenSize.y/prop.y-(MarginSize.y+60.f)/2, 2, &pHome);

        if(staDigitando)
        {
            PHome.etic=L" "+Str2Wstr(to_string((int) prossimoParagrafo/100))+L" "+Str2Wstr(to_string(((int) prossimoParagrafo%100)/10))+L" "+Str2Wstr(to_string((int) prossimoParagrafo%10));

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
    /*Moneta
    {///Lancio moneta
        static bool faccia=0;
        static int numero=0;
        static sf::Time tempoLancio;
        static sf::Clock clock;

        sf::Vector2f pos(ScreenSize.x-(MarginSize.x-60)*prop, ScreenSize.y-((MarginSize.y*9/10+60.f)*prop)/2);
        sf::Vector2f diff(3.f, 3.f);

        sf::CircleShape PulsantePP1(32.f*prop);
        PulsantePP1.setPosition(pos*prop);
        PulsantePP1.setFillColor(settings.colore[0]);


        sf::CircleShape PulsantePP2((32.f-diff.x)*prop);
        PulsantePP2.setPosition((pos+diff)*prop);
        PulsantePP2.setFillColor(settings.colore[1]);

        sf::CircleShape PulsantePP3((32.f-2*diff.x)*prop);
        PulsantePP3.setPosition((pos+2.f*diff)*prop);
        PulsantePP3.setFillColor(settings.colore[0]);

        sf::Text etichetta;
        etichetta.setFont(settings.font);
        etichetta.setString(" M");
        if(faccia) etichetta.setString(" "+to_string(numero));
        etichetta.setFillColor(settings.colore[1]);
        etichetta.setPosition((pos+4.f*diff)*prop);
        etichetta.setCharacterSize(settings.AltezzaCarattere+5);

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
            PulsantePP1.setFillColor(settings.colore[1]);
            PulsantePP2.setFillColor(settings.colore[0]);
            PulsantePP3.setFillColor(settings.colore[1]);
            etichetta.setFillColor(settings.colore[0]);
        }


        static bool PulsanteLancioMoneta=0;
        if(PulsantePP1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        {
            PulsantePP1.setFillColor(settings.colore[1]);
            PulsantePP2.setFillColor(settings.colore[0]);
            PulsantePP3.setFillColor(settings.colore[1]);
            etichetta.setFillColor(settings.colore[0]);

            if(faccia)
            {
                PulsantePP1.setFillColor(settings.colore[0]);
                PulsantePP2.setFillColor(settings.colore[1]);
                PulsantePP3.setFillColor(settings.colore[0]);
                etichetta.setFillColor(settings.colore[1]);
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
    //*/
    ///-----------------Barra Alta-----------------

    barraB.setFillColor(settings.colore[0]);
    barraB.setPosition(MarginSize.x*prop.x, 0);
    window->draw(barraB);
    {///Home
        static bool pHome=0;
        Pulsante PHome(L"H ", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return IdHome;
    }

    return IdPlay;
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
    barraD.setPosition(ScreenSize.x, 0);
    window->draw(barraD);

    {///Gioca
        static bool pHome=0;
        Pulsante PHome(scritta[4], (1200-180)/2, (600-60)/2, 3, &pHome);
        if(PHome.draw(window)==3) return IdPlay;
    }

    {///Impostazioni
        static bool pHome=0;
        Pulsante PHome(scritta[5], (1200-180)/2, (600-60)/2+60, 3, &pHome);
        if(PHome.draw(window)==3) return IdSettings;
    }

    {///Elenco Libri
        static bool pHome=0;
        Pulsante PHome(scritta[6], (1200-180)/2, (600-60)/2+120, 3, &pHome);
        if(PHome.draw(window)==3) return IdBooks;
    }

    {///Elenco Giochi
        static bool pHome=0;
        Pulsante PHome(scritta[7], (1200-180)/2, (600-60)/2+180, 3, &pHome);
        if(PHome.draw(window)==3) return IdGame;
    }
    {///Esci
        static bool pHome=0;
        Pulsante PHome(scritta[8], (1200-180)/2, (600-60)/2+240, 3, &pHome);
        if(PHome.draw(window)==3)
        {
            window->close();
            settings.salva();
            return -1;
        }
    }

    return IdHome;
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
    barraD.setPosition(ScreenSize.x, 0);
    window->draw(barraD);

    {///Lingua

        sf::Text titolo;
        titolo.setFont(settings.font);
        titolo.setString(scritta[14]);
        titolo.setFillColor(settings.colore[1]);
        titolo.setPosition(sf::Vector2f(MarginSize.x*10.f/9.f, 130.f)*prop);
        titolo.setCharacterSize(settings.AltezzaCarattere+5);
        window->draw(titolo);

        static bool pHome=0;
        Pulsante PHome(Str2Wstr(settings.language), MarginSize.x*10.f/9.f, 180.f, 3, &pHome);
        if(PHome.draw(window)==3)
        {
            int I=0, i=0;
            for(; i<50 && settings.availableLang[i]!=""; i++)
                if(settings.availableLang[i]==settings.language)
                    I=i;
            settings.language=settings.availableLang[(I+1)%i];
        }
    }
    
    {///Font

        sf::Text titolo;
        titolo.setFont(settings.font);
        titolo.setString(scritta[17]);
        titolo.setFillColor(settings.colore[1]);
        titolo.setPosition(sf::Vector2f(MarginSize.x*20.f/9.f, 130.f)*prop);
        titolo.setCharacterSize(settings.AltezzaCarattere+5);
        window->draw(titolo);

        static bool pHome=0;
        Pulsante PHome(Str2Wstr(settings.fontName.substr(0, settings.fontName.size()-4)), MarginSize.x*20.f/9.f, 180.f, 3, &pHome);
        if(PHome.draw(window)==3)
        {
            int I=0, i=0;
            for(; i<50 && settings.availableFont[i]!=""; i++)
                if(settings.availableFont[i]==settings.fontName)
                    I=i;
            settings.fontName=settings.availableFont[(I+1)%i];
            settings.font.loadFromFile("fonts/"+settings.fontName);//portability issue
        }
    }
	/*
	{//Volume
		sf::Text titolo;
        titolo.setFont(settings.font);
        titolo.setString(scritta[15]);
        titolo.setFillColor(settings.colore[1]);
        titolo.setPosition(sf::Vector2f(MarginSize.x*20.f/9.f, 130.f)*prop);
        titolo.setCharacterSize(settings.AltezzaCarattere+5);
        window->draw(titolo);
		
		Barra bVolume(L"%", MarginSize.x*20.f/9.f, 180.f, 3, settings.volume, 100);
		settings.volume=bVolume.draw(window);
	}
	//*/
    {///Colori
        sf::Vector2f pos(sf::Vector2f(MarginSize.x*10.f/9.f, 250.f));
        sf::Vector2f diff(5.f, 5.f);
        sf::Vector2f larg(60.f, 60.f);
        static int indColore=0;
        for(int i=0; i<3; i++)
            {//C1
            sf::RectangleShape PulsanteH1(larg*prop);
            PulsanteH1.setPosition((pos+(sf::Vector2f(larg.x-diff.x, 0))*float(i))*prop);
            PulsanteH1.setFillColor(settings.colore[1]);

            sf::RectangleShape PulsanteH2((larg-2.f*diff)*prop);
            PulsanteH2.setPosition((pos+(sf::Vector2f(larg.x-diff.x, 0))*float(i)+diff)*prop);
            PulsanteH2.setFillColor(settings.colore[i]);

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
                Barra bRed(L"", MarginSize.x*10.f/9.f, 320.f, 3,settings.colore[indColore].r, 255);
                settings.colore[indColore].r=bRed.draw(window);
            }
            {///G
                Barra bGreen(L"", MarginSize.x*10.f/9.f, 390.f, 3,settings.colore[indColore].g, 255);
                settings.colore[indColore].g=bGreen.draw(window);
            }
            {///B
                Barra bBlue(L"", MarginSize.x*10.f/9.f, 460.f, 3,settings.colore[indColore].b, 255);
                settings.colore[indColore].b=bBlue.draw(window);
            }
    }

    {///reset
        static bool pHome=0;
        Pulsante PHome(scritta[16], MarginSize.x*20.f/9.f, 460.f, 3, &pHome);
        if(PHome.draw(window)==3)
        {
            Impostazioni i;
            i.salva();
            settings.scarica();
        }
    }


    {///Home
        static bool pHome=0;
        Pulsante PHome(L"H ", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return IdHome;
    }

    caricaLingua();
    return IdSettings;
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
    barraD.setPosition(ScreenSize.x, 0);
    window->draw(barraD);

    {//IdHome
        static bool pHome=0;
        Pulsante PHome(L"H ", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return IdHome;
    }


    string titolo[100];
    int i=0;
    for (const auto& entry : filesystem::directory_iterator("./books")) //portability issue
    {
		if (entry.is_regular_file() && entry.path().extension() == ".txt") 
        {
            titolo[i]=entry.path().filename();
            titolo[i]=titolo[i].substr(0, titolo[i].size()-4);
            i++;
        }
    }
    ///visualizza
    for(int j=0; j<i; j++)
    {
        static bool pLibro=0;
        Pulsante PLibro(Str2Wstr(titolo[j]), (1200-120)/2, 2*MarginSize.y+60*j, 2, &pLibro);
        if(PLibro.draw(window)==3)
        {
            testo=caricaLibro(titolo[j]);        
			nPagine=dividiInPagine(pagina, nomePagina, testo);
        	nSez=disseziona(pagina[partita.indPagina], sezione);
            return IdPlay;
        }
    }
    return IdBooks;
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
    barraD.setPosition(ScreenSize.x, 0);
    window->draw(barraD);

    sf::Text WIP;
    WIP.setFont(settings.font);
    WIP.setString("WORK IN PROGRESS");
    WIP.setFillColor(settings.colore[1]);
    WIP.setCharacterSize(settings.AltezzaCarattere+5);
    WIP.setPosition((ScreenSize.x-WIP.getGlobalBounds().width)/2.f, ScreenSize.y/3.f);
    window->draw(WIP);

    static bool pHome=0;
    Pulsante PHome(L"H ", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
    if(PHome.draw(window)==3) return IdHome;

    return IdGame;
}

void visualizza(sf::RenderWindow* window, int &schermata, Sezione sezione[], sf::Texture intreccio, string pagina[10000], string nomePagina[10000], int &nPagine, int &nSez, string &testo, int scroll)
{
    switch(schermata)
    {
        case IdHome:
            schermata=visHome(window, intreccio);
        break;

        case IdPlay:
            schermata=visGioca(window, sezione, nSez, intreccio, nomePagina, scroll);
        break;

        case IdSettings:
            schermata=visImpostazioni(window, intreccio);
        break;

        case IdBooks:
            schermata=visElencoLibri(window, intreccio, pagina, nomePagina, sezione, nPagine, nSez, testo);
        break;

        case IdGame:
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
            //link
            case 'l':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.AltezzaCarattere/4.f, sezione[i].lung*settings.AltezzaCarattere, settings.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
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

            //nota
            case 'n':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.AltezzaCarattere/4.f, sezione[i].lung*settings.AltezzaCarattere, settings.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
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

            //sorte
            case 's':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.AltezzaCarattere/4.f, sezione[i].lung*settings.AltezzaCarattere, settings.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
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

            //aggingi barre
            case 'b':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.AltezzaCarattere/4.f, sezione[i].lung*settings.AltezzaCarattere, settings.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
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

            //setta barre
            case 'a':
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.AltezzaCarattere/4.f, sezione[i].lung*settings.AltezzaCarattere, settings.AltezzaCarattere).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
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
    {
		Display* d = XOpenDisplay(NULL); //portability issue
		Screen*  s = DefaultScreenOfDisplay(d);//portability issue
		ScreenSize=sf::Vector2f(s->width, s->height-60);
		
		prop=sf::Vector2f(ScreenSize.x/1200.f, ScreenSize.y/600.f);
		LarghezzaPagina=(1200.f-2*MarginSize.x)*prop.x;
		settings.AltezzaCarattere*=prop.x;
	}//*/
	
    settings.scarica();
    caricaLingua();


    sf::RenderWindow window(sf::VideoMode(ScreenSize.x, ScreenSize.y), "Display");
    string testo="#0 Nessun libro caricato/";

    Sezione sezione[100];
    int nSez=0;

    string nomePagina[1000];
    string pagina[1000];
    int nPagine=0;

    int scroll=0;

    int schermata=IdHome, schermataS=IdHome;
    sf::Vector2f dimIntrSchermata[5];
    sf::Texture intreccioR[5];
    sf::Texture intreccio;
	
    {
        window.setPosition(sf::Vector2i(-10, 0));

        window.clear(settings.colore[0]);
        sf::Text testoCaricamento;
        testoCaricamento.setFont(settings.font);
        testoCaricamento.setString(scritta[0]+L"\n"+scritta[1]);
        testoCaricamento.setFillColor(settings.colore[1]);
        testoCaricamento.setPosition((ScreenSize.x-testoCaricamento.getGlobalBounds().width)/2, (ScreenSize.y-testoCaricamento.getGlobalBounds().height)/2);
        window.draw(testoCaricamento);


        sf::RectangleShape barraA(sf::Vector2f(ScreenSize.x/3.f, ScreenSize.y/15.f));
        barraA.setFillColor(settings.colore[1]);
        barraA.setPosition(ScreenSize.x/3.f, ScreenSize.y*12/15.f);
        window.draw(barraA);

        sf::RectangleShape barraB(sf::Vector2f(ScreenSize.x/3.f-8, ScreenSize.y/15.f-8));
        barraB.setFillColor(settings.colore[0]);
        barraB.setPosition(ScreenSize.x/3.f+4, ScreenSize.y*12/15.f+4);
        window.draw(barraB);

        sf::RectangleShape barraC(sf::Vector2f(ScreenSize.x/3.f-14, ScreenSize.y/15.f-14));
        barraC.setFillColor(settings.colore[1]);
        barraC.setPosition(ScreenSize.x/3.f+7, ScreenSize.y*12/15.f+7);
        barraC.setScale(1/16.f, 1);
        window.draw(barraC);
        window.display();
		
		
		{
		string titolo;
		for (const auto& entry : filesystem::directory_iterator("./books")) //portability issue
		{
			if (entry.is_regular_file() && entry.path().extension() == ".txt") 
			{
				titolo=entry.path().filename();
				titolo=titolo.substr(0, titolo.size()-4);
				break;
			}
		}
		testo=caricaLibro(titolo);
        nPagine=dividiInPagine(pagina, nomePagina, testo);
        nSez=disseziona(pagina[partita.indPagina], sezione);
		}
		
		
		
        dimIntrSchermata[IdHome]=sf::Vector2f(MarginSize.x*14/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdPlay]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdSettings]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdBooks]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdGame]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);

        intreccio=disegnaIntreccio(dimIntrSchermata[IdHome]);
        barraC.setScale(2/8.f, 1);
        window.clear(settings.colore[0]);
            window.draw(testoCaricamento);
            window.draw(barraA);
            window.draw(barraB);
            window.draw(barraC);
            window.draw(testoCaricamento);
            window.display();

        for(int i=0; i<5; i++)
        {
            intreccioR[i]=disegnaIntreccio(dimIntrSchermata[i]);
            window.clear(settings.colore[0]);
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
                settings.salva();
            }

            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                scroll=min(event.mouseWheelScroll.delta+scroll, 0.f);
            }
        }
        window.clear(settings.colore[0]);
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