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
        struct gameState GameState;

	 int k[10] = {1,2,3,4,5,6,7,8,9,10};

	initializeGame(2, k, 2, &GameState);
	GameState.supplyCount[province] = 5;
        printf("\n*********Unit Test 3: isGameOver**********\n");
        printf("\n**Validate if the game won't end if player has province\n");
       	ASSERT(isGameOver(&GameState)==0);

	GameState.supplyCount[province] = 0;
        printf("\n**Validate if the game will end when no province left\n");
        ASSERT(isGameOver(&GameState)==1);

	GameState.supplyCount[0] = 0;
	GameState.supplyCount[1] = 0;
	GameState.supplyCount[1] = 0;
        printf("\n**Validate if the game will end if supply pile are at 0\n");
        ASSERT(isGameOver(&GameState)==1);

	
}
