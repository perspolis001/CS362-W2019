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

int main(){
        printf("\n*************Starting Card Test 1: Adventurer**************\n");

int opponentHandCount,opponentDeckCount, numActions, handCount, deckCount, seed;
    struct gameState GameState;
    int k[10] = {minion, smithy, gardens, tribute, baron, adventurer, cutpurse, mine, embargo, outpost};
    seed = 10;
        initializeGame(2, k, seed, &GameState);
         GameState.whoseTurn = 1;
    GameState.hand[1][0] = adventurer;
    GameState.deckCount[1] = 2;
    opponentDeckCount = GameState.deckCount[1];
    opponentHandCount = GameState.handCount[1];


        GameState.whoseTurn = 0;

    GameState.hand[0][0] = adventurer;
    GameState.handCount[0]= 1;
    GameState.deck[0][0]=copper;
   GameState.deck[0][1]=copper;
    GameState.deckCount[0] = 2;
    numActions = GameState.numActions;
    deckCount = GameState.deckCount[0];
    handCount = GameState.handCount[0];
        int cardReturnValue = cardEffect(adventurer, 1, 1, 1, &GameState, handCount, NULL);
        //int adventureFunctionReturn = playAdventurer(&GameState);
        printf("**Validate 2 cards added to players hand:\n");
        ASSERT((handCount + 2) == GameState.handCount[0]);
        printf("\n**Validate number of actions didn't change:\n");
        ASSERT(numActions == GameState.numActions);
        printf("\n**Validate the deck decreased by 3 cards:\n");
        ASSERT(deckCount > fullDeckCount(0,0, &GameState));
        printf("\n**Validate the opponent's handcount didn't change:\n");
        ASSERT(opponentHandCount == GameState.handCount[1]);
        printf("\n**Validate the  opponent's deckCount didn't change:\n");
        ASSERT(opponentDeckCount == GameState.deckCount[1]);
        printf("\n**Validate first card is a treasure\n");
        ASSERT(GameState.hand[0][2] == copper && GameState.hand[0][1] == copper);
        printf("\n** Validate card return value didn't change\n");
        ASSERT(cardReturnValue ==0);
        //printf("\n**Validate if cardEffect and refactoredAdventurer return the same value\n");
        //ASSERT(cardReturnValue == adventureFunctionReturn);



}

