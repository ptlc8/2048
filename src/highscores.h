#ifndef SCORE_H
#define SCORE_H


#include <string.h>


typedef struct Save Save;

struct Save{
  char nom[100];
  int score;
};


int toInt(char* str) {
	int n = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		n *= 10;
		n += str[i] - '0';
	}
	return n;
}


Save* getHighScores(int* nbrInscrits) {
	
	Save* highScores;
	FILE* fichier = NULL;
	fichier = fopen("saves/highscores.txt", "r");
	if(fichier!=NULL){
		char ch;
		char str[100] = "";
		int i = 0, nextCharIndex = 0;
		Save currentHighScore;
		while (1) { //EOF représente la fin d'un fichier texte
			ch = fgetc(fichier);
			if (ch == '\n' || ch == EOF) {
				if (i==0) {
					highScores = malloc((*nbrInscrits = toInt(str)) * sizeof(Save));
				} else if (i%2!=0) {
					strcpy(currentHighScore.nom, str);
				} else {
					currentHighScore.score = toInt(str);
					highScores[i/2-1].score = currentHighScore.score;
					strcpy(highScores[i/2-1].nom, currentHighScore.nom);
				}
				str[0] = '\0';
				nextCharIndex = 0;
				i++;
			} else {
				str[nextCharIndex] = ch;
				nextCharIndex++;
				str[nextCharIndex] = '\0';
			}
			if (ch == EOF) break;
		}
	}
	fclose(fichier);
	return highScores;
}


void printHighScores(){
  
  int nbrInscrits;
  Save* tabSaveDisp = getHighScores(&nbrInscrits);

  for(int j=0; j<nbrInscrits; j++){
    printf("%s : %d\n", tabSaveDisp[j].nom, tabSaveDisp[j].score);
  }

}


int isHighScore(int score){

  int nbrInscrits;
  Save* tabSave = getHighScores(&nbrInscrits);
  
  for(int i=0; i<10; i++){
    if(score>=tabSave[i].score) return 1;
  }
  return 0;

} 


void triHighScores(Save* tabSave, int nbrInscrits){

  Save tmp;
  for(int j=0; j<nbrInscrits; j++){
    for(int i=0; i<nbrInscrits; i++){
      if(tabSave[i].score<tabSave[i+1].score){
        tmp.score=tabSave[i].score;
        strcpy(tmp.nom, tabSave[i].nom);
        tabSave[i].score=tabSave[i+1].score;
        strcpy(tabSave[i].nom, tabSave[i+1].nom);
        tabSave[i+1].score=tmp.score;
        strcpy(tabSave[i+1].nom, tmp.nom);
      }
    }
  }
}


void addHighScore(int score, char name[100]){
  
  int nbrInscrits;
  Save* tabSave = getHighScores(&nbrInscrits);
  
  if(nbrInscrits<10) nbrInscrits+=1;
  
  strcpy(tabSave[9].nom, name);
  tabSave[9].score = score;
  triHighScores(tabSave, nbrInscrits);

  FILE* fichier;
  fichier = fopen("saves/highscores.txt", "w+");
  if(fichier!=NULL){
    fprintf(fichier, "%d", nbrInscrits);
    for(int j=0; j<nbrInscrits; j++){
      fprintf(fichier, "\n%s\n%d", tabSave[j].nom, tabSave[j].score);
    }
  }
  fclose(fichier);

}

#endif
