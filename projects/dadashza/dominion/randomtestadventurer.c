#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int main (int argc, char** argv) {
    	printf("********Random Test Adventurer********");
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    int success = 0;
    int failed = 0;
    int i;
	int handSuccess=0;
	int handFailed=0;
	int handOpSuccess=0;
        int handOpFailed=0;
    	int deckSuccess=0;
	int deckFailed=0;
    for(i = 0; i < 50; i++) {
        
        int numOfPlayers = rand() % 4;
        int seeds = rand();
        struct gameState GameState;
        initializeGame(numOfPlayers, k, seeds, &GameState);
        
        GameState.deckCount[0] = rand() % MAX_DECK;
        GameState.discardCount[0] = rand() % MAX_DECK;
        int previousHand = GameState.handCount[0] = rand() % MAX_HAND;
        int opponentsHand = GameState.handCount[1] = rand() % MAX_HAND;
        int opponentsDeck = GameState.deckCount[1] = rand() % MAX_DECK;
        int checkStatus = 0,x;
        
        for(x = 0 ; x < GameState.handCount[0]; x++) {
            if((GameState.hand[0][x] == gold) || (GameState.hand[0][x] == silver) || (GameState.hand[0][x] == copper)) {
                checkStatus++;
            }
        }
        cardEffect(adventurer, 1, 1, 1, &GameState, 0, 0);
        
        
        for(x = 0 ; x < GameState.handCount[0]; x++) {
            if((GameState.hand[0][x] == gold) || (GameState.hand[0][x] == silver) || (GameState.hand[0][x] == copper)) {
                checkStatus--;
            }
        }	
	if(previousHand + 1 == GameState.handCount[0])
		handSuccess++;
	else
		handFailed++;

	if(opponentsHand == GameState.handCount[1])			
		handOpSuccess++;
	else
		handOpFailed++;

	if(opponentsDeck == GameState.deckCount[1])
		deckSuccess++;
	else
		deckFailed++;
        if(checkStatus < 0 && previousHand + 1 == GameState.handCount[0] && opponentsHand == GameState.handCount[1] && opponentsDeck == GameState.deckCount[1])
            success++;
        else
            failed++;
        
        /*     free(GameState);*/
    }
	printf("\n%d correct numbers of card were added to player's hand  and %d wrong number of cards were added.\n", handSuccess, handFailed);
	 printf("\n%d correct numbers of card were added to opponent's hand  and %d wrong number of cards were added.\n", handOpSuccess, handOpFailed);
	 printf("\n%d correct numbers of deck count opponent's hand  and %d wrong number of deck count.\n", deckSuccess, deckFailed);
	printf("\nThere were %d successful adventurer card plays, and %d failures [out of 50 games]\n\n", success, failed);
    
    return 0;
}


