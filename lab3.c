//Lyman Shen 1/19/17

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define maxsize 10

typedef struct waitlistperson
{
	char names[20];
	int groupsize;
} RESSLOT;

RESSLOT slots [maxsize];
int currentsize = 0;

void insert(void);
int duplicatename(char *str);
void removeRes(void);
void list(void);
void listgroup(void);

int main(void)
{
	//initialize
	int i;
	char option;
	RESSLOT *personp = slots;
	for(i=0;i<maxsize;i++,personp++)
	{
		personp->groupsize = 0;
		personp->names[0] = '\0';
	}
	printf("Welcome to my Surf Lessons.\n");
	//Main loop
	while(1)
	{
		//Choose function to go to from int input
		printf("To add to wait list, type 1. To remove a  waitlist, type 2. To show the wait list, type 3. To show waitlists under a group size, type 4. To quit, type 0.\n");
		scanf(" %c",&option);
		switch(option)
		{
			case '1':
				//insert waitlist
				insert();
				break;
			case '2':
				//remove waitlist
				removeRes();
				break;
			case '3':
				//show waitlist
				list();
				break;
			case '4':
				//List groups smaller than a size
				listgroup();
				break;
			case '0':
				//end program
				return 0;
			default:
				printf("Please enter 1, 2, 3, 4 or 0.\n");
		}
	}
}

//Insert into waitlist
void insert(void)
{
	RESSLOT *personp = slots;
	//Only insert if slots are open
	if(currentsize < maxsize)
	{
		char name [20];
		int group;
		int i;
		//Enter names
		printf("What is the reservation name under?\n");
		scanf("%s",name);
		//Check for used names, exit if used
		i = duplicatename(name);
		if(i == 0)
			return;
		//Enter group size
		printf("What is size of the reservation group?\n");
		scanf("%d",&group);
		while(group < 1)
		{
			printf("Use a valid size for reservation group.\n");
			scanf("%d",&group);
		}
		//Put name, size at end of list, increment counter
		personp += currentsize;
		strcpy(personp->names,name);
		personp->groupsize = group;
		currentsize++;
		printf("You have successfully put yourself on the wait list.\n");
	}
	else
		//Notify full wait list
		printf("The waitlist is full. Try again later.\n");
	return;
}

//Check for duplicate names
int duplicatename(char *str)
{
	RESSLOT *personp = slots;
	int i=0;
	while(i<currentsize)
	{
		if(strcmp(str,personp->names)!=0)
		{
			i++,personp++;
		}
		else
		{
			printf("The name is taken.\n");
			return 0;
		}
	}
	return 1;
}

//Remove waitlist with opening slots
void removeRes(void)
{
	RESSLOT *personp = slots;
	RESSLOT *personq = slots;
	int i;
	int j;
	int slotsleft;
	//Enter opening size
	printf("How big is the opening size?\n");
	scanf("%d",&slotsleft);
	//Check for each wait list in order
	for(i=0;i<currentsize;i++,personp++)
	{
		//Remove if enough slots for group
		if(slotsleft >=  personp->groupsize)
		{
			printf("%s with groupsize %d has been successfully placed on a reservation and is now removed from waitlist.\n",personp->names,personp->groupsize);
			//Update slots left, shift other slots up
			slotsleft -= personp->groupsize;
			for(j=i;j<currentsize-1;j++)
			{
				personq[j] = personq[j+1];
			}
			//Continue from current slot as next slot is shifted up
			i--,personp--;
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
	RESSLOT *personp = slots;
	int i;
	//Show all used waitlisted slots with group size
	for(i=0;i<currentsize;i++,personp++)
	{
		printf("The number %d slot is waitlisted by %s with group size %d.\n", i+1 ,personp->names, personp->groupsize);
	}
	// Show free wait list slots
	while(i < maxsize)
	{
		printf("The number %d slot in waitlist is free.\n",i+1);
		i++;
	}
	return;
}

//List all wait listed reservation smaller than a designated groupsize
void listgroup(void)
{
	RESSLOT *personp = slots;
	int i;
	int constraint;
	//Enter max groupsize for list
	printf("What is the largest group size you would like to list?\n");
	scanf("%d",&constraint);
	//Show waitlisted reservation smaller than group size
	for(i=0;i<currentsize;i++,personp++)
	{
		if(personp->groupsize <= constraint)
			printf("The number %d slot is waitlisted by %s with group size %d.\n", i+1 ,personp->names, personp->groupsize);
	}
	return;
}
