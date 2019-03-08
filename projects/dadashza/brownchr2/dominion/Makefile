CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittestresults.out runAdventurer runTestCard1 runTestCard2

adventurer:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c randomtestadventurer.c rngs.c -o randomtestadventurer -lm -g

runAdventurer: adventurer
	./randomtestadventurer &> randomtestadventurer.out
	gcov dominion.c -f -b >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

randomtestcard1:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c randomtestcard1.c rngs.c -o randomtestcard1 -lm -g

runTestCard1: randomtestcard1
	./randomtestcard1 &> randomtestcard1.out
	gcov dominion.c -f -b >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out

randomtestcard2:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c randomtestcard2.c rngs.c -o randomtestcard2 -lm -g

runTestCard2: randomtestcard2
	./randomtestcard2 &> randomtestcard2.out
	gcov dominion.c -f -b >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out

unittestresults.out:    
	gcc -o cardtest1 cardtest1.c dominion.c rngs.c $(CFLAGS)
        
	./cardtest1 >> unittestresults.out
	gcov cardtest1.c >> unittestresults.out
	gcov -f -b dominion.c >> unittestresults.out
                
        
	gcc -o cardtest2 cardtest2.c -g dominion.c rngs.c -g $(CFLAGS)
        
	./cardtest2 >> unittestresults.out
	gcov cardtest2.c >> unittestresults.out
	gcov dominion.c -f -b >> unittestresults.out
        
	gcc -o cardtest3 cardtest3.c -g dominion.c rngs.c -g $(CFLAGS)
        
	./cardtest3 >> unittestresults.out
	gcov cardtest3.c >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out
        
	gcc -o cardtest4 cardtest4.c -g dominion.c rngs.c -g $(CFLAGS)  
        
	./cardtest4 >> unittestresults.out
	gcov cardtest4.c >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out 
        
	gcc -o unittest1 unittest1.c -g dominion.c rngs.c -g $(CFLAGS)
        
	./unittest1 >> unittestresults.out
	gcov unittest1.c >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out 
        
	gcc -o unittest2 unittest2.c -g dominion.c rngs.c -g $(CFLAGS)
        
	./unittest2 >> unittestresults.out
	gcov unittest2.c >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out 
        
	gcc -o unittest3 unittest3.c -g dominion.c rngs.c -g $(CFLAGS)
        
	./unittest3 >> unittestresults.out
	gcov unittest3.c >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out 
       
	gcc -o unittest4 unittest4.c -g dominion.c rngs.c -g $(CFLAGS)
        
	./unittest4 >> unittestresults.out
	gcov unittest4.c >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittestresults.out cardtest1 cardtest2 cardtest3 cardtest4 unittest1 unittest2 unittest3 unittest4 randomtestadventurer randomtestcard1 randomtestcard2 randomtestcard1.out randomtestcard2.out randomtestadventurer.out

