//Lyman Shen 1/19/17

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

const int maxsize = 10;
char names [maxsize][20];
int groupsize [maxsize];
int currentsize = 0;

void insert(void);
void removeRes(void);
void list(void);

int main(void)
{
	//initialize
	int i;
	int option;
	for(i=0;i<maxsize;i++)
	{
		groupsize[i] = 0;
		names[i][0] = '\0';
	}
	printf("Welcome to my Surf Lessons.\n");
	//Main loop
	while(1)
	{
		//Choose function to go to from int input
		printf("To add to wait list, type 1. To remove a  waitlist, type 2. To show the wait list, type 3. To quit, type 0.\n");
		scanf("%d",&option);
		switch(option)
		{
			case 1:
				//insert waitlist
				insert();
				break;
			case 2:
				//remove waitlist
				removeRes();
				break;
			case 3:
				//show waitlist
				list();
				break;
			case 0:
				//end program
				return 0;
			default:
				printf("Please enter 1, 2, 3, or 0.\n");
		}
	}
}

//Insert into waitlist
void insert(void)
{
	//Only insert if slots are open
	if(currentsize < maxsize)
	{
		char name [20];
		int group;
		int i = 0;
		//Enter names
		printf("What is the reservation name under?\n");
		scanf("%s",name);
		//Check for used names, exit if used
		while(i<currentsize)
		{
			if(strcmp(name,names[i])!=0)
			{
				i++;
			}
			else
			{
				printf("The name is taken.\n");
				return;
			}
		}
		//Enter group size
		printf("What is size of the reservation group?\n");
		scanf("%d",&group);
		while(group < 1)
		{
			printf("Use a valid size for reservation group.\n");
			scanf("%d",&group);
		}
		//Put name, size at end of list, increment counter
		strcpy(names[currentsize],name);
		groupsize[currentsize] = group;
		currentsize++;
		printf("You have successfully put yourself on the wait list.\n");
	}
	else
		//Notify full wait list
		printf("The waitlist is full. Try again later.\n");
	return;
}

//Remove waitlist with opening slots
void removeRes(void)
{
	int i;
	int j;
	int slotsleft;
	//Enter opening size
	printf("How big is the opening size?\n");
	scanf("%d",&slotsleft);
	//Check for each wait list in order
	for(i=0;i<currentsize;i++)
	{
		//Remove if enough slots for group
		if(slotsleft >= groupsize[i])
		{
			printf("%s with groupsize %d has been successfully placed on a reservation and is now removed from waitlist.\n",names[i],groupsize[i]);
			//Update slots left, shift other slots up
			slotsleft -= groupsize[i];
			for(j=i+1;j<currentsize;j++)
			{
				groupsize[j-1] = groupsize[j];
				strcpy(names[j-1],names[j]);
			}
			groupsize[currentsize] = 0;
			names[currentsize][0] = '\0';
			//Continue from current slot as next slot is shifted up
			i--;
			currentsize --;
		}
		//Done if there are no more slots
		if(!slotsleft)
			return;
	}
	return;
}

//List all wait list slots and their status
void list(void)
{
	int i;
	//Show all used waitlisted slots with group size
	for(i=0;i<currentsize;i++)
	{
		printf("The number %d slot is waitlisted by %s with group size %d.\n", i+1 ,names[i], groupsize[i]);
	}
	// Show free wait list slots
	while(i < maxsize)
	{
		printf("The number %d slot in waitlist is free.\n",i+1);
		i++;
	}
	return;
}
