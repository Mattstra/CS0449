#include<stdio.h>
#include<time.h>
#include<stdlib.h>

//function to get cards based on probability
int biasNum(int x);

//functions to count aces for player and dealer
int countPaces(int q);
int countDaces(int w);

//function to adjust ace from 11 to 1 if over 21
//a is the total and b is the number of aces
int adjustAce(int a, int b);

//keep track of aces for player and dealer
int paces = 0;
int daces = 0;


int main()
{
	//declare string array for input
	char choice[6];

	//seed random number generator
	srand((unsigned int)time(NULL));

	//generate random numbers between 1 and 11 for dealer
	int d1 = rand() %(11)+1;
	int d2 = rand() %(11)+1;

	//generate random numbers between 1 and 11 for player
	int p1 = rand() % (11)+1;
	int p2 = rand() % (11)+1;
	
	//use random number to get random card and count aces
	d1 = biasNum(d1);
	daces = countDaces(d1);
	d2 = biasNum(d2); 
	daces = countDaces(d2);
	p1 = biasNum(p1);
	paces = countPaces(p1);
	p2 = biasNum(p2);
	paces = countPaces(p2);
	
	int dtotal = d1 + d2;
	int ptotal = p1 + p2;

	//if having an 11 instead of a 1 will cause a score of 21 then change ace
	if((ptotal + 10) == 21)
	{
		if(p1 == 1)
		{
			p1 = 11;
		}

		if(p2 == 1)
		{
			p2 = 11;
		}
	}

	
        if((dtotal + 10) == 21)
        {
                if(d1 == 1)
                {
                        d1 = 11;
                }

                if(d2 == 1)
                {
                        d2 = 11;
                }
        }



	//If either player or dealer are over 21 at this point then adjust aces
	if(ptotal > 21)
	{
		ptotal = adjustAce(ptotal, paces);
		paces = 0;
	}

	if(dtotal < 21)
	{
		dtotal = adjustAce(dtotal, daces);
		daces = 0;
	}
	
	//declare comparison variable for loop
	int compare = 1;
	int compare2 = 1;
	
	printf("The dealer: \n");
	printf("? + %d \n",d2);
	printf("You: \n");
	printf("%d + %d = %d \n", p1, p2, ptotal);


	 //if total is 21 for player or dealer at this point then compare
        if((ptotal == 21) || (dtotal == 21))
        {
                 if(ptotal > dtotal)
                        {
                                printf(" Winner!!! \n");
                                printf("You win. Dealer loses.");
				goto exit;
                        }

                        if(dtotal > ptotal)
                        {
                                printf(" Loser!!! Dealer has %d \n", dtotal);
                                printf("You lost. Dealer wins.");
				goto exit;
                        }


                        if(ptotal == dtotal)
                        {
                                printf("\n");
                                printf("The totals are equal. Nobody wins.");
				goto exit;
                        }

        }


	//user input
	printf("Would you like to 'hit' or 'stand'?");
	scanf("%s", &choice); 
	
	//use strcmp to compare input for loop conditions
	compare = strcmp(choice, "hit");
	compare2 = strcmp(choice, "stand");

	//for when user chooses to hit
	if(compare == 0)
	{
		p1 = ptotal;
		p2 = rand()%(11)+1;
		p2 = biasNum(p2);
		paces = countPaces(p2);
		ptotal = p1 + p2;

		if(ptotal > 21)
		{
			//first see if there were any aces and adjust
			ptotal = adjustAce(ptotal, paces);
		}


		printf("The dealer: \n");
		printf("? + %d \n", d2);
		printf("You: \n");
		printf("%d + %d = %d", p1, p2, ptotal);

		if(ptotal > 21)
		{
			printf("Busted! \n");
			printf("You busted. Dealer wins");
		}

		//when the player total isn't over 21, see if it is higher than
		//the dealer's total
		else
		{
			if(ptotal > dtotal)
			{
				printf(" Winner!!! \n");
				printf("You win. Dealer loses.");
			}

			if(dtotal > ptotal)
			{
				printf(" Loser!!! \n");
				printf("You lost. Dealer wins.");
			}

			
                        if(ptotal == dtotal)
                        {
                                printf("\n");
                                printf("The totals are equal. Nobody wins.");
                        }

		}
	}


	//when player chooses to stand the dealer must hit if their total is
	//less than 17
	if(compare2 == 0)
	{
		if(dtotal < 17)
		{
			d1 = dtotal;
			d2 = rand()%(11)+1;
			d2 = biasNum(d2);
			daces = countDaces(d2);
			dtotal = d1 + d2;

			if(dtotal > 21)
			{
				dtotal = adjustAce(dtotal, daces);
			}

			printf("The dealer: \n");
			printf("? + %d \n", d1);
			printf("You: \n");
			printf("%d + %d = %d", p1, p2, ptotal);

			if(dtotal > 21)
			{
				printf("\n");
				printf("Dealer busted. You win!");	
			}

			else
			{
				if(ptotal > dtotal)
                        	{
                                	printf(" Winner!!! \n");
                                	printf("You win. Dealer loses.");
                        	}

                        	if(dtotal > ptotal)
                        	{
                                	printf(" Loser!!! \n");
                                	printf("You lost. Dealer wins.");
                      		}

	                        if(ptotal == dtotal)
        	                {
                	                printf("\n");
                        	        printf("The totals are equal. Nobody wins.");
                       		}
			}

		}

		//dealer has 17 or greater
		else
		{
                        printf("The dealer: \n");
                        printf("? + %d \n", d1);
                        printf("You: \n");
                        printf("%d + %d = %d", p1, p2, ptotal);

			 if(ptotal > dtotal)
                        {
                                printf(" Winner!!! \n");
                                printf("You win. Dealer loses.");
                        }

                        if(dtotal > ptotal)
                        {
                                printf(" Loser!!! \n");
                                printf("You lost. Dealer wins.");
                        }

			if(ptotal == dtotal)
			{
				printf("\n");
				printf("The totals are equal. Nobody wins.");
			}
		}
	}


exit:	return 0;
}


int biasNum(int x)
{
	//random number between 0 and 1 to use as a probability
	double bias = (double) rand() / (double)RAND_MAX;

	//probability of getting an ace is 1/13 (0.076)
	if(bias <= 0.076)
	{
		return 11;
	}

	else
	{
		//probability of a card worth 10 pts is 16/52 (0.307)
		if(bias <= 0.307)
		{
			return 10;
		}
		
		else
		{
			//remaining probability is for cards 2 - 9
			return x;
		}
	}
}

int countPaces(int q)
{
	if((q ==11) || (q == 1))
	{
		paces++;
		return paces;
	}

	else
	{
		return paces;
	}
}

int countDaces(int w)
{
	if((w ==11) || (w == 1))
        {
                daces++;
                return daces;
        }

        else
        {
                return daces;
        }
}

int adjustAce(int a, int b)
{
	a = (a - (b * 11)) + b;
	return a;
}

