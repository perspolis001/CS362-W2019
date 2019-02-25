#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int main (int argc, char** argv) {
        printf("********Random Test Card 1 Council Room********");
        int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    int success = 0;
    int failed = 0;
    int i;
        int handSuccess=0;
        int handFailed=0;
        int handOpSuccess=0;
        int handOpFailed=0;
        int numBuysSuccess=0;
        int numBuysFailed=0;
    for(i = 0; i < 50; i++) {

        int numOfPlayers = rand() % 4;
        int seeds = rand();
        struct gameState GameState;
        initializeGame(numOfPlayers, k, seeds, &GameState);

        GameState.deckCount[0] = rand() % MAX_DECK;
        GameState.discardCount[0] = rand() % MAX_DECK;
        int previousHand = GameState.handCount[0] = rand() % MAX_HAND;
        int opponentsHand = GameState.handCount[1] = rand() % MAX_HAND;
        int previousNumBuys= GameState.numBuys;

        cardEffect(council_room, 1, 1, 1, &GameState, 0, 0);

        if(previousHand + 3 == GameState.handCount[0])
                handSuccess++;
        else
                handFailed++;

        if(opponentsHand+1 == GameState.handCount[1])
                handOpSuccess++;
        else
                handOpFailed++;

        if(previousNumBuys +1== GameState.numBuys)
                numBuysSuccess++;
        else
                numBuysFailed++;
        if(previousHand + 3 == GameState.handCount[0] && opponentsHand+1 == GameState.handCount[1] && previousNumBuys +1== GameState.numBuys )
            success++;
        else
            failed++;

        /*     free(GameState);*/
    }
        printf("\n%d correct numbers of card were added to player's hand  and %d wrong number of cards were added.\n", handSuccess, handFailed);
         printf("\n%d correct numbers of card were added to opponent's hand  and %d wrong number of cards were added.\n", handOpSuccess, handOpFailed);
         printf("\n%d correct that numBuys increased by 1 and %d wrong number of numbuys.\n", numBuysSuccess, numBuysFailed);
        printf("\nThere were %d successful adventurer card plays, and %d failures [out of 50 games]\n\n", success, failed);

    return 0;
}

