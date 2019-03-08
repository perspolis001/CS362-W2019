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
    printf("\n*************Starting Card Test 3: Village**************\n");


    int opponentHandCount,opponentDeckCount, numActions, handCount, deckCount, seed;
    struct gameState GameState;
    int k[10] = {minion, smithy, gardens, tribute, baron, adventurer, cutpurse, mine, embargo, outpost};
    seed = 10;
    initializeGame(2, k, seed, &GameState);


    GameState.whoseTurn = 1;
    GameState.hand[1][0] = village;
    GameState.deckCount[1] = 3;
    opponentDeckCount = GameState.deckCount[1];
    opponentHandCount = GameState.handCount[1];

    GameState.whoseTurn = 0;

    GameState.hand[0][0] = village;

    GameState.deckCount[0] = 3;



    numActions = GameState.numActions;
    deckCount = GameState.deckCount[0];
    handCount = GameState.handCount[0];

    int cardReturnValue =  cardEffect(village, 1, 1, 1, &GameState, handCount, NULL);
        printf("**Validate the handcount increased by 1 cards:\n");
        ASSERT(handCount == GameState.handCount[0]);
        printf("\n**Validate the number of actions increased by 2:\n");
        ASSERT(numActions + 2 == GameState.numActions);
        printf("\n**Validate the deck decreased by 1 cards:\n");
        ASSERT(deckCount - 1 == GameState.deckCount[0]);
         printf("\n**Validate opponent's deckCount didn't change:\n");
        ASSERT(opponentDeckCount == GameState.deckCount[1]);
        printf("\n**Validate opponent's hand count didn't change:\n");
        ASSERT(opponentHandCount == GameState.handCount[1]);
        printf("\n**Validate card return value is correct:\n");
        ASSERT(cardReturnValue ==0);
}

