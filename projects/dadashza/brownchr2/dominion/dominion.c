#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void playAdventurer(struct gameState*);
void playSmithy(int, struct gameState*);
void playVillage(int, struct gameState*);
void playGreatHall(int, struct gameState*);
void playCouncilRoom(int, struct gameState*);

int compare(const void *a, const void *b) {
    if (*(int *) a > *(int *) b)
        return 1;
    if (*(int *) a < *(int *) b)
        return -1;
    return 0;
}

struct gameState *newGame() {
    struct gameState *g = malloc(sizeof(struct gameState));
    return g;
}

int *kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7,
                  int k8, int k9, int k10) {
    int *k = malloc(10 * sizeof(int));
    k[0] = k1;
    k[1] = k2;
    k[2] = k3;
    k[3] = k4;
    k[4] = k5;
    k[5] = k6;
    k[6] = k7;
    k[7] = k8;
    k[8] = k9;
    k[9] = k10;
    return k;
}

int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
                   struct gameState *state) {
    
    int i;
    int j;
    int it;
   
    SelectStream(1);
    PutSeed((long) randomSeed);
    
    
    if (numPlayers > MAX_PLAYERS || numPlayers < 2) {
        return -1;
    }
    
    
    state->numPlayers = numPlayers;
    
    
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (j != i && kingdomCards[j] == kingdomCards[i]) {
                return -1;
            }
        }
    }
    
    
    
    if (numPlayers == 2) {
        state->supplyCount[curse] = 10;
    } else if (numPlayers == 3) {
        state->supplyCount[curse] = 20;
    } else {
        state->supplyCount[curse] = 30;
    }
    
    
    if (numPlayers == 2) {
        state->supplyCount[estate] = 8;
        state->supplyCount[duchy] = 8;
        state->supplyCount[province] = 8;
    } else {
        state->supplyCount[estate] = 12;
        state->supplyCount[duchy] = 12;
        state->supplyCount[province] = 12;
    }
    
    
    state->supplyCount[copper] = 60 - (7 * numPlayers);
    state->supplyCount[silver] = 40;
    state->supplyCount[gold] = 30;
    
    
    for (i = adventurer; i <= treasure_map; i++)        //loop all cards
    {
        for (j = 0; j < 10; j++)                //loop chosen cards
        {
            if (kingdomCards[j] == i) {
                
                if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens) {
                    if (numPlayers == 2) {
                        state->supplyCount[i] = 8;
                    } else { state->supplyCount[i] = 12; }
                } else {
                    state->supplyCount[i] = 10;
                }
                break;
            } else    //card is not in the set choosen for the game
            {
                state->supplyCount[i] = -1;
            }
        }
        
    }
    
    
    for (i = 0; i < numPlayers; i++) {
        state->deckCount[i] = 0;
        for (j = 0; j < 3; j++) {
            state->deck[i][j] = estate;
            state->deckCount[i]++;
        }
        for (j = 3; j < 10; j++) {
            state->deck[i][j] = copper;
            state->deckCount[i]++;
        }
    }
    
    
    for (i = 0; i < numPlayers; i++) {
        if (shuffle(i, state) < 0) {
            return -1;
        }
    }
    
    
    for (i = 0; i < numPlayers; i++) {
        
        state->handCount[i] = 0;
        state->discardCount[i] = 0;
       
    }
    
    
    for (i = 0; i <= treasure_map; i++) {
        state->embargoTokens[i] = 0;
    }
    
    
    state->outpostPlayed = 0;
    state->phase = 0;
    state->numActions = 1;
    state->numBuys = 1;
    state->playedCardCount = 0;
    state->whoseTurn = 0;
    state->handCount[state->whoseTurn] = 0;
    
    for (it = 0; it < 5; it++) {
        drawCard(state->whoseTurn, state);
    }
    
    updateCoins(state->whoseTurn, state, 0);
    
    return 0;
}

int shuffle(int player, struct gameState *state) {
    
    
    int newDeck[MAX_DECK];
    int newDeckPos = 0;
    int card;
    int i;
    
    if (state->deckCount[player] < 1)
        return -1;
    qsort((void *) (state->deck[player]), state->deckCount[player], sizeof(int), compare);
    /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */
    
    while (state->deckCount[player] > 0) {
        card = floor(Random() * state->deckCount[player]);
        newDeck[newDeckPos] = state->deck[player][card];
        newDeckPos++;
        for (i = card; i < state->deckCount[player] - 1; i++) {
            state->deck[player][i] = state->deck[player][i + 1];
        }
        state->deckCount[player]--;
    }
    for (i = 0; i < newDeckPos; i++) {
        state->deck[player][i] = newDeck[i];
        state->deckCount[player]++;
    }
    
    return 0;
}

int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state) {
    int card;
    int coin_bonus = 0;        //tracks coins gain from actions
    
    
    if (state->phase != 0) {
        return -1;
    }
    
    
    if (state->numActions < 1) {
        return -1;
    }
    
    
    card = handCard(handPos, state);
    
    
    if (card < adventurer || card > treasure_map) {
        return -1;
    }
    
    
    if (cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus) < 0) {
        return -1;
    }
    
    
    state->numActions--;
    
    
    updateCoins(state->whoseTurn, state, coin_bonus);
    
    return 0;
}

int buyCard(int supplyPos, struct gameState *state) {
    int who;
    if (DEBUG) {
        printf("Entering buyCard...\n");
    }
    
    
    
    who = state->whoseTurn;
    
    if (state->numBuys < 1) {
        if (DEBUG)
            printf("You do not have any buys left\n");
        return -1;
    } else if (supplyCount(supplyPos, state) < 1) {
        if (DEBUG)
            printf("There are not any of that type of card left\n");
        return -1;
    } else if (state->coins < getCost(supplyPos)) {
        if (DEBUG)
            printf("You do not have enough money to buy that. You have %d coins.\n", state->coins);
        return -1;
    } else {
        state->phase = 1;
        
        gainCard(supplyPos, state, 0,
                 who); //card goes in discard, this might be wrong.. (2 means goes into hand, 0 goes into discard)
        
        state->coins = (state->coins) - (getCost(supplyPos));
        state->numBuys--;
        if (DEBUG)
            printf("You bought card number %d for %d coins. You now have %d buys and %d coins.\n", supplyPos,
                   getCost(supplyPos), state->numBuys, state->coins);
    }
    
    
    
    return 0;
}

int numHandCards(struct gameState *state) {
    return state->handCount[whoseTurn(state)];
}

int handCard(int handPos, struct gameState *state) {
    int currentPlayer = whoseTurn(state);
    return state->hand[currentPlayer][handPos];
}

int supplyCount(int card, struct gameState *state) {
    return state->supplyCount[card];
}

int fullDeckCount(int player, int card, struct gameState *state) {
    int i;
    int count = 0;
    
    for (i = 0; i < state->deckCount[player]; i++) {
        if (state->deck[player][i] == card) count++;
    }
    
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] == card) count++;
    }
    
    for (i = 0; i < state->discardCount[player]; i++) {
        if (state->discard[player][i] == card) count++;
    }
    
    return count;
}

int whoseTurn(struct gameState *state) {
    return state->whoseTurn;
}

int endTurn(struct gameState *state) {
    int k;
    int i;
    int currentPlayer = whoseTurn(state);
    
    
    for (i = 0; i < state->handCount[currentPlayer]; i++) {
        state->discard[currentPlayer][state->discardCount[currentPlayer]++] = state->hand[currentPlayer][i];//Discard
        state->hand[currentPlayer][i] = -1;//Set card to -1
    }
    state->handCount[currentPlayer] = 0;//Reset hand count
    
    
    if (currentPlayer < (state->numPlayers - 1)) {
        state->whoseTurn = currentPlayer + 1;//Still safe to increment
    } else {
        state->whoseTurn = 0;//Max player has been reached, loop back around to player 1
    }
    
    state->outpostPlayed = 0;
    state->phase = 0;
    state->numActions = 1;
    state->coins = 0;
    state->numBuys = 1;
    state->playedCardCount = 0;
    state->handCount[state->whoseTurn] = 0;
    
    
    for (k = 0; k < 5; k++) {
        drawCard(state->whoseTurn, state);//Draw a card
    }
    
    
    updateCoins(state->whoseTurn, state, 0);
    
    return 0;
}

int isGameOver(struct gameState *state) {
    int i;
    int j;
    
    
    if (state->supplyCount[province] == 0) {
        return 1;
    }
    
    
    j = 0;
    for (i = 0; i < 25; i++) {
        if (state->supplyCount[i] == 0) {
            j++;
        }
    }
    if (j >= 3) {
        return 1;
    }
    
    return 0;
}

int scoreFor(int player, struct gameState *state) {
    
    int i;
    int score = 0;
    
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] == curse) { score = score - 1; };
        if (state->hand[player][i] == estate) { score = score + 1; };
        if (state->hand[player][i] == duchy) { score = score + 3; };
        if (state->hand[player][i] == province) { score = score + 6; };
        if (state->hand[player][i] == great_hall) { score = score + 1; };
        if (state->hand[player][i] == gardens) { score = score + (fullDeckCount(player, 0, state) / 10); };
    }
    
    
    for (i = 0; i < state->discardCount[player]; i++) {
        if (state->discard[player][i] == curse) { score = score - 1; };
        if (state->discard[player][i] == estate) { score = score + 1; };
        if (state->discard[player][i] == duchy) { score = score + 3; };
        if (state->discard[player][i] == province) { score = score + 6; };
        if (state->discard[player][i] == great_hall) { score = score + 1; };
        if (state->discard[player][i] == gardens) { score = score + (fullDeckCount(player, 0, state) / 10); };
    }
    
    
    for (i = 0; i < state->discardCount[player]; i++) {
        if (state->deck[player][i] == curse) { score = score - 1; };
        if (state->deck[player][i] == estate) { score = score + 1; };
        if (state->deck[player][i] == duchy) { score = score + 3; };
        if (state->deck[player][i] == province) { score = score + 6; };
        if (state->deck[player][i] == great_hall) { score = score + 1; };
        if (state->deck[player][i] == gardens) { score = score + (fullDeckCount(player, 0, state) / 10); };
    }
    
    return score;
}

int getWinners(int players[MAX_PLAYERS], struct gameState *state) {
    int i;
    int j;
    int highScore;
    int currentPlayer;
    
    
    for (i = 0; i < MAX_PLAYERS; i++) {
        
        if (i >= state->numPlayers) {
            players[i] = -9999;
        } else {
            players[i] = scoreFor(i, state);
        }
    }
    
    
    j = 0;
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (players[i] > players[j]) {
            j = i;
        }
    }
    highScore = players[j];
    
    
    currentPlayer = whoseTurn(state);
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (players[i] == highScore && i > currentPlayer) {
            players[i]++;
        }
    }
    
    
    j = 0;
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (players[i] > players[j]) {
            j = i;
        }
    }
    highScore = players[j];
    
    
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (players[i] == highScore) {
            players[i] = 1;
        } else {
            players[i] = 0;
        }
    }
    
    return 0;
}

int drawCard(int player, struct gameState *state) {
    int count;
    int deckCounter;
    if (state->deckCount[player] <= 0) {//Deck is empty
        
        
        int i;
        
        for (i = 0; i < state->discardCount[player]; i++) {
            state->deck[player][i] = state->discard[player][i];
            state->discard[player][i] = -1;
        }
        
        state->deckCount[player] = state->discardCount[player];
        state->discardCount[player] = 0;//Reset discard
        
        
        shuffle(player, state);//Shuffle the deck up and make it so that we can draw
        
        if (DEBUG) {//Debug statements
            printf("Deck count now: %d\n", state->deckCount[player]);
        }
        
        state->discardCount[player] = 0;
        
        
        count = state->handCount[player];//Get current player's hand count
        
        if (DEBUG) {//Debug statements
            printf("Current hand count: %d\n", count);
        }
        
        deckCounter = state->deckCount[player];//Create a holder for the deck count
        
        if (deckCounter == 0)
            return -1;
        
        state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to hand
        state->deckCount[player]--;
        state->handCount[player]++;//Increment hand count
    } else {
        int count = state->handCount[player];//Get current hand count for player
        int deckCounter;
        if (DEBUG) {//Debug statements
            printf("Current hand count: %d\n", count);
        }
        
        deckCounter = state->deckCount[player];//Create holder for the deck count
        state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to the hand
        state->deckCount[player]--;
        state->handCount[player]++;//Increment hand count
    }
    
    return 0;
}

int getCost(int cardNumber) {
    switch (cardNumber) {
        case curse:
            return 0;
        case estate:
            return 2;
        case duchy:
            return 5;
        case province:
            return 8;
        case copper:
            return 0;
        case silver:
            return 3;
        case gold:
            return 6;
        case adventurer:
            return 6;
        case council_room:
            return 5;
        case feast:
            return 4;
        case gardens:
            return 4;
        case mine:
            return 5;
        case remodel:
            return 4;
        case smithy:
            return 4;
        case village:
            return 3;
        case baron:
            return 4;
        case great_hall:
            return 3;
        case minion:
            return 5;
        case steward:
            return 3;
        case tribute:
            return 5;
        case ambassador:
            return 3;
        case cutpurse:
            return 4;
        case embargo:
            return 2;
        case outpost:
            return 5;
        case salvager:
            return 4;
        case sea_hag:
            return 4;
        case treasure_map:
            return 4;
    }
    
    return -1;
}

int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus) {
    int i;
    int j;
    int k;
    int x;
    int index;
    int currentPlayer = whoseTurn(state);
    int nextPlayer = currentPlayer + 1;
    
    int tributeRevealedCards[2] = {-1, -1};
    int temphand[MAX_HAND];// moved above the if statement
    //int drawntreasure = 0;
    //int z = 0;// this is the counter for the temp hand
    if (nextPlayer > (state->numPlayers - 1)) {
        nextPlayer = 0;
    }
    
    
   
    switch (card) {
        case adventurer:
            playAdventurer(state);
            return 0;
            
        case council_room:
            playCouncilRoom(handPos, state);
            return 0;
            
        case feast:
            
            for (i = 0; i <= state->handCount[currentPlayer]; i++) {
                temphand[i] = state->hand[currentPlayer][i];//Backup card
                state->hand[currentPlayer][i] = -1;//Set to nothing
            }
            
            updateCoins(currentPlayer, state, 5);
            x = 1;//Condition to loop on
            while (x == 1) {//Buy one card
                if (supplyCount(choice1, state) <= 0) {
                    if (DEBUG)
                        printf("None of that card left, sorry!\n");
                    
                    if (DEBUG) {
                        printf("Cards Left: %d\n", supplyCount(choice1, state));
                    }
                } else if (state->coins < getCost(choice1)) {
                    printf("That card is too expensive!\n");
                    
                    if (DEBUG) {
                        printf("Coins: %d < %d\n", state->coins, getCost(choice1));
                    }
                } else {
                    
                    if (DEBUG) {
                        printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] +
                               state->discardCount[currentPlayer]);
                    }
                    
                    gainCard(choice1, state, 0, currentPlayer);//Gain the card
                    x = 0;//No more buying cards
                    
                    if (DEBUG) {
                        printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] +
                               state->discardCount[currentPlayer]);
                    }
                    
                }
            }
            
            
            for (i = 0; i <= state->handCount[currentPlayer]; i++) {
                state->hand[currentPlayer][i] = temphand[i];
                temphand[i] = -1;
            }
            
            
            return 0;
            
        case gardens:
            return -1;
            
        case mine:
            j = state->hand[currentPlayer][choice1];  //store card we will trash
            
            if (state->hand[currentPlayer][choice1] < copper || state->hand[currentPlayer][choice1] > gold) {
                return -1;
            }
            
            if (choice2 > treasure_map || choice2 < curse) {
                return -1;
            }
            
            if ((getCost(state->hand[currentPlayer][choice1]) + 3) > getCost(choice2)) {
                return -1;
            }
            
            gainCard(choice2, state, 2, currentPlayer);
            
            
            discardCard(handPos, currentPlayer, state, 0);
            
            
            for (i = 0; i < state->handCount[currentPlayer]; i++) {
                if (state->hand[currentPlayer][i] == j) {
                    discardCard(i, currentPlayer, state, 0);
                    break;
                }
            }
            
            return 0;
            
        case remodel:
            j = state->hand[currentPlayer][choice1];  //store card we will trash
            
            if ((getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(choice2)) {
                return -1;
            }
            
            gainCard(choice2, state, 0, currentPlayer);
            
            
            discardCard(handPos, currentPlayer, state, 0);
            
            
            for (i = 0; i < state->handCount[currentPlayer]; i++) {
                if (state->hand[currentPlayer][i] == j) {
                    discardCard(i, currentPlayer, state, 0);
                    break;
                }
            }
            
            
            return 0;
            
        case smithy:
            playSmithy(handPos, state);
            return 0;
            
        case village:
            playVillage(handPos, state);
            return 0;
            
        case baron:
            state->numBuys++;//Increase buys by 1!
            if (choice1 > 0) {//Boolean true or going to discard an estate
                int p = 0;//Iterator for hand!
                int card_not_discarded = 1;//Flag for discard set!
                while (card_not_discarded) {
                    if (state->hand[currentPlayer][p] == estate) {//Found an estate card!
                        state->coins += 4;//Add 4 coins to the amount of coins
                        state->discard[currentPlayer][state->discardCount[currentPlayer]] = state->hand[currentPlayer][p];
                        state->discardCount[currentPlayer]++;
                        for (; p < state->handCount[currentPlayer]; p++) {
                            state->hand[currentPlayer][p] = state->hand[currentPlayer][p + 1];
                        }
                        state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
                        state->handCount[currentPlayer]--;
                        card_not_discarded = 0;//Exit the loop
                    } else if (p > state->handCount[currentPlayer]) {
                        if (DEBUG) {
                            printf("No estate cards in your hand, invalid choice\n");
                            printf("Must gain an estate if there are any\n");
                        }
                        if (supplyCount(estate, state) > 0) {
                            gainCard(estate, state, 0, currentPlayer);
                            state->supplyCount[estate]--;//Decrement estates
                            if (supplyCount(estate, state) == 0) {
                                isGameOver(state);
                            }
                        }
                        card_not_discarded = 0;//Exit the loop
                    } else {
                        p++;//Next card
                    }
                }
            } else {
                if (supplyCount(estate, state) > 0) {
                    gainCard(estate, state, 0, currentPlayer);//Gain an estate
                    state->supplyCount[estate]--;//Decrement Estates
                    if (supplyCount(estate, state) == 0) {
                        isGameOver(state);
                    }
                }
            }
            
            
            return 0;
            
        case great_hall:
            playGreatHall(handPos, state);
            return 0;
            
        case minion:
            
            state->numActions++;
            
            
            discardCard(handPos, currentPlayer, state, 0);
            
            if (choice1)        //+2 coins
            {
                state->coins = state->coins + 2;
            } else if (choice2)        //discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
            {
                
                while (numHandCards(state) > 0) {
                    discardCard(handPos, currentPlayer, state, 0);
                }
                
                
                for (i = 0; i < 4; i++) {
                    drawCard(currentPlayer, state);
                }
                
                
                for (i = 0; i < state->numPlayers; i++) {
                    if (i != currentPlayer) {
                        if (state->handCount[i] > 4) {
                            
                            while (state->handCount[i] > 0) {
                                discardCard(handPos, i, state, 0);
                            }
                            
                            
                            for (j = 0; j < 4; j++) {
                                drawCard(i, state);
                            }
                        }
                    }
                }
                
            }
            return 0;
            
        case steward:
            if (choice1 == 1) {
                
                drawCard(currentPlayer, state);
                drawCard(currentPlayer, state);
            } else if (choice1 == 2) {
                
                state->coins = state->coins + 2;
            } else {
                
                discardCard(choice2, currentPlayer, state, 1);
                discardCard(choice3, currentPlayer, state, 1);
            }
            
            
            discardCard(handPos, currentPlayer, state, 0);
            return 0;
            
        case tribute:
            if ((state->discardCount[nextPlayer] + state->deckCount[nextPlayer]) <= 1) {
                if (state->deckCount[nextPlayer] > 0) {
                    tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
                    state->deckCount[nextPlayer]--;
                } else if (state->discardCount[nextPlayer] > 0) {
                    tributeRevealedCards[0] = state->discard[nextPlayer][state->discardCount[nextPlayer] - 1];
                    state->discardCount[nextPlayer]--;
                } else {
                    
                    if (DEBUG) {
                        printf("No cards to reveal\n");
                    }
                }
            } else {
                if (state->deckCount[nextPlayer] == 0) {
                    for (i = 0; i < state->discardCount[nextPlayer]; i++) {
                        state->deck[nextPlayer][i] = state->discard[nextPlayer][i];//Move to deck
                        state->deckCount[nextPlayer]++;
                        state->discard[nextPlayer][i] = -1;
                        state->discardCount[nextPlayer]--;
                    }
                    
                    shuffle(nextPlayer, state);//Shuffle the deck
                }
                tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
                state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
                state->deckCount[nextPlayer]--;
                tributeRevealedCards[1] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
                state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
                state->deckCount[nextPlayer]--;
            }
            
            if (tributeRevealedCards[0] == tributeRevealedCards[1]) {//If we have a duplicate card, just drop one
                state->playedCards[state->playedCardCount] = tributeRevealedCards[1];
                state->playedCardCount++;
                tributeRevealedCards[1] = -1;
            }
            
            for (i = 0; i <= 2; i++) {
                if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver ||
                    tributeRevealedCards[i] == gold) {//Treasure cards
                    state->coins += 2;
                } else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy ||
                           tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens ||
                           tributeRevealedCards[i] == great_hall) {//Victory Card Found
                    drawCard(currentPlayer, state);
                    drawCard(currentPlayer, state);
                } else {//Action Card
                    state->numActions = state->numActions + 2;
                }
            }
            
            return 0;
            
        case ambassador:
            j = 0;        //used to check if player has enough cards to discard
            
            if (choice2 > 2 || choice2 < 0) {
                return -1;
            }
            
            if (choice1 == handPos) {
                return -1;
            }
            
            for (i = 0; i < state->handCount[currentPlayer]; i++) {
                if (i != handPos && i == state->hand[currentPlayer][choice1] && i != choice1) {
                    j++;
                }
            }
            if (j < choice2) {
                return -1;
            }
            
            if (DEBUG)
                printf("Player %d reveals card number: %d\n", currentPlayer, state->hand[currentPlayer][choice1]);
            
            
            state->supplyCount[state->hand[currentPlayer][choice1]] += choice2;
            
            
            for (i = 0; i < state->numPlayers; i++) {
                if (i != currentPlayer) {
                    gainCard(state->hand[currentPlayer][choice1], state, 0, i);
                }
            }
            
            
            discardCard(handPos, currentPlayer, state, 0);
            
            
            for (j = 0; j < choice2; j++) {
                for (i = 0; i < state->handCount[currentPlayer]; i++) {
                    if (state->hand[currentPlayer][i] == state->hand[currentPlayer][choice1]) {
                        discardCard(i, currentPlayer, state, 1);
                        break;
                    }
                }
            }
            
            return 0;
            
        case cutpurse:
            
            updateCoins(currentPlayer, state, 2);
            for (i = 0; i < state->numPlayers; i++) {
                if (i != currentPlayer) {
                    for (j = 0; j < state->handCount[i]; j++) {
                        if (state->hand[i][j] == copper) {
                            discardCard(j, i, state, 0);
                            break;
                        }
                        if (j == state->handCount[i]) {
                            for (k = 0; k < state->handCount[i]; k++) {
                                if (DEBUG)
                                    printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
                            }
                            break;
                        }
                    }
                    
                }
                
            }
            
            
            discardCard(handPos, currentPlayer, state, 0);
            
            return 0;
            
            
        case embargo:
            
            state->coins = state->coins + 2;
            
            
            if (state->supplyCount[choice1] == -1) {
                return -1;
            }
            
            
            state->embargoTokens[choice1]++;
            
            
            discardCard(handPos, currentPlayer, state, 1);
            return 0;
            
        case outpost:
            
            state->outpostPlayed++;
            
            
            discardCard(handPos, currentPlayer, state, 0);
            return 0;
            
        case salvager:
            
            state->numBuys++;
            
            if (choice1) {
                
                state->coins = state->coins + getCost(handCard(choice1, state));
                
                discardCard(choice1, currentPlayer, state, 1);
            }
            
            
            discardCard(handPos, currentPlayer, state, 0);
            return 0;
            
        case sea_hag:
            for (i = 0; i < state->numPlayers; i++) {
                if (i != currentPlayer) {
                    state->discard[i][state->discardCount[i]] = state->deck[i][state->deckCount[i]--];
                    state->deckCount[i]--;
                    state->discardCount[i]++;
                    state->deck[i][state->deckCount[i]--] = curse;//Top card now a curse
                }
            }
            return 0;
            
        case treasure_map:
            
            index = -1;
            for (i = 0; i < state->handCount[currentPlayer]; i++) {
                if (state->hand[currentPlayer][i] == treasure_map && i != handPos) {
                    index = i;
                    break;
                }
            }
            if (index > -1) {
                
                discardCard(handPos, currentPlayer, state, 1);
                discardCard(index, currentPlayer, state, 1);
                
                
                for (i = 0; i < 4; i++) {
                    gainCard(gold, state, 1, currentPlayer);
                }
                
                
                return 1;
            }
            
            
            return -1;
    }
    
    return -1;
}

int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag) {
    
    
    if (trashFlag < 1) {
        
        state->playedCards[state->playedCardCount] = state->hand[currentPlayer][handPos];
        state->playedCardCount++;
    }
    
    
    state->hand[currentPlayer][handPos] = -1;
    
    
    if (handPos == (state->handCount[currentPlayer] - 1))    //last card in hand array is played
    {
        
        state->handCount[currentPlayer]--;
    } else if (state->handCount[currentPlayer] == 1) //only one card in hand
    {
        
        state->handCount[currentPlayer]--;
    } else {
        
        state->hand[currentPlayer][handPos] = state->hand[currentPlayer][(state->handCount[currentPlayer] - 1)];
        
        state->hand[currentPlayer][state->handCount[currentPlayer] - 1] = -1;
        
        state->handCount[currentPlayer]--;
    }
    
    return 0;
}

int gainCard(int supplyPos, struct gameState *state, int toFlag, int player) {
    
    if (supplyCount(supplyPos, state) < 1) {
        return -1;
    }
    
    
    
    if (toFlag == 1) {
        state->deck[player][state->deckCount[player]] = supplyPos;
        state->deckCount[player]++;
    } else if (toFlag == 2) {
        state->hand[player][state->handCount[player]] = supplyPos;
        state->handCount[player]++;
    } else {
        state->discard[player][state->discardCount[player]] = supplyPos;
        state->discardCount[player]++;
    }
    
    
    state->supplyCount[supplyPos]--;
    
    return 0;
}

int updateCoins(int player, struct gameState *state, int bonus) {
    int i;
    
    
    state->coins = 0;
    
    
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] == copper) {
            state->coins += 1;
        } else if (state->hand[player][i] == silver) {
            state->coins += 2;
        } else if (state->hand[player][i] == gold) {
            state->coins += 3;
        }
    }
    
    
    state->coins += bonus;
    
    return 0;
}

void playAdventurer(struct gameState *state) {
	int z = 0;
	int drawntreasure=0;
	int currentPlayer = whoseTurn(state);
	int temphand[MAX_HAND]; 
   while (drawntreasure < 2) {
        
        if (state->deckCount[currentPlayer] < 1)
            shuffle(currentPlayer, state);
        
        drawCard(currentPlayer, state);
        
        
        int cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer] - 1];
        
        if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
            drawntreasure++;
        else {
            temphand[z] = cardDrawn;
            
            state->handCount[currentPlayer]--;
            z++;
        }
    }
    while (z-- >= 0) {
        
        state->discard[currentPlayer][state->discardCount[currentPlayer]++] = temphand[z - 1];
        z = z - 1;
    }
}

void playSmithy(int handPos,  struct gameState *state) {
    
    int i;
	int currentPlayer = whoseTurn(state);
    for (i = 0; i <= 3; i++) {
        drawCard(currentPlayer, state);
    }
    
    
    discardCard(handPos, currentPlayer, state, 0);
}

void playVillage(int handPos, struct gameState *state) {
    int currentPlayer = whoseTurn(state);
    drawCard(currentPlayer, state);
    
    
    state->numActions = state->numActions + 22;
    
    
    discardCard(handPos, currentPlayer, state, 0);
}

void playGreatHall(int handPos, struct gameState *state) {
    int currentPlayer = whoseTurn(state);
    drawCard(currentPlayer, state);
    
    
    state->numActions++;
    
    
    discardCard(handPos, currentPlayer, state, 0);
}

void playCouncilRoom(int handPos, struct gameState *state) {
    int i;
    int currentPlayer = whoseTurn(state);
    
    for (i = 0; i < 4; i++)
        drawCard(currentPlayer, state);
    
    
    state->numBuys++;
    
    
    for (i = 0; i < state->numPlayers; i++) {
        if (i != currentPlayer)
            drawCard(i, state);
    }
    
    
    discardCard(handPos, currentPlayer, state, 0);
}


