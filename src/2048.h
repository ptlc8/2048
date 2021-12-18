#ifndef _2048_H
#define _2048_H

#include <math.h>
#include <time.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

int** createPlateau(int dim){
	int** plateau;

  	plateau = malloc((dim * sizeof(int*)));

  	for(int i=0; i<dim; i++){
  		plateau[i]=malloc((dim * sizeof(int*)));
    	for(int j=0; j<dim; j++){
      		plateau[i][j]=0;
    	}
  	}

  	return plateau;
}

void printPlateau(int** plateau, int dim){
  printf("+---+---+---+---+\n");
	for(int i=0; i<dim; i++){
		printf("|");
    	for(int j=0; j<dim; j++){
        if (plateau[i][j]==0)
          printf("   |");
  			else printf(" %d |", (int)pow(2, plateau[i][j]));
		}
		printf("\n+---+---+---+---+\n");
	}
}

void addRandomTile(int** plateau, int dim){
	int x=0, y=0;
	do{
 		x=rand()%(dim);
 		y=rand()%(dim);
 	}while(plateau[x][y]!=0);
 	plateau[x][y]=1;

}

int isEnd2048(int** plateau, int dim){

	int res=0;
	for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++){
			if(plateau[i][j]==11){
				res+=1;
			}
		}
	}
	return res;
}

int isEndFull(int** plateau, int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      if (i>0 && plateau[i-1][j] == plateau[i][j]) return 0;
      if (j>0 && plateau[i][j-1] == plateau[i][j]) return 0;
      if (i<dim-1 && plateau[i+1][j] == plateau[i][j]) return 0;
      if (j<dim-1 && plateau[i][j+1] == plateau[i][j]) return 0;
    }
  }
  return 1;
}

void rotateBoard(int** tableau, int dim) {
    for (int i=0; i<dim/2; i++) {
        for (int j=i; j<dim-i-1; j++) {
            int tmp = tableau[i][j];
            tableau[i][j] = tableau[j][dim-i-1];
            tableau[j][dim-i-1] = tableau[dim-i-1][dim-j-1];
            tableau[dim-i-1][dim-j-1] = tableau[dim-j-1][i];
            tableau[dim-j-1][i] = tmp;
        }
    }
}

int moveUp(int** plateau, int dim) {
  int moved = 0;
  int score = 0;
  for (int j = 0; j < dim; j++) {
    for (int i = 0; i < dim-1; i++) {
      int merged = 0;
      for (int k = i; k>=0 && (plateau[k][j]==0 || (plateau[k][j]==plateau[k+1][j] && !merged)); k--) {
        if (plateau[k][j] == 0 && plateau[k+1][j] != 0) {
          plateau[k][j] = plateau[k+1][j];
          plateau[k+1][j] = 0;
          moved = 1;
        } else if (plateau[k][j] != 0 && plateau[k][j] == plateau[k+1][j] && !merged) {
          plateau[k][j]++;
          plateau[k+1][j] = 0;
          score += pow(2, plateau[k][j]);
          merged = 1;
          moved = 1;
        }
      }
    }
  }
  return moved ? score : -1;
}

/*int moveLeft(int** plateau, int dim) {
  // TODO
  return 0;
}
int moveDown(int** plateau, int dim){

	int scored=0;
  //for(int k=0; k<dim; k++){
    for(int i=dim-2; i>=0; i--){
      for(int j=0; j<dim; j++){
        printf("a");
        if(plateau[i][j]==plateau[i+1][j]){
          plateau[i+1][j]=plateau[i+1][j]+1;
          plateau[i][j]=1;
          scored+=pow(plateau[i+1][j],2);
        }
        else if(plateau[i+1][j]==0){
          plateau[i+1][j]=plateau[i][j];
          plateau[i][j]=1;
        }
      }
    }
  //}
	return scored;
}
int moveRight(int** plateau, int dim) {
  // TODO
  return 0;
}*/
int moveRight(int** plateau, int dim) {
    int success;
    rotateBoard(plateau, dim);
    success = moveUp(plateau, dim);
    rotateBoard(plateau, dim);
    rotateBoard(plateau, dim);
    rotateBoard(plateau, dim);
    return success;
}

int moveDown(int** plateau, int dim) {
    int success;
    rotateBoard(plateau, dim);
    rotateBoard(plateau, dim);
    success = moveUp(plateau, dim);
    rotateBoard(plateau, dim);
    rotateBoard(plateau, dim);
    return success;
}

int moveLeft(int** plateau, int dim) {
    int success;
    rotateBoard(plateau, dim);
    rotateBoard(plateau, dim);
    rotateBoard(plateau, dim);
    success = moveUp(plateau, dim);
    rotateBoard(plateau, dim);
    return success;
}

int movePlateau(int** plateau, int dim, int move) {
  switch (move) {
    case UP:
      return moveUp(plateau, dim);
    case RIGHT:
      return moveRight(plateau, dim);
    case DOWN:
      return moveDown(plateau, dim);
    case LEFT:
      return moveLeft(plateau, dim);
    default:
      return -1;
  }
}

int** clonePlateau(int** plateau, int dim) {
  int** p = malloc(sizeof(int*) * dim);
  for (int i = 0; i < dim; i++) {
    p[i] = malloc(sizeof(int) * dim);
    for (int j = 0; j < dim; j++) {
      p[i][j] = plateau[i][j];
    }
  }
  return p;
}

void freePlateau(int** plateau, int dim) {
  if (plateau == NULL) return;
  for (int i = 0; i < dim; i++) {
    free(plateau[i]);
  }
  free(plateau);
}

#endif
