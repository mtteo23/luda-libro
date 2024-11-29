/*
   ludlibrilo, a program to run gamebooks
   Copyright (C) 2024 Matteo Pascali (makte0)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/


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

void calcolaIntreccio(sf::Vector2f dimIntrSchermata,int tile[100][100])
{
	#define LargTile 8
    int r=dimIntrSchermata.y/LargTile/2+4;
    int c=dimIntrSchermata.x/LargTile+2;
    unsigned char poss[100][100];
    bool adatto[8][4][8];//tileA, dir, tileB
    srand(time(0));
    caricaTabAdatto(adatto);

    do{inizializza(poss, r, c, adatto);}while(!risolvi(poss, r, c, adatto));
    for(int i=0; i<r; i++)  for(int j=0; j<c; j++)  tile[i][j]=collassa(poss[i][j]);
}

sf::Texture disegnaIntreccio(sf::Vector2f dimIntrSchermata, int tile[100][100])
{
	
    #define LargTile 8
    int r=dimIntrSchermata.y/LargTile/2+4;
    int c=dimIntrSchermata.x/LargTile+2;
    
    /*
    unsigned char poss[100][100];
    bool adatto[8][4][8];//tileA, dir, tileB
    int tile[100][100];
    srand(time(0));
    caricaTabAdatto(adatto);

    do{inizializza(poss, r, c, adatto);}while(!risolvi(poss, r, c, adatto));
    for(int i=0; i<r; i++)  for(int j=0; j<c; j++)  tile[i][j]=collassa(poss[i][j]);
	*/
	
	
    sf::RenderTexture tavola;
    tavola.create((unsigned int) c*LargTile, (unsigned int) (r-2)*2*LargTile);
    tavola.clear(settings.colore[0]);

    sf::RectangleShape quad(sf::Vector2f(LargTile/4.f, LargTile/4.f));
    quad.setFillColor(settings.colore[1]);

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
