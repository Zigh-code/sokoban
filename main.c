#include <stdio.h>

#include <stdlib.h>

#include <SDL.h>

#include <SDL_ttf.h>

#include <SDL_image.h>

int game(SDL_Surface * screen);

int main(int argc, char * argv[]) {

    SDL_Surface * screen = NULL, * menu = NULL;
    SDL_Event event;
    SDL_Rect positionMenu;
    int etat = 1;

    //ecran
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_WM_SetIcon(IMG_Load("img/ico.png"), NULL);
    screen = SDL_SetVideoMode(800, 512, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Sokoban!", NULL);

    //menu
    menu = IMG_Load("img/home.png");
    positionMenu.x = 0;
    positionMenu.y = 0;


    while (etat) {
        SDL_Delay(100);
        SDL_WaitEvent( & event);
        switch (event.type)

        {
            case SDL_QUIT:
                etat = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        etat = game(screen);
                        break;
                }

                break;
        }
        SDL_BlitSurface(menu, NULL, screen, & positionMenu);
        SDL_Flip(screen); //Raffraichi l ecran

    }
    TTF_Quit();

    SDL_Quit();



    return 0;
}


int game(SDL_Surface * screen) {
    SDL_Surface * box = NULL, * movtext = NULL,* winn = NULL, * timetext = NULL, 
    * gamestat = NULL, * boxDone = NULL, * objectif = NULL, * wall = NULL, * vide = NULL, * perso[4] = {
        NULL
    };
    SDL_Rect position, stat, posmov, postime,poswin;
    SDL_Event event;
    TTF_Font * police = NULL;
    SDL_Color textcolor = {
        255,
        255,
        255
    };
    char move[15], e,temps[20] = "";
    int etatgame = 1,etat = 1, direction = 0, k = 0, j = 0, x, y, o = 0, p,win = 0 , qtmove = 0 ,tempsActuel = 0, tempsPrecedent = 0,
     compteur = 0, seconde = 0, min = 0, hr = 0;

    police = TTF_OpenFont("montserrat-black.ttf", 22);
    //perso
    perso[0] = IMG_Load("img/F.png");
    perso[1] = IMG_Load("img/L.png");
    perso[2] = IMG_Load("img/R.png");
    perso[3] = IMG_Load("img/H.png");


    //map
    int carte[8][8] = {};
    wall = IMG_Load("img/w.png");
    box = IMG_Load("img/caisse.png");
    boxDone = IMG_Load("img/caisse_ok.png");
    objectif = IMG_Load("img/p.png");
    vide = SDL_CreateRGBSurface(SDL_HWSURFACE, 64, 64, 32, 0, 0, 0, 0);
    SDL_FillRect(vide, NULL, SDL_MapRGB(vide -> format, 255, 255, 255));

    gamestat = SDL_CreateRGBSurface(SDL_HWSURFACE, 288, 512, 32, 0, 0, 0, 0);
    SDL_FillRect(gamestat, NULL, SDL_MapRGB(gamestat-> format, 130, 150, 255));
    stat.x = 512;
    stat.y = 0;



    //generation de la carte
    FILE * f = fopen("map.txt", "r");
    do {
        e = fgetc(f);
        switch (e) {
            case '0':
                carte[k][j] = 0;
                break;
            case '1':
                carte[k][j] = 1;
                break;
            case '2':
                carte[k][j] = 2;
                break;
            case '3':
                carte[k][j] = 3;
                break;
            case '4':
                carte[k][j] = 4;
                break;
            case '5':
                carte[k][j] = 5;
                break;
        }

        j++;
        if (j == 8) {
            j = 0;
            k++;
        }

    } while (e != EOF);
    fclose(f);  

    /* Initialisation du temps et du timetext */
    tempsActuel = SDL_GetTicks();


    timetext = TTF_RenderText_Blended(police, temps, textcolor);

    sprintf(temps, "Time : %d", compteur);
    SDL_EnableKeyRepeat(100,100);
    while (etatgame) {
        
        sprintf(move, "Moves :%d", qtmove);
        movtext = TTF_RenderText_Blended(police, move, textcolor);
        winn = TTF_RenderText_Blended(police, "YOU WIN", textcolor);
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen -> format, 255, 255, 255));
        SDL_BlitSurface(gamestat, NULL, screen, & stat);

        posmov.x = stat.x + 20;
        posmov.y = stat.y + 20;

        SDL_BlitSurface(movtext, NULL, screen, & posmov);

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent >= 100) {
            compteur += 100;
            hr = (compteur / 1000);
            seconde = hr % 60;
            min = hr / 60;

            sprintf(temps, "Time : %d Min %d Sec", min, seconde);
            SDL_FreeSurface(timetext);
            timetext = TTF_RenderText_Blended(police, temps, textcolor);
            tempsPrecedent = tempsActuel; /* On met à jour le tempsPrecedent */
        }
        postime.x = posmov.x;
        postime.y = posmov.y + 50;
        SDL_BlitSurface(timetext, NULL, screen, & postime);
        poswin.x=posmov.x;
        poswin.y=posmov.y+100;
        SDL_Delay(50);
        if (win==1){
            SDL_BlitSurface(winn, NULL, screen, & poswin);

        }


        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                position.x = 64 * i;
                position.y = 64 * j;
                switch (carte[i][j]) {

                    case 0:
                        SDL_BlitSurface(wall, NULL, screen, & position);
                        break;
                    case 1:
                        SDL_BlitSurface(box, NULL, screen, & position);
                        break;
                    case 2:
                        SDL_BlitSurface(boxDone, NULL, screen, & position);
                        break;
                    case 3:
                        SDL_BlitSurface(objectif, NULL, screen, & position);
                        break;
                    case 4:
                        SDL_BlitSurface(vide, NULL, screen, & position);
                        break;
                    case 5:
                        x = i;
                        y = j;
                        SDL_BlitSurface(perso[direction], NULL, screen, & position);
                        break;




                }
            }

        }


        SDL_PollEvent( & event);
        
        switch (event.type) {
            case SDL_QUIT:
                etatgame = 0;
                etat = 0;
                break;
            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        etatgame = 0;
                        SDL_WaitEvent( & event);
                        break;

                    case SDLK_RIGHT:
                        direction = 2;
                        if (o == 0) {
                            p = 4;
                        } else if (o == 1 && carte[x + 2][y] != 0 && carte[x + 2][y] != 1 && carte[x + 2][y] != 2) {
                            p = 3;
                            o = 0;
                        }
                        if (carte[x + 1][y] == 3) {
                            o = 1;
                        } else if (carte[x + 1][y] == 2 && carte[x + 2][y] != 0 && carte[x + 2][y] != 1 && carte[x + 2][y] != 2) {
                            o = 1;
                        }

                        if (carte[x + 1][y] == 4 || carte[x + 1][y] == 3) {
                            qtmove++;
                            carte[x + 1][y] = 5;
                            carte[x][y] = p;
                        } else if (((carte[x + 1][y] == 1) || (carte[x + 1][y] == 2)) && carte[x + 2][y] == 4 || carte[x + 2][y] == 3) {
                            qtmove++;
                            carte[x + 1][y] = 5;
                            carte[x][y] = p;
                            if (carte[x + 2][y] == 3) {
                                carte[x + 2][y] = 2;
                            } else {
                                carte[x + 2][y] = 1;
                            }

                        } else
                            break;
                        SDL_WaitEvent( & event);
                        break;



                    case SDLK_LEFT:
                        direction = 1;
                        if (o == 0) {
                            p = 4;
                        } else if (o == 1 && carte[x - 2][y] != 0 && carte[x - 2][y] != 1 && carte[x - 2][y] != 2) {
                            p = 3;

                            o = 0;


                        }
                        if (carte[x - 1][y] == 3) {
                            o = 1;
                        } else if (carte[x - 1][y] == 2 && carte[x - 2][y] != 0 && carte[x - 2][y] != 1 && carte[x - 2][y] != 2) {
                            o = 1;
                        }
                        if (carte[x - 1][y] == 4 || carte[x - 1][y] == 3) {
                            qtmove++;

                            carte[x - 1][y] = 5;
                            carte[x][y] = p;
                        } else if (((carte[x - 1][y] == 1) || (carte[x - 1][y] == 2)) && carte[x - 2][y] != 0) {
                            qtmove++;
                            carte[x - 1][y] = 5;
                            carte[x][y] = p;

                            if (carte[x - 2][y] == 3) {
                                carte[x - 2][y] = 2;
                            } else {
                                carte[x - 2][y] = 1;
                            }
                        } else {
                            break;
                        }
                        SDL_WaitEvent( & event);

                        break;

                    case SDLK_UP:
                        direction = 3;
                        if (o == 0) {
                            p = 4;
                        } else if (o == 1 && carte[x][y - 2] != 0 && carte[x][y - 2] != 1 && carte[x][y - 2] != 2) {
                            p = 3;
                            o = 0;
                        }

                        if (carte[x][y - 1] == 3) {

                            o = 1;

                        } else if (carte[x][y - 1] == 2 && carte[x][y - 2] != 0 && carte[x][y - 2] != 1 && carte[x][y - 2] != 2) {

                            o = 1;

                        }

                        if (carte[x][y - 1] == 4 || carte[x][y - 1] == 3) {
                            qtmove++;
                            carte[x][y - 1] = 5;
                            carte[x][y] = p;
                        } else if ((carte[x][y - 1] == 1 || carte[x][y - 1] == 2) && carte[x][y - 2] != 0) {
                            qtmove++;
                            carte[x][y - 1] = 5;
                            carte[x][y] = p;
                            if (carte[x][y - 2] == 3) {
                                carte[x][y - 2] = 2;
                            } else {
                                carte[x][y - 2] = 1;
                            }
                        } else {
                            break;
                        }
                        SDL_WaitEvent( & event);
                        break;

                    case SDLK_DOWN:
                        direction = 0;
                        if (o == 0) {
                            p = 4;
                        } else if (o == 1 && carte[x][y + 2] != 0 && carte[x][y + 2] != 1 && carte[x][y + 2] != 2) {
                            p = 3;
                            o = 0;

                        }
                        if (carte[x][y + 1] == 3) {
                            o = 1;
                        } else if (carte[x][y + 1] == 2 && carte[x][y + 2] != 0 && carte[x][y + 2] != 1 && carte[x][y + 2] != 2) {

                            o = 1;
                        }

                        if (carte[x][y + 1] == 4 || carte[x][y + 1] == 3) {
                            qtmove++;
                            carte[x][y + 1] = 5;
                            carte[x][y] = p;
                        } else if ((carte[x][y + 1] == 1 || carte[x][y + 1] == 2) && carte[x][y + 2] != 0) {
                            qtmove++;
                            carte[x][y + 1] = 5;
                            carte[x][y] = p;
                            if (carte[x][y + 2] == 3) {
                                carte[x][y + 2] = 2;
                            } else {
                                carte[x][y + 2] = 1;
                            }
                        } else {
                            break;
                        }
                        SDL_WaitEvent( & event);
                        break;

                }


                break;

        }
        int g = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                
                if (carte[i][j]==3){
                    g++;
                }  
            }
        }
        if (g==0)
                {
                    win=1;
                }


        SDL_Flip(screen);

    }
    //liberation memoire
    SDL_FreeSurface(objectif);
    SDL_FreeSurface(wall);
    SDL_FreeSurface(vide);
    SDL_FreeSurface(box);
    SDL_FreeSurface(movtext);
    SDL_FreeSurface(timetext);
    SDL_FreeSurface(boxDone);
    SDL_FreeSurface(gamestat);
    TTF_CloseFont(police);

    for (int direction = 0; direction < 4; direction++) {
        SDL_FreeSurface(perso[direction]);
    }
    return etat;
}

//gcc 'sdl-config --cflags --libs' -o main.x main.c
//gcc main.c -o soko -I ../SDL/include -L ../SDL/lib -lmingw32 -lSDLmain -lSDL -lSDL_Image -SDL_ttf