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
    printf("\n*************Card Test 4: Council Room**************\n");
    
    
    int opponentHandCount, handCount, seed, numBuys;
    struct gameState GameState;
    int k[10] = {minion, smithy, gardens, tribute, baron, adventurer, cutpurse, mine, embargo, outpost};
    seed = 10;
    initializeGame(2, k, seed, &GameState);
    
    
    GameState.whoseTurn = 1;
    GameState.hand[1][0] = council_room;
    GameState.deckCount[1] = 3;
   
    opponentHandCount = GameState.handCount[1];
    
    GameState.whoseTurn = 0;
    
    GameState.hand[0][0] = council_room;
    
    GameState.deckCount[0] = 3;
    
    
    
    
    handCount = GameState.handCount[0];
    numBuys = GameState.numBuys;
    int cardReturnValue =  cardEffect(council_room, 1, 1, 1, &GameState, handCount, NULL);
	printf("**Should +4 cards to the players hand:\n");
    ASSERT(handCount - 1 + 4 == GameState.handCount[0]);
	printf("\n**Should buy 1 extra card:\n");
	ASSERT(numBuys + 1 == GameState.numBuys);
	printf("\n**Opponent draws 1 card:\n");
	ASSERT(opponentHandCount + 1 == GameState.handCount[1]);
	printf("\n**Card returned from cardEffect with the correct value of 0:\n");
    	ASSERT(cardReturnValue == 0);
	
} 
