#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int main (int argc, char** argv) {
        printf("********Random Test Card 2 Village********");
        int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    int success = 0;
    int failed = 0;
    int i;
        int handSuccess=0;
        int handFailed=0;
        int handOpSuccess=0;
        int handOpFailed=0;
        int numActionSuccess=0;
        int numActionFailed=0;
        int deckCountSuccess=0;
        int deckCountFailed=0;
    for(i = 0; i < 50; i++) {

        int numOfPlayers = rand() % 4;
        int seeds = rand();
        struct gameState GameState;
        initializeGame(numOfPlayers, k, seeds, &GameState);

        GameState.deckCount[0] = rand() % MAX_DECK;
        GameState.discardCount[0] = rand() % MAX_DECK;
        int previousHand = GameState.handCount[0] = rand() % MAX_HAND;
        int opponentsHand = GameState.handCount[1] = rand() % MAX_HAND;
        int previousNumActions= GameState.numActions;
        int deckCount = GameState.deckCount[0]= rand() % MAX_DECK;
        cardEffect(village, 1, 1, 1, &GameState, 0, 0);

        if(previousHand  == GameState.handCount[0])
                handSuccess++;
        else
                handFailed++;

        if(opponentsHand == GameState.handCount[1])
                handOpSuccess++;
        else
                handOpFailed++;

        if(deckCount - 1== GameState.deckCount[0])
                deckCountSuccess++;
        else
                deckCountFailed++;


        if(previousNumActions +2== GameState.numActions)
                numActionSuccess++;
        else
                numActionFailed++;
        if(previousHand  == GameState.handCount[0] && opponentsHand == GameState.handCount[1] && deckCount - 1== GameState.deckCount[0] && previousNumActions +2== GameState.numActions )
            success++;
        else
            failed++;

        /*     free(GameState);*/
    }
        printf("\n%d correct numbers of card were added to player's hand  and %d wrong number of cards were added.\n", handSuccess, handFailed);
         printf("\n%d correct numbers of card were added to opponent's hand  and %d wrong number of cards were added.\n", handOpSuccess, handOpFailed);
         printf("\n%d correct that actions increased by 2 and %d wrong number of actions.\n", numActionSuccess, numActionFailed);
        printf("\n%d correct numbers of deck for player 1 decreased by 1 and %d wrong number of deck were decreased.\n", deckCountSuccess, deckCountFailed);
        printf("\nThere were %d successful adventurer card plays, and %d failures [out of 50 games]\n\n", success, failed);

    return 0;
}

