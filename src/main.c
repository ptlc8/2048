#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "2048.h"
#include "ai.h"
#include "highscores.h"
#include "save.h"



// Création d'une constante pour chaque menu
#define MAIN_MENU 0
#define MENU_JOUEUR 1
#define EN_JEU 2
#define CHOIX_DIM 7
#define END_MENU 8
//#define BRAVO_MENU 11

// Création d'une constante pour chaque mode de jeu
#define SOLO 5
#define HUMAIN_VS_HUMAIN 3
#define HUMAIN_VS_ORDI 4
#define MENU_MEILLEURS_SCORES 6


void displayText(SDL_Renderer* pRenderer, TTF_Font* font, SDL_Color fontColor, char* text, int x, int y, int w, int h){
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, fontColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(pRenderer, surfaceMessage);
    SDL_Rect textR = {x, y, h*(surfaceMessage->w)/(surfaceMessage->h), h};
    SDL_RenderCopy(pRenderer, textTexture, NULL, &textR);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(textTexture);
}

void displayTextCenter(SDL_Renderer* pRenderer, TTF_Font* font, SDL_Color fontColor, char* text, int x, int y, int w, int h){
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, fontColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(pRenderer, surfaceMessage);
    int newW = h*(surfaceMessage->w)/(surfaceMessage->h);
    SDL_Rect textR = {x+w/2-newW/2, y, newW, h};
    SDL_RenderCopy(pRenderer, textTexture, NULL, &textR);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(textTexture);
}

SDL_Texture* backgroundTexture = NULL;
// Affiche le joli fond d'écran
void displayBackGround(SDL_Renderer* pRenderer, int w, int h){

	if (backgroundTexture == NULL) {
		SDL_Surface* image = SDL_LoadBMP("assets/licorne.bmp");
		backgroundTexture = SDL_CreateTextureFromSurface(pRenderer, image);
		SDL_FreeSurface(image);
	}
	
	SDL_Rect rectangle = {0, 0, w, h};

	// SDL_QueryTexture(backgroundTexture, NULL, NULL, &rectangle.w, &rectangle.h); // si on utilise cette fonction la taille des elements est fixés en fonction de la taille de l'image, dans ec cas si on redimensionne la fenetre, le fond ne s'adaptera pas, il vaut mieux prendre les dimensions de la fenetre

	SDL_RenderCopy(pRenderer, backgroundTexture, NULL, &rectangle);
 
}

SDL_Texture* homeButtonTexture = NULL;
// Affiche l'image du boutton d'accueil
void displayHomeButton(SDL_Renderer* pRenderer, int w, int h, int x, int y){
	
	if (homeButtonTexture == NULL) {
		SDL_Surface* image = SDL_LoadBMP("assets/home.bmp");
		homeButtonTexture = SDL_CreateTextureFromSurface(pRenderer, image);
		SDL_FreeSurface(image);
	}
	
	SDL_Rect rectangle = {x, y, h/10, h/10};

	SDL_RenderCopy(pRenderer, homeButtonTexture, NULL, &rectangle);
}


SDL_Texture* restartButtonTexture = NULL;
// Affiche l'image du boutton recommencer
void displayRestartButton(SDL_Renderer* pRenderer, int w, int h){
	
	if (restartButtonTexture == NULL) {
		SDL_Surface* image = SDL_LoadBMP("assets/button4.bmp");
		restartButtonTexture = SDL_CreateTextureFromSurface(pRenderer, image);
		SDL_FreeSurface(image);
	}
	
	SDL_Rect rectangle = {w-h/10, h/10, h/10, h/10};

	SDL_RenderCopy(pRenderer, restartButtonTexture, NULL, &rectangle);
}


SDL_Texture** buttonsTextures = NULL;
// initilise les textures utilisées dans le programme
void initTextures(SDL_Renderer* pRenderer) {
	buttonsTextures = malloc(11 * sizeof(SDL_Texture*));
	for (int i = 0; i < 11; i++) {
		char path[22];
		snprintf(path, 22, "assets/button%d.bmp", i);
		SDL_Surface* image = SDL_LoadBMP(path);
		buttonsTextures[i] = SDL_CreateTextureFromSurface(pRenderer, image);
		SDL_FreeSurface(image);
	}
}


// Affiche le menu principal
void displayMainMenu(SDL_Renderer* pRenderer, TTF_Font* font, int w, int h){

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	displayBackGround(pRenderer, w, h);
	SDL_Color fontColor = {255, 255, 255};
	
	displayTextCenter(pRenderer, font, fontColor, "Bienvenue dans le jeu du 2084 !!", w/10, h/10, 4*w/5, h/20);
	
	SDL_Rect r = {w/20, 3*h/11, w-w/10, 3*h/11};
	SDL_RenderCopy(pRenderer, buttonsTextures[0], NULL, &r);
	displayTextCenter(pRenderer, font, fontColor, "JOUER", w/20, 8*h/33, w-w/10, 3*h/11);
	
	SDL_Rect r2 = {w/20, 7*h/11, 9*w/10, 3*h/11};
	SDL_RenderCopy(pRenderer, buttonsTextures[1], NULL, &r2);
	displayTextCenter(pRenderer, font, fontColor, "Meilleurs scores", w/20, 2*h/3, w-w/10, 3*h/20);
		
}
// Prend un event (cliquer) sur le menu principal et agit en conséquence
void onMainMenuEvent(SDL_Event event, int w, int h, int* menu){
	int x, y;
	if(event.type == SDL_MOUSEBUTTONUP) {
		//printf("%d %d\n", x, y);
		x=event.motion.x;
		y=event.motion.y;
		
		if(w/20<x && x<w-w/20 && 3*h/11<y && y<6*h/11){
			*menu=MENU_JOUEUR;
		}
		if(w/20<x && x<w-w/20 && 7*h/11<y && y<10*h/11){
			*menu=MENU_MEILLEURS_SCORES;
		}
	}
}


//Prend un event (cliquer) sur le menu de meilleurs scores. Si la personne clique sur le bouton HOME, on retourne au menu principal, sinon ne rien faire
void onBestScoreEvent(SDL_Event event, int w, int h, int* menu){
	int x, y;
	
	if(event.type == SDL_MOUSEBUTTONUP) {
		x=event.motion.x;
		y=event.motion.y;
		//if(0<x && x<150 && 0<y && y<150){ // le bouton est un cercle ^^
		if (sqrt(pow(h/20-x, 2) + pow(h/20-y, 2)) <= h/20) {
			*menu=MAIN_MENU;
		}
	}
}
//Affiche le tableau des meilleurs scores
void displayBestScores(SDL_Renderer* pRenderer, TTF_Font* font, int w, int h){
	
	int n;
	Save* tab = getHighScores(&n);
	
	SDL_Color fontColor = {0, 0, 0};
	displayBackGround(pRenderer, w, h);
	
	SDL_Rect r = {w/20, h/40, 9*w/10, h-h/20};
	SDL_RenderFillRect(pRenderer, &r);
	
	displayText(pRenderer, font, fontColor, "Meilleurs scores", w/10, h/40, w-w/5, h/10);
	for(int i=0; i<n; i++){
		displayText(pRenderer, font, fontColor, tab[i].nom, w/10, h/40*(3*i+5), 2*w/3, h/10);
		char score[10];
		snprintf(score, 10, "%d", tab[i].score);
		displayText(pRenderer, font, fontColor, score, 8*w/10, h/40*(3*i+5), 2*w/3, h/10);
	}
	displayHomeButton(pRenderer, w, h, 0, 0);
}


//Affiche le menu secondaire "jouer".
void displayMenuJouer(SDL_Renderer* pRenderer, TTF_Font* font, int w, int h){

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	displayBackGround(pRenderer, w, h);

	SDL_Color fontColor = {255, 255, 255};
		
	for(int i=0; i<3; i++){
		SDL_Rect r = {w/20, h/13*(i*3+1), w-w/10, h/13*2};
		SDL_RenderCopy(pRenderer, buttonsTextures[i], NULL, &r);
		
		displayTextCenter(pRenderer, font, fontColor, i==0?"Solo":i==1?"1 VS 1":i==2?"1 VS Ordi":"", r.x, r.y-h/50, r.w, r.h);
	}
	
	for (int i = 0; i < 3; i++) {
		SDL_Rect r = {w/20+i*(9*w/20-h/13), h/13*10, h/13*2, h/13*2};
		SDL_RenderCopy(pRenderer, buttonsTextures[i+5], NULL, &r);
	}
	
	displayHomeButton(pRenderer, w, h, 0, 0);
}
//Prend un event (ciquer) et agit en conséquence
void onMenuJoueur(SDL_Event event, int w, int h, int *menu, int *gamemode, int *dim, int** *plateau1, int** *plateau2, int *score1, int* score2, int *currentPlayer){
	int x, y;
	if(event.type == SDL_MOUSEBUTTONUP) {
		x=event.motion.x;
		y=event.motion.y;
		
		if (sqrt(pow(h/20-x, 2) + pow(h/20-y, 2)) <= h/20) {
			*menu = MAIN_MENU;
			return;
		}
		
		if (w/20<x && x<w-w/20) {
			if(h/13<y && y<h/13*3){
				*gamemode=SOLO;
				*menu=CHOIX_DIM;
			}
			if(h/13*4<y && y<h/13*6){
				*gamemode=HUMAIN_VS_HUMAIN;
				*menu=CHOIX_DIM;
			}
			if(h/13*7<y && y<h/13*9){
				*gamemode=HUMAIN_VS_ORDI;
				*menu=CHOIX_DIM;
			}
			if(h/13*10<y && y<h/13*12){
				if (w/20<x && x<w/20+h/13*2) {
					if (charge(1, gamemode, dim, plateau1, plateau2, score1, score2, currentPlayer))
						*menu = EN_JEU;
				}
				if (w/2-h/13<x && x<w/2-h/13+h/13*2) {
					if (charge(2, gamemode, dim, plateau1, plateau2, score1, score2, currentPlayer))
						*menu = EN_JEU;
				}
				if (19*w/20-2*h/13<x && x<19*w/20-2*h/13+h/13*2) {
					if (charge(3, gamemode, dim, plateau1, plateau2, score1, score2, currentPlayer))
						*menu = EN_JEU;
				}
			}
		}
	}
}


//Affiche le menu de choix de dimension de plateau
void displayDimChoice(SDL_Renderer* pRenderer, TTF_Font* font, int w, int h){

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	displayBackGround(pRenderer, w, h);
	SDL_Color fontColor = {255, 255, 255};
	
	displayHomeButton(pRenderer, w, h, 0, 0);
		
	for(int i=0; i<3; i++){
		SDL_Rect r = {w/10, (i*3+1)*h/10, 4*w/5, h/5};
		SDL_RenderCopy(pRenderer, buttonsTextures[i], NULL, &r);
		
		char text[10];
		snprintf(text, 10, "%dx%d", (i+2)*2, (i+2)*2);
		displayTextCenter(pRenderer, font, fontColor, text, r.x, r.y-h/40, r.w, r.h);
	}
	SDL_RenderPresent(pRenderer);
}
//Prend un event (cliquer) et agit en conséquence
void onChoixDimEvent(SDL_Event event, int w, int h, int *menu, int *dim, int*** plateau, int*** plateau2, int *score, int *score2, int *had2048, int *currentPlayer){
	int x, y;
	if(event.type == SDL_MOUSEBUTTONUP) {
		x=event.motion.x;
		y=event.motion.y;
		
		if (sqrt(pow(h/20-x, 2) + pow(h/20-y, 2)) <= h/20) {
			*menu = MAIN_MENU;
			return;
		}
		if(w/10<x && x<w-w/10) {
			if (h/10<y && y<3*h/10){
				*dim=4;
			}
			else if (4*h/10<y && y<6*h/10){
				*dim = 6;
			}
			else if (7*h/10<y && y<9*h/10)
				*dim = 8;
			else return;
			
			*menu = EN_JEU;
			*plateau = createPlateau(*dim);
			addRandomTile(*plateau, *dim);
			addRandomTile(*plateau, *dim);
			*plateau2 = createPlateau(*dim);
			addRandomTile(*plateau2, *dim);
			addRandomTile(*plateau2, *dim);
			*score = 0;
			*score2 = 0;
			*currentPlayer = 0;
			*had2048 = 0;
		}
	}
}


// Affiche les boutons en jeu 
void displayGameBar(SDL_Renderer* pRenderer, int w, int h) {
	displayRestartButton(pRenderer, w, h);
	displayHomeButton(pRenderer, w, h, 10*w/11, 0);
	for (int i = 0; i < 3; i++) {
		SDL_Rect r = {10*w/11, (i+3)*h/10, h/10, h/10};
		SDL_RenderCopy(pRenderer, buttonsTextures[i+8], NULL, &r);
	}
}
void onGameBar(SDL_Event event, int* menu, int gamemode, int w, int h, int dim, int** plateau1, int** plateau2, int *score1, int *score2, int current_player) {
	int x, y;
	
	if(event.type == SDL_MOUSEBUTTONUP) {
		x=event.motion.x;
		y=event.motion.y;
		if (sqrt(pow(w-h/20-x, 2) + pow(h/20-y, 2)) <= h/20) {
		//if (x>w-h/10 && y<h/10) {
			*menu=MAIN_MENU;
		}
		//if (sqrt(pow(h/20-x, 2) + pow(h/20-y, 2)) <= h/20) {
		if (x>w-h/10 && y>h/10 && y<h/5) {
            if(gamemode==SOLO){
                for(int i=0; i<dim; i++){
                    for(int j=0; j<dim; j++){
                        plateau1[i][j]=0;
                    }
                }
                addRandomTile(plateau1, dim);
                addRandomTile(plateau1, dim);
                *score1 =0;
            }
            if(gamemode==HUMAIN_VS_ORDI || gamemode == HUMAIN_VS_HUMAIN){
                for(int i=0; i<dim; i++){
                    for(int j=0; j<dim; j++){
                        plateau1[i][j]=0;
                        plateau2[i][j]=0;
                    }
                }
                addRandomTile(plateau1, dim);
                addRandomTile(plateau1, dim);
                addRandomTile(plateau2, dim);
                addRandomTile(plateau2, dim);
                *score1 =0;
                *score2 =0;
                current_player = 0;
            }
        }
		if (10*w/11<x && x<w) {
			if (3*h/10<y && y<4*h/10) {
				save(1, gamemode, dim, plateau1, plateau2, *score1, *score2, current_player);
			}
			if (4*h/10<y && y<5*h/10) {
				save(2, gamemode, dim, plateau1, plateau2, *score1, *score2, current_player);
			}
			if (5*h/10<y && y<6*h/10) {
				save(3, gamemode, dim, plateau1, plateau2, *score1, *score2, current_player);
			}
		}
		
	}
}


//Afficher le plateau de jeu
void displayPlateau(SDL_Renderer* pRenderer, int** plateau, int dim, TTF_Font* font, int x, int y, int w) {
	w /= dim;
	SDL_Color fontColor = {255, 255, 255};
	for (int i = 0; i < dim; i++) for (int j = 0; j < dim; j++) {
		int t = plateau[j][i];
		if (t <= 0) SDL_SetRenderDrawColor(pRenderer, 239, 239, 239, 255);
		else SDL_SetRenderDrawColor(pRenderer, (t+1)%12<6?255:(t+2)%12<8?127:0, (t+9)%12<6?255:(t+10)%12<8?127:0, (t+5)%12<6?255:(t+6)%12<8?127:0, 255);
		SDL_Rect r = {x+i*w+w/16, y+j*w+w/16, w-w/8, w-w/8};
		SDL_RenderFillRect(pRenderer, &r);
		if (t <= 0) continue;
		char text[10];
		snprintf(text, 10, "%d", (int)pow(2, t));
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, fontColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);
		SDL_Rect textR = {x+i*w+w/2-w/4*textSurface->w/textSurface->h, y+j*w+w/4, w/2*textSurface->w/textSurface->h, w/2};
		SDL_RenderCopy(pRenderer, textTexture, NULL, &textR);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}
}

//Affiche le plateau de jeu mais via le main
void displaySolo(SDL_Renderer* pRenderer, int** plateau, int dim, int w, int h, TTF_Font* font, int score) {
	displayPlateau(pRenderer, plateau, dim, font, 0, h/20, 19*h/20);
	displayGameBar(pRenderer, w, h);
	SDL_Color fontColor = {0, 0, 0};
	char scoreText[20];
	snprintf(scoreText, 20, "Score : %d", score);
	displayText(pRenderer, font, fontColor, scoreText, 0, 0, w, h/20);
}

//Prend un event clavier et agit en conséquence sur le plateau de jeu
void onSoloEvent(SDL_Event event, int *menu, int w, int h, int** plateau1, int dim, int *score, int *had2048) {
	int move = -1;
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.scancode) {
			case 82: // fleche haut
				move = UP;
				break;
			case 79: // fleche droite
				move = RIGHT;
				break;
			case 81: // fleche bas
				move = DOWN;
				break;
			case 80: // fleche gauche
				move = LEFT;
				break;
		}
		if (move != -1) {
			int s = movePlateau(plateau1, dim, move);
			if (s >= 0) {
				addRandomTile(plateau1, dim);
				*score += s;
			}
			if (isEndFull(plateau1, dim)) *menu = END_MENU;
			if (isEnd2048(plateau1, dim) && *had2048==0) {
				*menu = END_MENU;
				*had2048 = 1;
			}
		}
	}
	int tmp = 0;
	onGameBar(event, menu, SOLO, w, h, dim, plateau1, createPlateau(dim), score, &tmp, 0);
}


void displayVersusAI(SDL_Renderer* pRenderer, int** plateau1, int** plateau2, int dim, int w, int h, TTF_Font* font, int score1, int score2) {
	displayPlateau(pRenderer, plateau1, dim, font, 0, 0, h/2);
	displayPlateau(pRenderer, plateau2, dim, font, h/2, h/2, h/2);
	displayGameBar(pRenderer, w, h);
	SDL_Color fontColor = {0, 0, 0};
	displayText(pRenderer, font, fontColor, "Ordi =>", w/20, h-3*h/20, 100, h/20);
	displayText(pRenderer, font, fontColor, "<= Vous ", 2*w/3, h/20, 100, h/20);
	char score1Text[20];
	snprintf(score1Text, 20, "Score : %d", score1);
	displayText(pRenderer, font, fontColor, score1Text, 2*w/3, 2*h/20, 100, h/20);
	char score2Text[20];
	snprintf(score2Text, 20, "Score : %d", score2);
	displayText(pRenderer, font, fontColor, score2Text, w/20, h-h/10, 100, h/20);
}
void onVersusAIEvent(SDL_Event event, int *menu, int w, int h, int** plateau1, int** plateau2, int dim, int *score, int *score2) {
	if (event.type == SDL_KEYDOWN) {
		int move = -1;
		switch (event.key.keysym.scancode) {
			case 82: // fleche haut
				move = UP;
				break;
			case 79: // fleche droite
				move = RIGHT;
				break;
			case 81: // fleche bas
				move = DOWN;
				break;
			case 80: // fleche gauche
				move = LEFT;
				break;
		}
		if (move != -1) {
			int s = movePlateau(plateau1, dim, move);
			if (s >= 0) {
				addRandomTile(plateau1, dim);
				*score += s;
				if (isEndFull(plateau1, dim)) {
					*menu = END_MENU;
					return;
				}
				s = movePlateau(plateau2, dim, ai_getBest(plateau2, dim));
				if (s >= 0) {
					addRandomTile(plateau2, dim);
					*score2 += s;
				}
				if (isEndFull(plateau2, dim)) *menu = END_MENU;
			}
		}
	}
	onGameBar(event, menu, HUMAIN_VS_ORDI, w, h, dim, plateau1, plateau2, score, score2, 0);
}


void displayVersusPlayer(SDL_Renderer* pRenderer, int** plateau1, int** plateau2, int dim, int w, int h, TTF_Font* font, int score1, int score2) {
	displayPlateau(pRenderer, plateau1, dim, font, 0, 0, h/2);
	displayPlateau(pRenderer, plateau2, dim, font, h/2, h/2, h/2);
	displayGameBar(pRenderer, w, h);
	SDL_Color fontColor = {0, 0, 0};
	displayText(pRenderer, font, fontColor, "Joueur 2 =>", w/20, h-3*h/20, 100, h/20);
	displayText(pRenderer, font, fontColor, "<= Joueur 1 ", 2*w/3, h/20, 100, h/20);
	char score1Text[20];
	snprintf(score1Text, 20, "Score : %d", score1);
	displayText(pRenderer, font, fontColor, score1Text, 2*w/3, 2*h/20, 100, h/20);
	char score2Text[20];
	snprintf(score2Text, 20, "Score : %d", score2);
	displayText(pRenderer, font, fontColor, score2Text, w/20, h-h/10, 100, h/20);
}
void onVersusPlayerEvent(SDL_Event event, int *menu, int w, int h, int** plateau1, int** plateau2, int dim, int *score, int *score2, int *currentPlayer) {
	if (event.type == SDL_KEYDOWN) {
		int move = -1;
		switch (event.key.keysym.scancode) {
			case 82: // fleche haut
				move = UP;
				break;
			case 79: // fleche droite
				move = RIGHT;
				break;
			case 81: // fleche bas
				move = DOWN;
				break;
			case 80: // fleche gauche
				move = LEFT;
				break;
		}
		if (move != -1) {
			int s = movePlateau(*currentPlayer==0?plateau1:plateau2, dim, move);
			if (s >= 0) {
				addRandomTile(*currentPlayer==0?plateau1:plateau2, dim);
				if (*currentPlayer==0) *score += s;
				else *score2 += s;
				if (isEndFull(*currentPlayer==0?plateau1:plateau2, dim)) *menu = END_MENU;
				*currentPlayer=*currentPlayer==0?1:0;
			}
		}
	}
	onGameBar(event, menu, HUMAIN_VS_HUMAIN, w, h, dim, plateau1, plateau2, score, score2, *currentPlayer);
}


void displayEndMenu(SDL_Renderer* pRenderer, int w, int h, TTF_Font* font, int gamemode, int dim, int** plateau1, int** plateau2, int score1, int score2, int had2048, char bufferName[100]) {
	
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	displayHomeButton(pRenderer, w, h, 0, 0); // à supprimer ?
	SDL_Color fontColor = {0, 0, 0};
	
	switch (gamemode) {
		case SOLO:
			if (isEndFull(plateau1, dim)) {
				displayTextCenter(pRenderer, font, fontColor, "Votre grille est pleine :(", w/10, h/4, 4*w/5, h/10);
				displayTextCenter(pRenderer, font, fontColor, had2048?"Mais vous avez eu 2048":"Et vous n'avez pas de 2048...", w/10, h/3, 4*w/5, h/10);
				if (dim == 4 && isHighScore(score1)) {
					char recordText[30];
					snprintf(recordText, 30, "NOUVEAU RECORD !!! %d", score1);
					displayTextCenter(pRenderer, font, fontColor, recordText, w/10, h/2, 4*w/5, h/10);
					char yourName[120];
					snprintf(yourName, 100, "Ton nom : %s", bufferName);
					displayTextCenter(pRenderer, font, fontColor, yourName, w/10, 7*h/12, 4*w/5, h/10);
					SDL_Rect r = {w/4, 9*h/12, w/2, h/12};
					SDL_RenderCopy(pRenderer, buttonsTextures[2], NULL, &r);
					displayTextCenter(pRenderer, font, fontColor, "Valider", w/4, 9*h/12, w/2, h/10);
				}
			} else if (had2048) {
				displayTextCenter(pRenderer, font, fontColor, "Vous avez gagne !", w/10, h/4, 4*w/5, h/10);
				displayTextCenter(pRenderer, font, fontColor, "On continue ?", w/10, h/3, 4*w/5, h/10);
				SDL_Rect r1 = {w/10, 6*h/10, 3*w/10, h/6};
				SDL_RenderCopy(pRenderer, buttonsTextures[1], NULL, &r1);
				SDL_Rect r2 = {6*w/10, 6*h/10, 3*w/10, h/6};
				SDL_RenderCopy(pRenderer, buttonsTextures[3], NULL, &r2);
				displayTextCenter(pRenderer, font, fontColor, "Oui", w/10, 6*h/10, 3*w/10, h/6);
				displayTextCenter(pRenderer, font, fontColor, "Non", 6*w/10, 6*h/10, 3*w/10, h/6);
			}
		break;
		case HUMAIN_VS_ORDI:
			displayTextCenter(pRenderer, font, fontColor, score1==score2?"Égalité !":score1>score2?"Victoire !":"Défaite !", w/10, h/3, 4*w/5, h/10);
		break;
		case HUMAIN_VS_HUMAIN:
			displayTextCenter(pRenderer, font, fontColor, score1==score2?"Égalité !":score1>score2?"Victoire du joueur 1 !":"Victoire du joueur 2 !", w/10, h/3, 4*w/5, h/10);
		break;
	}
	
}
void onEndMenuEvent(SDL_Event event, int *menu, int gamemode, int w, int h, int dim, int** plateau1, int score1, int had2048, char bufferName[100]) {
	if (event.type == SDL_MOUSEBUTTONUP) {
		int x = event.motion.x;
		int y = event.motion.y;
		if (sqrt(pow(x-h/20, 2) + pow(y-h/20, 2)) < h/20) {
			*menu = MAIN_MENU;
		}
		if (gamemode==SOLO) {
			if (isEndFull(plateau1, dim)) {
				if (dim == 4 && isHighScore(score1) && strlen(bufferName)!=0) {
					if (w/4<x && x<w-w/4 && h-h/4<y && y<5*h/6) {
						addHighScore(score1, bufferName);
						*menu = MAIN_MENU;
					}
				}
			} else if (isEnd2048(plateau1, dim)) {
				if (6*h/10<y && y<6*h/10+h/6) {
					if (w/10<x && x<4*w/10)
						*menu = EN_JEU;
					if (6*w/10<x && x<9*w/10)
						*menu = MAIN_MENU;
				}
			}
		}
	}
	if (event.type == SDL_KEYDOWN) {
		if (gamemode==SOLO && isEndFull(plateau1, dim) && dim == 4 && isHighScore(score1)) {
			int key = event.key.keysym.sym;
			int mod = event.key.keysym.mod;
			if (strlen(bufferName) < 99) {
				if (97<=key && key<=122) {
					char c[2] = " ";
					c[0] = key + (mod&(KMOD_SHIFT|KMOD_CAPS)!=0 ? -32 : 0);
					strcat(bufferName, c);
				}
				if (32<=key && key<=64) {
					char c = key;
					strcat(bufferName, &c);
				}
			}
			if (strlen(bufferName) != 0) {
				if (key == 8)
					bufferName[strlen(bufferName)-1] = '\0';
				if (key == 13) {
					addHighScore(score1, bufferName);
					*menu = MAIN_MENU;
				}
			}
		}
	}
}


int main(int argc, char *argv[]) {
	
	int w = 640+64;
	int h = 640;
	
	if (SDL_Init(SDL_INIT_VIDEO) == -1) { // Démarrage de la SDL // SDL_INIT_EVENTS ?
		fprintf(stderr, "Erreur d'initialisation de la SDL");
		exit(EXIT_FAILURE);
	}
	
	SDL_Window* pWindow = NULL; // Création de la fenêtre
	pWindow = SDL_CreateWindow("2048 - Alili x Flo x ZaToX x Kéké", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	
	SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED); // Création du renderer (pour afficher)
	
	TTF_Init(); // Démarrage de SDL_ttf pour les textes
	TTF_Font* font = TTF_OpenFont("assets/elected-office.regular.ttf", 256); // Chargement de la police
	
	int menu = MAIN_MENU;
	int gamemode;
	int dim;
	int **plateau = NULL;
	int **plateau2 = NULL;
	int score = 0;
	int score2 = 0;
	int had2048 = 0;
	int currentPlayer = 0;
	char bufferName[100] = "";
	
	initTextures(pRenderer);

	displayMainMenu(pRenderer, font, w, h);
	while (1) {
		SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(pRenderer); // Netoyage du SDL_Renderer
		
		int endProgram = 0;
		
		SDL_Event event;
		while (SDL_PollEvent(&event)) { // les events
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
				endProgram = 1;
			
			switch (menu) {
				case MAIN_MENU:
					onMainMenuEvent(event, w, h, &menu);
				break;

				case CHOIX_DIM:
					onChoixDimEvent(event, w, h, &menu, &dim, &plateau, &plateau2, &score, &score2, &currentPlayer, &had2048);
				break;

				case MENU_JOUEUR:
					onMenuJoueur(event, w, h, &menu, &gamemode, &dim, &plateau, &plateau2, &score, &score2, &currentPlayer);
				break;

				case MENU_MEILLEURS_SCORES:
					onBestScoreEvent(event, w, h, &menu);
				break;

				case EN_JEU:
					switch (gamemode) {
						case SOLO:
							onSoloEvent(event, &menu, w, h, plateau, dim, &score, &had2048);
						break;

						case HUMAIN_VS_HUMAIN:
							onVersusPlayerEvent(event, &menu, w, h, plateau, plateau2, dim, &score, &score2, &currentPlayer);
						break;

						case HUMAIN_VS_ORDI:
							onVersusAIEvent(event, &menu, w, h, plateau, plateau2, dim, &score, &score2);
						break;
					}
				break;
				
				case END_MENU:
					onEndMenuEvent(event, &menu, gamemode, w, h, dim, plateau, score, had2048, bufferName);
				break;
			}
		}
		
		switch (menu) {
			case MAIN_MENU:
				displayMainMenu(pRenderer, font, w, h);
			break;
			case MENU_MEILLEURS_SCORES:
				displayBestScores(pRenderer, font, w, h);
			break;
			case CHOIX_DIM:
				displayDimChoice(pRenderer, font, w, h);
			break;
			case MENU_JOUEUR:
				displayMenuJouer(pRenderer, font, w, h);
			break;
			case EN_JEU:
				switch (gamemode) {
					case SOLO:
						displaySolo(pRenderer, plateau, dim, w, h, font, score);
					break;
					case HUMAIN_VS_ORDI:
						displayVersusAI(pRenderer, plateau, plateau2, dim, w, h, font, score, score2);
					break;
					case HUMAIN_VS_HUMAIN:
						displayVersusPlayer(pRenderer, plateau, plateau2, dim, w, h, font, score, score2);
					break;
				}
			break;
			case END_MENU:
				displayEndMenu(pRenderer, w, h, font, gamemode, dim, plateau, plateau2, score, score2, had2048, bufferName);
			break;
		}
		
		if (endProgram) break; // Condition d'arrêt du programme
		
		SDL_RenderPresent(pRenderer); // Affichage
		
		SDL_Delay(1000/30); // Attendre 1/30 seconde
		
	}


	TTF_CloseFont(font); // Libération de la mémoire associée à la police
	TTF_Quit(); // Arrêt de SDL_ttf
	
	SDL_DestroyRenderer(pRenderer); // Libération de la mémoire du SDL_Renderer
	
	SDL_DestroyWindow(pWindow); // Détruire la fenêtre
	
	SDL_Quit(); // Arrêt de la SDL
	
	return EXIT_SUCCESS;
	
}
