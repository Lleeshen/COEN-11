//Lyman Shen 1/19/17

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define maxsize 10

typedef struct waitlistperson
{
	char names[20];
	int groupsize;
	struct waitlistperson *next;
} RESSLOT;

void insert(void);
int duplicatename(char *str);
void removeRes(void);
void list(void);
void listgroup(void);

RESSLOT *head = NULL;
RESSLOT *tail = NULL;

int main(void)
{
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
	RESSLOT *p;
	if((p=(RESSLOT *)malloc(sizeof(RESSLOT))) ==  (RESSLOT *) NULL)
	{
		printf("Memory cannot be allocated.\n");
		return;
	}
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
	//Link the structure
	//Empty list
	if(head == NULL)
	{
		head = p;
		tail = p;
	}
	//There are elements in waitlist
	else
	{
		tail->next = p;
	}
	//Input values in structure
	strcpy(p->names,name);
	p->groupsize = group;
	tail = p;
	p->next = NULL ;
	printf("You have successfully put yourself on the wait list.\n");
	return;
}

//Check for duplicate names
int duplicatename(char *str)
{
	RESSLOT *p = head;
	while(p != NULL)
	{
		if(strcmp(str,p->names)!=0)
		{
			p = p->next;
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
	//Check if empty
	if(head == NULL)
	{
		printf("No slots to remove.\n");
		return;
	}
	int slotsleft;
	//Enter opening size
	printf("How big is the opening size?\n");
	scanf("%d",&slotsleft);
	RESSLOT *p = head;
	RESSLOT *q = head;
	//Check for each wait list in order
	while(p != NULL)
	{
		//Remove if enough slots for group
		if(slotsleft >=  p->groupsize)
		{
			printf("%s with groupsize %d has been successfully placed on a reservation and is now removed from waitlist.\n",p->names,p->groupsize);
			//Update slots left
			slotsleft -= p->groupsize;
			//1 slot
			if(head->next == NULL )
			{
				head = NULL;
				tail = NULL;
				free(p);
				return;
			}
			//head slot
			else if (head == p)
			{
				head = p->next;
				free(q);
				q=head;
				p=head;
			}
			//end slot
			else if (tail == p)
			{
				tail = q;
				q->next = NULL;
				free(p);
				return;
			}
			//middle slots
			else
			{
				q->next = p->next;
				free(p);
				p = q->next;
			}
		}
		else
		{
			q=p;
			p=p->next;
		}
		//Done if there are no more slots
		if(slotsleft<1)
			return;
	}
	return;
}

//List all wait list slots and their status
void list(void)
{
	RESSLOT *p = head;
	int i = 1;
	printf("\n");
	//Show all used waitlisted slots with group size
	while(p != NULL)
	{
		printf("The number %d slot is waitlisted by %s with group size %d.\n", i , p->names, p->groupsize);
		i++;
		p = p->next;
	}
	printf("\n");
	return;
}

//List all wait listed reservation smaller than a designated groupsize
void listgroup(void)
{
	RESSLOT *p = head;
	int i=1;
	int constraint;
	//Enter max groupsize for list
	printf("What is the largest group size you would like to list?\n");
	scanf("%d",&constraint);
	printf("\n");
	//Show waitlisted reservation smaller than group size
	while(p != NULL)
	{
		if(p->groupsize <= constraint)
			printf("The number %d slot is waitlisted by %s with group size %d.\n", i, p->names, p->groupsize);
		i++;
		p = p->next;
	}
	printf("\n");
	return;
}
