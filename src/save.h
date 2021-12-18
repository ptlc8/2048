#include "2048.h"


/*sauvegarde et chargement d'une partie*/
int save(int n, int gamemode, int dim, int** plateau1, int** plateau2, int score1, int score2, int current_player) {
	FILE *p;
	char fileName[20];
	snprintf(fileName, 20, "saves/save%d.txt", n);
	p=fopen(fileName, "w");
	if(p==NULL){
        printf("Erreur de sauvegarde %d\n", n);
        return 0;
    }
    fprintf(p, "%d %d %d %d %d\n", gamemode, dim, score1, score2, current_player); 
    for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			fprintf(p,"%d\n", plateau1[i][j]);
        }
    }
    for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			fprintf(p,"%d\n", plateau2[i][j]);
        }
    }
    fclose(p);
    return 1;
}

int charge(int n, int *gamemode, int *dim, int** *plateau1, int** *plateau2, int *score1, int *score2, int *current_player) {
	FILE *p;
	char fileName[20];
	snprintf(fileName, 20, "saves/save%d.txt", n);
	p=fopen(fileName, "r");
	if(p==NULL){
        printf("Erreur de chargement de la sauvegarde %d\n", n);
        return 0;
    }
    
    fscanf(p, "%d %d %d %d %d", gamemode, dim, score1, score2, current_player); 
    
	//freePlateau(plateau1);
	*plateau1 = createPlateau(*dim);
    for(int i=0;i<*dim;i++){
        for(int j=0;j<*dim;j++){
            fscanf(p,"%d",&(*plateau1)[i][j]);
        }
    }
    
    //freePlateau(plateau2);
	*plateau2 = createPlateau(*dim);
    for(int i=0;i<*dim;i++){
        for(int j=0;j<*dim;j++){
            fscanf(p,"%d",&(*plateau2)[i][j]);
        }
    }
	fclose(p);
	return 1;
}
