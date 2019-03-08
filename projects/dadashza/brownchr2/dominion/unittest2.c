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
        printf("\n******Unit Test 2: shuffle******\n");

        GameState.deckCount[0]=3;
        GameState.deck[0][0]= copper;
        GameState.deck[0][1]= silver;
        GameState.deck[0][2]= gold;

        printf("\n**Validate if the function will give correct output numbers from the input numbers\n");
        ASSERT(shuffle(0, &GameState) == 0);

        int cards[MAX_DECK]= {0};
        int i;
        for(i=0; i < 3; i++)
        {
                cards[GameState.deck[0][i]]=1;
        }

        printf("\n**Validate that the cards are all still there\n");
        ASSERT(cards[copper] && cards[silver] && cards[gold]);


}

