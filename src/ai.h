#ifndef AI_H
#define AI_H

#include "2048.h"

int ai_randomRun(int** plateau, int dim, int move, int limit) {
	int** p = clonePlateau(plateau, dim);
	int score = 0;
	int s = movePlateau(p, dim, move);
	if (s == -1)
		return -1;
	score += s;
	addRandomTile(p, dim);
	
	int moves=1;
	for (int i = 0; i < limit; i++) {
		if (isEndFull(p, dim)) break;
		s = movePlateau(p, dim, rand()%4);
		if (s == -1) continue;
		score += s;
		addRandomTile(p, dim);
	}
	
	freePlateau(p, dim);
	return score;
}

int ai_multiRandomRun(int** plateau, int dim, int move, int runs) {
	int total = 0;
	int min = 1000000;
	int max = 0;
	
	for (int i = 0; i < runs; i++) {
		int s = ai_randomRun(plateau, dim, move, runs);
		if (s == -1) return -1;
		total += s;
		if (s < min) min = s;
		if (s > max) max = s;
	}
	
	int avg = total / runs;
	return avg;
	//return max;
}

int ai_getBestMove(int** plateau, int dim, int runs) {
	int bestScore = 0; 
	int bestMove = -1;
	
	for (int i = 0; i < 4; i++) {
		int score = ai_multiRandomRun(plateau, dim, i, runs);
		if (score >= bestScore) {
			bestScore = score;
			bestMove = i;
		}
	}
	
	return bestMove;
}

int ai_getBest(int** plateau, int dim) {
	int runs = 100;
	return ai_getBestMove(plateau, dim, runs);
}

#endif
