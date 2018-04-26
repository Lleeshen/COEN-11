//Lyman Shen 2/02/17

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define maxsize 10

typedef union info
{
	char phone[20];
	int minage;
	float average;
} INFO;

typedef struct waitlistperson
{
	char names[20];
	int groupsize;
	int maxage;
	INFO r_age;
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
	//Only insert if slots are open
	if(currentsize < maxsize)
	{
		char name [20];
		int group;
		int maxage;
		INFO extra;
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
		//Enter max age
		printf("What is the maximum age?\n");
		scanf("%d",&maxage);
		while(maxage < 1)
		{
			printf("Use the real age.\n");
			scanf("%d",&maxage);
		}
		//Enter info
		if(maxage < 18)
		{
			printf("What is the emergency contact information of an adult?\n");
			scanf("%s",extra.phone);
			strcpy(slots[currentsize].r_age.phone,extra.phone);
		}
		else if(maxage > 60)
		{
			printf("What is the minimum age of the group?\n");
			scanf("%d",&extra.minage);
			while(1 > extra.minage || maxage < extra.minage)
			{
				printf("Enter a valid minimuim age.\n");
				scanf("%d",&extra.minage);
			}
			slots[currentsize].r_age.minage = extra.minage;
		}
		else
		{
			printf("What is the average age?\n");
			scanf("%f",&extra.average);
			slots[currentsize].r_age.average = extra.average;
		}
		//Put reservation at end of list, increment counter
		strcpy(slots[currentsize].names,name);
		slots[currentsize].groupsize = group;
		slots[currentsize].maxage = maxage;
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
	int i=0;
	while(i<currentsize)
	{
		if(strcmp(str,slots[i].names)!=0)
		{
			i++;
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
		if(slotsleft >= slots[i].groupsize)
		{
			printf("%s with groupsize %d has been successfully placed on a reservation and is now removed from waitlist.\n",slots[i].names,slots[i].groupsize);
			//Update slots left, shift other slots up
			slotsleft -= slots[i].groupsize;
			for(j=i;j<currentsize-1;j++)
			{
				slots[j] = slots[j+1];
			}
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
		printf("The number %d slot is waitlisted by %s with group size %d. ", i+1 ,slots[i].names, slots[i].groupsize);
		if(slots[i].maxage < 18)
		{
			printf("The oldest member is %d years old, and the emergency contact number is %s.\n",slots[i].maxage,slots[i].r_age.phone);
		}
		else if(slots[i].maxage > 60)
		{
			printf("The oldest member is %d years old, and the youngest member is %d years old.\n",slots[i].maxage,slots[i].r_age.minage);
		}
		else
		{
			printf("The oldest member is %d years old, and the average age is %2.3f years old.\n",slots[i].maxage,slots[i].r_age.average);
		}
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
	int i;
	int constraint;
	//Enter max groupsize for list
	printf("What is the largest group size you would like to list?\n");
	scanf("%d",&constraint);
	//Show waitlisted reservation smaller than group size
	for(i=0;i<currentsize;i++)
	{
		if(slots[i].groupsize <= constraint)
			printf("The number %d slot is waitlisted by %s with group size %d.\n", i+1 ,slots[i].names, slots[i].groupsize);
	}
	return;
}
