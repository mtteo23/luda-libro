struct Sezione
{
    string testo="";//principale, a capo
    float lung=0;
    sf::Vector2f posizione;
    int colore=1;
    sf::Text::Style stile=sf::Text::Regular;
    int arg=0;
    char tipo='x';
    string lingua="";
};

float LarghezzaPagina;

void initLC(float lC[])
{
	sf::Text t;
	t.setFont(settings.font);
	t.setCharacterSize(50);
	
	t.setString("he");
    float kerning=t.getGlobalBounds().width;
    t.setString("h");
    kerning-=t.getGlobalBounds().width;
    t.setString("e");
    kerning-=t.getGlobalBounds().width;
    
    for(int i=0; i<256; i++)
    {
		t.setString((char) i);
		lC[i]=(t.getGlobalBounds().width+kerning)/50.f;
	}
}

int disseziona(string testo, Sezione sezione[])
{
	string linguaLibro=testo.substr(0, 2);
    float lC[256]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.2, 1.2, 1.1, 1.1, 2, 1.3, 1.3, 1.4, 1.4, 1.4, 1.2, 1.5, 1.4, 0.5, 1, 1.3, 1.1, 1.6, 1.4, 1.5, 1.2, 1.5, 1.4, 1.3, 1.2, 1.5, 1.3, 1.9, 1.3, 1.3, 1.2, 0.5, 0.5, 0.5, 0.9, 1.1, 0.7, 1, 1, 1, 1.1, 1.1, 0.6, 1.1, 1.1, 0.4, 0.4, 1, 0.4, 1.6, 1.1, 1.1, 1.1, 1, 0.7, 0.9, 0.6, 1.1, 1, 1.4, 1, 1, 1, 0.7, 0.5, 0.6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	initLC(lC);
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
        sezione[i].lingua=linguaLibro;
    }
    for(unsigned int i=2; i<testo.size(); i++)
    {
        if(testo[i]=='/' || testo[i]=='#')
        {
            if(sezione[indSez].lung>0)
            {
				indSez++;
			}
            if(testo[i]=='/')
            {
                i++;
                if(testo[i]==' ' || testo[i]=='\n' || testo[i]==',' || testo[i]=='.' || testo[i]==':' || testo[i]==';'|| testo[i]=='!'|| testo[i]=='?' || testo[i]==13)
                {
                    sezione[indSez].stile=sf::Text::Regular;
                    sezione[indSez].tipo='x';
                    sezione[indSez].arg=0;
                    sezione[indSez].colore=1;
                    sezione[indSez].posizione=sf::Vector2f(c*settings.mainCharSize+MarginSize.x*prop.x, r*settings.mainCharSize+MarginSize.y*prop.y);
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
                    sezione[indSez].posizione=sf::Vector2f(c*settings.mainCharSize+MarginSize.x*prop.x, r*settings.mainCharSize+MarginSize.y*prop.y);
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
                sezione[indSez].posizione=sf::Vector2f(c*settings.mainCharSize+MarginSize.x*prop.x, r*settings.mainCharSize+MarginSize.y*prop.y);
            }
        }
        else
        {
            int lungProsParola=testo.substr(i+1).find(" ");
            if(testo[i]=='\n' || (c+lungProsParola)*settings.mainCharSize>LarghezzaPagina)
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
                    sezione[indSez].posizione=sf::Vector2f(c*settings.mainCharSize+MarginSize.x*prop.x, r*settings.mainCharSize+MarginSize.y*prop.y);
                }
                else
                {
                    //Work in progress
                }
            }
            else
            {
                sezione[indSez].testo+=testo.substr(i, 1);
                if(sezione[indSez].lingua=="eo")
                {
					sezione[indSez].lung+=lC[(int) testo[i]]*(testo[i]!='x');
					c+=lC[(int) testo[i]]*(testo[i]!='x');
				}
				else
				{
					sezione[indSez].lung+=lC[(int) testo[i]];
					c+=lC[(int) testo[i]];
				}
                
            }
        }
    }
    return indSez+1;
}

string caricaLibro(string titolo)
{
    ifstream fin("books/"+titolo+".txt");
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

int dividiInPagine(string pagina[], string nomePagina[], string testo)
{
	string linguaLibro=testo.substr(0, 2);
	
	int nPagine=0;
	long unsigned int inizio=0, fine=0, lunghezza=0, np=string::npos;
	for(int i=0; fine!=testo.size()-1; i++)
   	{
       	inizio=testo.find("#"+to_string(i)+" ");
      	fine=testo.find("#"+to_string(i+1)+" ");
        if(fine==np)  fine=testo.size()-1;
        lunghezza=fine-inizio;
        string tmpText=testo.substr(inizio);
        tmpText=tmpText.substr(tmpText.find(" ")+1);
        tmpText=tmpText.substr(0, tmpText.find("/"));
        nomePagina[i]=tmpText;
        pagina[i]=linguaLibro+testo.substr(inizio, lunghezza);
		nPagine=i+1;
	}
	return nPagine;
}
