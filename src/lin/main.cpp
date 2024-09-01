#include <time.h>
#include <cmath>

#include "GUI.h"
#include "knots.h"
#include "parser.h"

#include <X11/Xlib.h>//portability issue



//Paragrafo: Azzera_barre Barra X(normale) Link Colore Nota Oggetto Sorte



enum {IdHome, IdPlay, IdSettings, IdBooks, IdGame, IdRules};


bool isNum(string s)
{
		for(int i=0; i<s.size(); i++)
		{
			if(!(s[i]<='9' && s[i]>='0'))	
				return 0;
		}
		return 1;
}

class Game{
    public:
    string nome="";
    string libro="";
    
    int indPagina=0;
    
    int cronoPagine[10000]={0};
    int indCP=0;

    int nota[100];
    int nNote=0;

    int sorte=0;
    int barre=0;
    
	void save()
	{		
		ofstream fout("games/"+nome+".txt");
		fout<<sorte<<endl;
		fout<<barre<<endl;
		fout<<nNote<<endl;
		fout<<indPagina<<endl;
		fout<<indCP<<endl;
		fout<<endl;
		for(int i=0; i<indCP+1; i++)
			fout<<cronoPagine[i]<<endl;
		fout<<endl;
		for(int i=0; i<nNote; i++)
			fout<<nota[i]<<endl;
	};
	void load(string inpNome)
	{
		
		if(indCP!=0)
			save();
			
		nome=inpNome;
		int limTitolo;
		limTitolo=nome.find("-");
		libro=nome.substr(0, limTitolo);
		string tmp;
		ifstream fin("games/"+nome+".txt");
		fin>>sorte;
		fin>>barre;
		fin>>nNote;
		fin>>indPagina;
		fin>>indCP;
		getline(fin, tmp);
		for(int i=0; i<indCP+1; i++)
			fin>>cronoPagine[i];
		getline(fin, tmp);
		for(int i=0; i<nNote; i++)
			fin>>nota[i];
	};
    void newGame(string inpLibro)
    {
		if(indCP!=0)
			save();
		
		libro=inpLibro;
		int indLibro=0;
		string titolo[100];
		int i=0;
		
		for (const auto& entry : filesystem::directory_iterator("./games")) //portability issue
		{
			if (entry.is_regular_file() && entry.path().extension()==".txt") 
			{
				
				titolo[i]=entry.path().filename();
				titolo[i]=titolo[i].substr(0, titolo[i].size()-4);
				int tmpInd=0;
				int tmpLim=titolo[i].find("-");
				string tmpTitolo=titolo[i];
				
				if(tmpLim!=string::npos)
				{
					tmpTitolo=titolo[i].substr(0, tmpLim);
					string tmpInt=titolo[i].substr(tmpLim+1, titolo[i].size()-tmpLim);
					
					if(isNum(tmpInt))
						tmpInd=stoi(tmpInt);
					else 
						tmpTitolo=titolo[i];	
				}
				cout<<tmpTitolo<<endl;
				if(tmpTitolo==libro)	
					indLibro=max(indLibro, tmpInd+1);
				i++;
			}
		}
		
		nome=inpLibro+"-"+to_string(indLibro);
		cout<<endl<<nome<<endl;
		sorte=0;
		barre=0;
		indPagina=0;
		cronoPagine[0]=0;
		indCP=0;
		nNote=0;
	};
}game;

wstring scritta[100];

void exit(sf::RenderWindow* window)
{
	window->close();
	settings.salva();
	game.save();
}

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
    testoPagina.setCharacterSize(settings.mainCharSize);
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
            testoPagina.setPosition(sezione[i].posizione+sf::Vector2f(c*settings.mainCharSize, settings.mainCharSize*scroll/4.f));
            window->draw(testoPagina);
            c+=lC[(int) sezione[i].testo[j]];
        }
    }


    ///---------------Sorto-----------------------
    {

    sf::Text testoSorte;
    testoSorte.setFillColor(settings.colore[1]);
    testoSorte.setFont(settings.font);
    testoSorte.setCharacterSize(settings.mainCharSize);
    testoSorte.setStyle(sf::Text::Bold);

    testoSorte.setString(scritta[9]+L": "+Str2Wstr(to_string(game.sorte)));
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
                game.sorte++;
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
                game.sorte=max(0, game.sorte-1);
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
    testoSbarre.setCharacterSize(settings.mainCharSize);
    testoSbarre.setStyle(sf::Text::Bold);

    testoSbarre.setPosition(sf::Vector2f(MarginSize.x*10/50*prop.x, MarginSize.y/2*prop.y+settings.mainCharSize*1.5f));
    string txtSbarre="";
    for(int i=0; i<game.barre; i++)
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
    testoSbarre.setPosition(sf::Vector2f(MarginSize.x*30/50*prop.x, MarginSize.y/2*prop.y+settings.mainCharSize*1.5f));
    if(testoSbarre.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSbarre.setFillColor(settings.colore[1]);
        if(premutoP)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoP=0;
                game.barre++;
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


    testoSbarre.setPosition(sf::Vector2f(MarginSize.x*35/50*prop.x, MarginSize.y/2*prop.y+settings.mainCharSize*1.5f));
    static bool premutoM=0;
    if(testoSbarre.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
    {
        testoSbarre.setFillColor(settings.colore[1]);
        if(premutoM)
        {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                premutoM=0;
                game.barre=max(0, game.barre-1);
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
    testoNote.setCharacterSize(settings.mainCharSize);
    testoNote.setStyle(sf::Text::Bold);

    testoNote.setString(scritta[10]);
    testoNote.setFillColor(settings.colore[1]);
    testoNote.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x+(MarginSize.x-testoNote.getGlobalBounds().width)/2.f, MarginSize.y/2));
    window->draw(testoNote);

    testoNote.setStyle(sf::Text::Regular);
    static int notaAperta=game.nNote;
    for(int i=0; i<game.nNote; i++)
    {
        testoNote.setFillColor(settings.colore[2]);
        testoNote.setString(nomePagina[game.nota[i]]+" ["+to_string(game.nota[i])+"]");
        testoNote.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*4/5*prop.x, MarginSize.y*prop.y+(i+(notaAperta<i))*settings.mainCharSize));
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
                    testoOpzioni.setCharacterSize(settings.mainCharSize*4/5);
                    testoOpzioni.setStyle(sf::Text::Regular);

                    //vidu
                    testoOpzioni.setString(scritta[11]);
                    testoOpzioni.setFillColor(settings.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*4/5*prop.x, MarginSize.y*prop.y+(i+1)*settings.mainCharSize));

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
                                game.indPagina=game.nota[i];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                    //uzu
                    testoOpzioni.setString(scritta[12]);
                    testoOpzioni.setStyle(sf::Text::Regular);
                    testoOpzioni.setFillColor(settings.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*prop.x*4/5+3*settings.mainCharSize, MarginSize.y*prop.y+(i+1)*settings.mainCharSize));

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
                                game.indPagina+=game.nota[i];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                    //visxu
                    testoOpzioni.setString(scritta[13]);
                    testoOpzioni.setStyle(sf::Text::Regular);
                    testoOpzioni.setFillColor(settings.colore[2]);
                    testoOpzioni.setPosition(sf::Vector2f(ScreenSize.x-MarginSize.x*prop.x*4/5+5*settings.mainCharSize, MarginSize.y*prop.y+(i+1)*settings.mainCharSize));

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
                                game.nNote--;
                                for(int j=i; j<game.nNote; j++)  game.nota[j]=game.nota[j+1];
                            }
                        }
                    }

                    window->draw(testoOpzioni);
                }
            }
            else
            {
                notaAperta=game.nNote;
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
        Pulsante PHome(L"<<", MarginSize.x, ScreenSize.y/prop.y-(MarginSize.y+60.f)/2, 1, &pHome);
        if(PHome.draw(window)==3)
        {
            game.indPagina=game.cronoPagine[game.indCP-1];
            game.indCP-=2;
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
            PHome.etic=Str2Wstr(to_string((int) prossimoParagrafo/100))+L" "+Str2Wstr(to_string(((int) prossimoParagrafo%100)/10))+L" "+Str2Wstr(to_string((int) prossimoParagrafo%10));

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
                game.indPagina=prossimoParagrafo;
                prossimoParagrafo=0;
            }
        }

        if(PHome.draw(window)==3)
        {
            staDigitando=1;
        }
    }
    
    ///-----------------Barra Alta-----------------
	sf::RectangleShape barraA(sf::Vector2f(ScreenSize.x-2*MarginSize.x*prop.x, MarginSize.y*prop.y));
    barraA.setFillColor(settings.colore[0]);
    barraA.setPosition(MarginSize.x*prop.x, 0);
    window->draw(barraA);
    
    {///Home
        static bool pHome=0;
        Pulsante PHome(L"H", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return IdHome;
    }

	
	{///char size

        sf::Text titolo;
        titolo.setFont(settings.font);
        titolo.setString(scritta[18]+L"  "+Str2Wstr(to_string(settings.mainCharSize))+L" ");
        titolo.setFillColor(settings.colore[1]);
        titolo.setPosition(sf::Vector2f(MarginSize.x*2, (MarginSize.y-60)/2)*prop);
        titolo.setCharacterSize(settings.labelCharSize);
        window->draw(titolo);

        sf::Text plus;
        plus.setFont(settings.font);
        plus.setString(" + ");
        plus.setFillColor(settings.colore[2]);
        plus.setPosition(sf::Vector2f(titolo.getGlobalBounds().left+titolo.getGlobalBounds().width, (MarginSize.y-60)/2*prop.y));
        plus.setCharacterSize(settings.labelCharSize);
        static bool premutoP=0;
        if(plus.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		{
			plus.setFillColor(settings.colore[1]);
			if(premutoP)
			{
				if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					premutoP=0;
					settings.mainCharSize=max(0, settings.mainCharSize+1);
				}
			}
			else    
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
					premutoP=1;
		}
		else
		{
			plus.setFillColor(settings.colore[2]);
			premutoP=0;
		}	
        window->draw(plus);

        sf::Text minus;
        minus.setFont(settings.font);
        minus.setString(" - ");
        minus.setFillColor(settings.colore[2]);
        minus.setPosition(sf::Vector2f(plus.getGlobalBounds().left+plus.getGlobalBounds().width, (MarginSize.y-60)/2*prop.y));
        minus.setCharacterSize(settings.labelCharSize);
        static bool premutoM=0;
        if(minus.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		{
			minus.setFillColor(settings.colore[1]);
			if(premutoM)
			{
				if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					premutoM=0;
					settings.mainCharSize=max(0, settings.mainCharSize-1);
				}
			}
			else    
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
					premutoM=1;
		}
		else
		{
			minus.setFillColor(settings.colore[2]);
			premutoM=0;
		}
        window->draw(minus);
        
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
    {///Rules
        static bool pHome=0;
        Pulsante PHome(scritta[8], (1200-180)/2, (600-60)/2+240, 3, &pHome);
        if(PHome.draw(window)==3) return IdRules;
    }
    {///Exit
        static bool pHome=0;
        Pulsante PHome(L"X", ScreenSize.x/prop.x-68, 8, 1, &pHome);
        if(PHome.draw(window)==3)
		{
            exit(window);
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
        titolo.setCharacterSize(settings.labelCharSize);
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
        titolo.setCharacterSize(settings.labelCharSize);
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
        titolo.setCharacterSize(settings.labelCharSize);
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
	
	{///char size

        sf::Text titolo;
        titolo.setFont(settings.font);
        titolo.setString(scritta[18]+L"  "+Str2Wstr(to_string(settings.labelCharSize))+L" ");
        titolo.setFillColor(settings.colore[1]);
        titolo.setPosition(sf::Vector2f(MarginSize.x*20.f/9.f, 260.f)*prop);
        titolo.setCharacterSize(settings.labelCharSize);
        window->draw(titolo);

        sf::Text plus;
        plus.setFont(settings.font);
        plus.setString(" + ");
        plus.setFillColor(settings.colore[2]);
        plus.setPosition(sf::Vector2f(titolo.getGlobalBounds().left+titolo.getGlobalBounds().width, 260*prop.y));
        plus.setCharacterSize(settings.labelCharSize);
        static bool premutoP=0;
        if(plus.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		{
			plus.setFillColor(settings.colore[1]);
			if(premutoP)
			{
				if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					premutoP=0;
					settings.labelCharSize=max(0, settings.labelCharSize+1);
				}
			}
			else    
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
					premutoP=1;
		}
		else
		{
			plus.setFillColor(settings.colore[2]);
			premutoP=0;
		}	
        window->draw(plus);

        sf::Text minus;
        minus.setFont(settings.font);
        minus.setString(" - ");
        minus.setFillColor(settings.colore[2]);
        minus.setPosition(sf::Vector2f(plus.getGlobalBounds().left+plus.getGlobalBounds().width, 260*prop.y));
        minus.setCharacterSize(settings.labelCharSize);
        static bool premutoM=0;
        if(minus.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		{
			minus.setFillColor(settings.colore[1]);
			if(premutoM)
			{
				if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					premutoM=0;
					settings.labelCharSize=max(0, settings.labelCharSize-1);
				}
			}
			else    
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
					premutoM=1;
		}
		else
		{
			minus.setFillColor(settings.colore[2]);
			premutoM=0;
		}
        window->draw(minus);
        
    }
    
	
    {///Home
        static bool pHome=0;
        Pulsante PHome(L"H", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
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
        Pulsante PHome(L"H", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
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
        Pulsante PLibro(Str2Wstr(titolo[j]), (1200-240)/2, 2*MarginSize.y+60*j, 4, &pLibro);
        if(PLibro.draw(window)==3)
        {
			game.newGame(titolo[j]);
            testo=caricaLibro(titolo[j]);        
			nPagine=dividiInPagine(pagina, nomePagina, testo);
        	nSez=disseziona(pagina[game.indPagina], sezione);
            return IdPlay;
        }
    }
    return IdBooks;
}

int visElencoGiochi(sf::RenderWindow* window, sf::Texture intreccio, string pagina[10000], string nomePagina[10000], Sezione sezione[100], int &nPagine, int &nSez, string &testo)
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

    string titolo[100];
    int i=0;
    for (const auto& entry : filesystem::directory_iterator("./games")) //portability issue
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
        Pulsante PLibro(Str2Wstr(titolo[j]), (1200-240)/2, 2*MarginSize.y+60*j, 4, &pLibro);
        if(PLibro.draw(window)==3)
        {
			game.load(titolo[j]);
            testo=caricaLibro(game.libro);        
			nPagine=dividiInPagine(pagina, nomePagina, testo);
        	nSez=disseziona(pagina[game.indPagina], sezione);
            return IdPlay;
        }
    }

    static bool pHome=0;
    Pulsante PHome(L"H", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
    if(PHome.draw(window)==3) return IdHome;

    return IdGame;
}

int visRules(sf::RenderWindow* window, sf::Texture intreccio, int scroll)
{
	ifstream fin("rules/"+settings.language+".txt");
	string text="";
	while(!fin.eof())
	{
		string tmp;
		getline(fin, tmp);
		text+=tmp+"\n";
	}
	
	Sezione sezione[500];
	int nSez=disseziona(text, sezione);	
    float lC[256];
	initLC(lC);
    sf::Text testoPagina;
    testoPagina.setFont(settings.font);
    testoPagina.setCharacterSize(settings.mainCharSize);
    testoPagina.setStyle(sf::Text::Regular);
  
	for(int i=0; i<nSez; i++)
    {
        float c=0;
        testoPagina.setFillColor(settings.colore[sezione[i].colore]);
        testoPagina.setStyle(sezione[i].stile);
        int contX=0;
        for(unsigned int j=0; j<sezione[i].testo.size(); j++)
        {
			if(settings.language=="Esperanto")
            {
				if(sezione[i].testo[j]=='x' ){j++; contX++;}
				testoPagina.setString(X2EO(sezione[i].testo).substr(j-contX, 1));
			}
			else
			{
				testoPagina.setString(sezione[i].testo.substr(j, 1));
			}
            testoPagina.setPosition(sezione[i].posizione+sf::Vector2f(c*settings.mainCharSize, settings.mainCharSize*scroll/4.f));
            window->draw(testoPagina);
            c+=lC[(int) sezione[i].testo[j]];
        }
    }


    ///------------intreccio Sinistro
    sf::Sprite barraS;
    barraS.setTexture(intreccio);
    barraS.setPosition(0, 0);
    window->draw(barraS);


    ///-----------------Intreccio Destro
    sf::Sprite barraD;
    barraD.setTexture(intreccio);
    barraD.scale(-1.f, 1.f);
    barraD.setPosition(ScreenSize.x, 0);
    window->draw(barraD);

    ///-----------------Barra Alta-----------------
	sf::RectangleShape barraA(sf::Vector2f(ScreenSize.x-2*MarginSize.x*prop.x, MarginSize.y*prop.y));
    barraA.setFillColor(settings.colore[0]);
    barraA.setPosition(MarginSize.x*prop.x, 0);
    window->draw(barraA);
    {///Home
        static bool pHome=0;
        Pulsante PHome(L"H", MarginSize.x, (MarginSize.y-60)/2, 1, &pHome);
        if(PHome.draw(window)==3) return IdHome;
    }

    return IdRules;
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
            schermata=visElencoGiochi(window, intreccio, pagina, nomePagina, sezione, nPagine, nSez, testo);
        break;
        
        case IdRules:
            schermata=visRules(window, intreccio, scroll);
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
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.mainCharSize/4.f, sezione[i].lung*settings.mainCharSize, settings.mainCharSize).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        game.indPagina=sezione[i].arg;
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
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.mainCharSize/4.f, sezione[i].lung*settings.mainCharSize, settings.mainCharSize).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        bool presente=0;
                        for(int j=0; j<game.nNote; j++)
                            presente=presente || (game.nota[j]==sezione[i].arg);
                        if(!presente)
                        {
                            game.nota[game.nNote]=sezione[i].arg;
                            game.nNote++;
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
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.mainCharSize/4.f, sezione[i].lung*settings.mainCharSize, settings.mainCharSize).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(premuto[i])
                    {
                        if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            premuto[i]=0;
                            game.sorte+=sezione[i].arg;
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
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.mainCharSize/4.f, sezione[i].lung*settings.mainCharSize, settings.mainCharSize).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(premuto[i])
                    {
                        if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            premuto[i]=0;
                            game.barre+=sezione[i].arg;
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
                if(sf::FloatRect(sezione[i].posizione.x, sezione[i].posizione.y+scroll*settings.mainCharSize/4.f, sezione[i].lung*settings.mainCharSize, settings.mainCharSize).contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                {
                    sezione[i].colore=1;
                    sezione[i].stile=sf::Text::Bold;
                    if(premuto[i])
                    {
                        if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            premuto[i]=0;
                            game.barre=sezione[i].arg;
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
    sf::Vector2f dimIntrSchermata[10];
    int tile[10][100][100];
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
        nSez=disseziona(pagina[game.indPagina], sezione);
		}
		
		
		
        dimIntrSchermata[IdHome]=sf::Vector2f(MarginSize.x*14/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdPlay]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdSettings]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdBooks]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdGame]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);
        dimIntrSchermata[IdRules]=sf::Vector2f(MarginSize.x*9/10*prop.x, ScreenSize.y);

		calcolaIntreccio(dimIntrSchermata[IdHome], tile[IdHome]);
        intreccio=disegnaIntreccio(dimIntrSchermata[IdHome], tile[0]);
        
        barraC.setScale(2/8.f, 1);
        window.clear(settings.colore[0]);
            window.draw(testoCaricamento);
            window.draw(barraA);
            window.draw(barraB);
            window.draw(barraC);
            window.draw(testoCaricamento);
            window.display();

        for(int i=0; i<6; i++)
        {
			calcolaIntreccio(dimIntrSchermata[i], tile[i+1]);
            
            window.clear(settings.colore[0]);
            barraC.setScale((i+3)/9.f, 1);
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
                exit(&window);
            }

            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                scroll=min(event.mouseWheelScroll.delta+scroll, 0.f);
            }
        }
        window.clear(settings.colore[0]);
        intreccio=disegnaIntreccio(dimIntrSchermata[schermata], tile[0]);
        
        if(game.indCP<0)
        {
            game.indCP=0;
            game.cronoPagine[game.indCP]=0;
            nSez=disseziona(pagina[game.indPagina], sezione);
        }
        bool cambioPagina=0;
        if(game.cronoPagine[game.indCP]!=game.indPagina)
        {
            cambioPagina=1;
            rilascio=0;
            game.indCP++;
            game.cronoPagine[game.indCP]=game.indPagina;
            nSez=disseziona(pagina[game.indPagina], sezione);

            scroll=0;
        }

        if(cambioPagina || schermata!=schermataS)
        {
            schermataS=schermata;
            
            for(int i=0; i<100; i++)
            {
				for(int j=0; j<100; j++)
				{
					tile[0][i][j]=tile[schermata+1][i][j];
				}		
			}
            intreccio=disegnaIntreccio(dimIntrSchermata[schermata], tile[0]);
            
            if(rilascio) azionaIpertesto(sezione, nSez, &window, scroll);
            visualizza(&window, schermata, sezione, intreccio, pagina, nomePagina, nPagine, nSez, testo, scroll);
            window.display();
            
            calcolaIntreccio(dimIntrSchermata[schermata], tile[schermata+1]);
        }
        else
        {
            if(rilascio) azionaIpertesto(sezione, nSez, &window, scroll);
            visualizza(&window, schermata, sezione, intreccio, pagina, nomePagina, nPagine, nSez, testo, scroll);
            window.display();
        }
        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {rilascio=1; nSez=disseziona(pagina[game.indPagina], sezione);}
    }

    return 0;
}
