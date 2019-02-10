#include "dominion.h"
#include "dominion_helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include "rngs.h"
#include "assert.h"

void ASSERT(int test) {
    if (test) {
        printf ("TEST PASSED!\n");
        
    }
    else{
        printf("TEST FAILED!\n");
        
    }
}

int main () {
	int score = 0;
	struct gameState GameState;
    printf("\n*************Unit Test 1: scoreFor**************\n");
	GameState.handCount[0]=5;
        GameState.hand[0][0]= province;
        GameState.hand[0][1]=curse;
        GameState.hand[0][2]=estate;
        GameState.hand[0][3]=curse;
        GameState.hand[0][3]=duchy;
	
	score = scoreFor(0, &GameState);
	
	printf("\n**Validate score from hand:\n");
	printf("Estimated score from hand: 8\n ");
        printf("Actual score from hand: %d\n ", score);
	ASSERT(score==8);
	
	score=0;
	printf("\n**Validate score from discard:\n");
	GameState.handCount[0]=5;
        GameState.hand[0][0]=duchy;
        GameState.hand[0][1]=curse;
        GameState.hand[0][2]=estate;
        GameState.hand[0][3]=curse;
        GameState.hand[0][3]=great_hall;
	score = scoreFor(0, &GameState);
        
	printf("\n**Expected score from discard: 3\n ");
        printf("Actual score from discard: %d\n ", score);
	ASSERT(score==3);
	
	score=0;
        printf("\n**Validate score from deck:\n");
	GameState.handCount[0]=5;
        GameState.hand[0][0]=duchy;
        GameState.hand[0][1]=curse;
        GameState.hand[0][2]=estate;
        GameState.hand[0][3]=curse;
        GameState.hand[0][3]=gardens;
	score = scoreFor(0, &GameState);
        printf("Expected score from deck: 2\n ");
        printf("Actual score from deck: %d\n ", score);
	ASSERT(score==2);

	
}
