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
	printf("***********Unit Test 4: BuyCard ************\n");

	initializeGame(2, k, 2, &GameState);

	GameState.numBuys = 0;
	printf("**Validate if player denied from buying when no buy left:\n");
	ASSERT(buyCard(5, &GameState)==-1);

	GameState.numBuys = 5;
	GameState.coins=0;

	printf("\n**Validate if player denied from buying when not enough coins:\n");
        ASSERT(buyCard(5, &GameState)==-1);
	
	GameState.coins=100;

	printf("\n**Validate if player can make a purchase:\n");
	ASSERT(buyCard(5, &GameState)==0);
	
}	
