#include<stdio.h>
#include<fcntl.h>

int rerollCount = 0;

//variables to hold scores
unsigned char ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixes = 0;
unsigned char bonus = 0, score = 0, threeKind = 0, fourKind = 0, small = 0;
unsigned char large = 0, full = 0, yahtzee = 0, chance = 0;

int onesSelect = 0, twosSelect = 0, threesSelect = 0, foursSelect = 0, fivesSelect = 0, sixesSelect = 0;
int threeKindSelect = 0, fourKindSelect = 0, smallSelect = 0, largeSelect = 0, fullSelect = 0, chanceSelect = 0, yahtzeeSelect = 0;
int upperSelect = 0, lowerSelect = 0, over = 0;

//array for holding dice
unsigned char dice[5];

//returns roll number from dice_driver
unsigned char getDiceRoll()
{
	unsigned char diceNum;
	//open dice_driver in read only mode
	int x = open("/dev/dice", O_RDONLY);
	int y = read(x, &diceNum, sizeof(char));
	return diceNum;
}

void reroll()
{
	int rerollNum = 0;
	int diceNum = 0;
	int x = 0;
	unsigned char reroll = getDiceRoll();
	rerollCount++;


		printf("How many dice do you want to reroll? \n");
		scanf("%d", &rerollNum);
		printf("Which dice do you wish to reroll? \n");

		while(x < rerollNum)
		{
			scanf("%d", &diceNum);
			
			switch(diceNum)
			{
				case 1:
				dice[0] = getDiceRoll();
				break;

				case 2:
                		dice[1] = getDiceRoll();
                		break;

				case 3:
                		dice[2] = getDiceRoll();
                		break;

                		case 4:
                		dice[3] = getDiceRoll();
                		break;

                		case 5:
                		dice[4] = getDiceRoll();
                		break;

				default:
				break;
	
			}
		x++;
	}
}

//compare function
unsigned char compare(const void *a, const void *b)
{
	return (*(unsigned char*)a - *(unsigned char*)b);
}


void displayScores()
{
        printf("Your score so far is: %d \n", score);
        printf("Ones:   %d               Fours: %d \n", ones, fours);
        printf("Twos:   %d               Fives: %d \n", twos, fives);
        printf("Threes: %d               Sixes: %d \n", threes, sixes);
        printf("Upper section bonus: %d \n", bonus);
        printf("Three of a kind: %d      Four of a kind: %d \n", threeKind, fourKind);
	printf("Small Straight:  %d      Large Straight: %d \n", small, large);
	printf("Full House:      %d      Yahtzee:        %d \n", full, yahtzee);
	printf("Chance:		 %d \n", chance);


}

//1 valid 0 invalid
int upperValid()
{
	switch(upperSelect)
	{
		case 1:
		if(onesSelect == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;

		case 2:
		if(twosSelect == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;

		case 3:
		if(threesSelect == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;
	
		case 4:	
		if(foursSelect == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;

		case 5:
                if(fivesSelect == 1)
		{
                        return 0;
		}
                else
                {
                        return 1;
                }
                break;

		case 6:
                if(sixesSelect == 1)
		{
                        return 0;
		}
                else
                {
                        return 1;
                }
                break;

		default:
		break;

	}

}

int lowerValid()
{
	switch(lowerSelect)
        {
                case 1:
                if(threeKindSelect == 1)
                {
                        return 0;
                }
                else
                {
                        return 1;
                }
                break;

                case 2:
                if(fourKindSelect == 1)
                {
                        return 0;
                }
                else
                {
                        return 1;
                }
                break;

                case 3:
                if(fullSelect == 1)
                {
                        return 0;
                }
                else
                {
                        return 1;
                }
                break;

		case 4:
                if(smallSelect == 1)
                {
                        return 0;
                }
                else
                {
                        return 1;
                }
                break;

                case 5:
                if(largeSelect == 1)
                {
                        return 0;
                }
                else
                {
                        return 1;
                }
                break;

                case 6:
                if(yahtzeeSelect == 1)
                {
                        return 0;
                }
                else
                {
                        return 1;
                }
                break;

		case 7:
		if(chanceSelect == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;

		default:
		break;
	}
}



int main()
{	
	//0 for false 1 for true
	int rollAgain = 0;

	int selection = 0;
	

	while(1)
	{
		int i = 0;	
		for(i = 0; i < 5; i ++)
		{
			dice[i] = getDiceRoll();
		}

		printf("Your roll: \n");
		for(i = 0; i < 5; i ++)
		{
			printf("%d ", dice[i]);
		}
	

		while(rerollCount < 2)
		{
			printf("\nDo you wish to reroll any of the dice? 0 = no  1 = yes: \n");
			scanf("%d", &rollAgain);

			if(rollAgain == 1)
			{
				reroll();
				printf("Your new roll: ");
				for(i = 0; i < 5; i ++)
				{
					printf("%d ", dice[i]);
				}

			}

			else
			{
				break;
			}
		}

		//sort dice
		qsort(dice, 5, sizeof(char), compare);

		//Assign and score current roll
		printf("\nPlace die into: \n");
		printf("1) Upper Section \n");
		printf("2) Lower Section \n");

		printf("Selection? ");
		scanf("%d", &selection);

		if(selection == 1)
		{
			printf("Place dice into: \n");
			printf("1) Ones \n");
			printf("2) Twos \n");
			printf("3) Threes \n");
			printf("4) Fours \n");
			printf("5) Fives \n");
			printf("6) Sixes \n");
		
			do
			{
				printf("Select an available option: ");
				scanf("%d", &upperSelect);
			}
			while(upperValid() == 0);


			int i = 0;
			switch(upperSelect)
			{
				case 1:
				while(i < 5)
				{
					if(dice[i] == 1)
					{
						ones = ones + dice[i];
						onesSelect = 1;
					}
					i++;
				}
				score = score + ones; 
				over++;
				break;


				case 2:
                        	while(i < 5)
                        	{
                                	if(dice[i] == 2)
                                	{
                                        	twos = twos + dice[i];
						twosSelect = 1;
                                	}
                                	i++;
                        	}
				score = score + twos;
				over++;
                        	break;

			
				case 3:
                        	while(i < 5)
                        	{
                                	if(dice[i] == 3)
                                	{
                                        	threes = threes + dice[i];
						threesSelect = 1;
                                	}
                                	i++;
                        	}
				score = score + threes;
				over++;
                        	break;


				case 4:
                        	while(i < 5)
                        	{
                                	if(dice[i] == 4)
                                	{
                                        	fours = fours + dice[i];
						foursSelect = 1;
                                	}
                                	i++;
                        	}
				score = score + fours;
				over++;
                        	break;


				case 5:
                        	while(i < 5)
                        	{
                                	if(dice[i] == 5)
                                	{
                                        	fives = fives + dice[i];
						fivesSelect = 1;
                                	}
                                	i++;
                        	}
				score = score + fives;
				over++;
                        	break;

				case 6:
                        	while(i < 5)
                        	{
                                	if(dice[i] == 6)
                                	{
                                        	sixes = sixes + dice[i];
						sixesSelect = 1;
                                	}
                                	i++;
                        	}
				score = score + sixes;
				over++;
                        	break;


				default:
				break;
			}

			if(score >= 63)
				bonus = 35;

			score = score + bonus;
		}

		if(selection == 2)
		{
			printf("1) Three of a kind \n");
			printf("2) Four of a kind \n");
			printf("3) Full House \n");
			printf("4) Small Straight \n");
			printf("5) Large Straight \n");
			printf("6) Yahtzee \n");
			printf("7) Chance \n");

			do
                        {
                                printf("Select an available option: ");
                                scanf("%d", &lowerSelect);
                        }
			while(lowerValid() == 0);

			int sameThree = 0;
			int sameFour = 0;
			int y = 0;
			int i = 0;
			int j = 0;

			
			switch(lowerSelect)
			{
				//3 of a kind
				case 1:
				for(i = 0; i < 5; i++)
				{
					for(j = 0; j < 5; j++)
					{
						if(dice[i] == dice[j])
						{
							sameThree++;
							break;
						}
					}
					if(sameThree == 3)
						break;
				}

				threeKind = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
				score = score + threeKind;
				threeKindSelect = 1;
				over++;
				break;


				//4 of a kind
				case 2:
				for(i = 0; i < 5; i++)
                                {
                                        for(j = 0; j < 5; j++)
                                        {
                                                if(dice[i] == dice[j])
                                                {
                                                        sameFour++;
                                                        break;
                                                }
                                        }
                                        if(sameFour == 4)
                                                break;

                                }

				
				fourKind = dice[0]+dice[1]+dice[2]+dice[3]+dice[4];
				score = score + fourKind;
				fourKindSelect = 1;
				over++;
				break;


				//full house
				case 3:
				//dice are sorted so need to see if there are three that are the same and 2 that are the same
				//checks to see if first three dice are the same
				if(dice[0] == dice[1] == dice[2])
				{
					//now checks to see if last 2 are the same
					if(dice[3] == dice[4])
					{
						full = full + 25;
					}
				}

				//checks to see if last three are the same
				if(dice[2] == dice[3] == dice[4])
				{
					//now check if first two are the same
					if(dice[0] == dice[1])
					{
						full = full + 25;
					}
				}

				score = score + full;
				fullSelect = 1;
				over++;
				break;


				//small straight
				case 4:
				//check to see if first four dice are sequential
				if((dice[3] == (1 + dice[2])) && (dice[2] == (1 + dice[1])) && (dice[1] == (1 + dice[0])))
					small = small + 30;

				//check to see if last four are sequential
				if((dice[4] == (1 + dice[3])) && (dice[3] == (1 + dice[2])) && (dice[2] == (1 + dice[1])))
                                        small = small + 30;


				score = score + small;
				smallSelect = 1;
				over++;
				break;


				//large straight
                                case 5:
                                //check to see if all dice are sequential
                                if((dice[4] == (1 + dice[3])) && (dice[3] == (1 + dice[2])) && (dice[2] == (1 + dice[1])) && (dice[1] == (1 + dice[0])))
                                        large = large + 40;


                                score = score + large;
				largeSelect = 1;
                                break;

	
				//yahtzee
				case 6:
				if(dice[0] == dice[1] == dice[2] == dice[3] == dice[4])
					yahtzee = yahtzee + 50;
				score = score + yahtzee;
				yahtzeeSelect = 1;
				over++;
				break;


				//chance
				case 7:
				chance = dice[0] + dice[1] +dice[2] + dice[3] + dice[4];
				score = score + chance;
				chanceSelect = 1;
				over++;
				break;

				default:
				break;
			}
		}

		displayScores();
		rerollCount = 0;
		if(over == 13)
			break;
	}
	return 0;
}
