#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXTIMES 10

int divisiontest(int divisor1, int quotient1);

int main(void)
{
	int times, correct, divisor, quotient;
	int correctnumber = 0;
	srand((int) time (NULL));
	//Run division test 10 times
	for(times = 0; times < MAXTIMES; times++)
	{
		//Generate divisor and quotient
		divisor = rand()%12+1;
		quotient = rand()%13;
		//Perform division, check if user got correct
		correct = divisiontest(divisor, quotient);
		correctnumber += correct;
	}
	printf("You got %d0%% correct.\n",correctnumber);
	return 0;
}

//Check if user got the divison right
int divisiontest(int divisor1, int quotient1)
{
	int divisor = divisor1;
	int quotient = quotient1;
	int dividend = quotient * divisor;
	int userresult;
	int numbertoreturn;
	printf("What is %d divided by %d?\n", dividend, divisor);
	scanf("%d",&userresult);
	numbertoreturn = ((userresult == quotient) ? 1 : 0);
	return numbertoreturn;
}
