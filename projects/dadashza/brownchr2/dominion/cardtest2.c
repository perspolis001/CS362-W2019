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
    printf("\n*************Starting Card Test 2: Smithy**************\n");


    int opponentHandCount,opponentDeckCount, numActions, handCount, deckCount, seed;
    struct gameState GameState;
    int k[10] = {minion, smithy, gardens, tribute, baron, adventurer, cutpurse, mine, embargo, outpost};
    seed = 10;
    initializeGame(2, k, seed, &GameState);


    GameState.whoseTurn = 1;
    GameState.hand[1][0] = smithy;
    GameState.deckCount[1] = 3;
    opponentDeckCount = GameState.deckCount[1];
    opponentHandCount = GameState.handCount[1];

    GameState.whoseTurn = 0;

    GameState.hand[0][0] = smithy;

    GameState.deckCount[0] = 3;



    numActions = GameState.numActions;
    deckCount = GameState.deckCount[0];
    handCount = GameState.handCount[0];

    int cardReturnValue =  cardEffect(smithy, 1, 1, 1, &GameState, handCount, NULL);

        printf("**Should +3 cards to the players hand:\n");
     ASSERT(handCount - 1 + 3 == GameState.handCount[0]);
        printf("\n**Number of actions remained the same\n");
     ASSERT(numActions == GameState.numActions);
        printf("\n**Deck size was increased by 3:\n");
     ASSERT(deckCount - 3 == GameState.deckCount[0]);
        printf("\n**Opponent's hand size was not changed:\n");
     ASSERT(opponentHandCount == GameState.handCount[1]);
        printf("\n**Oppenent's deck size was not altered:\n");
     ASSERT(opponentDeckCount == GameState.deckCount[1]);
        printf("\n**Card returned from cardEffect with the correct value of 0:\n");
     ASSERT(cardReturnValue == 0);

}

