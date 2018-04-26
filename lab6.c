//Lyman Shen 2/16/17

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define maxsize 10

typedef struct waitlistperson
{
	char names[20];
	int groupsize;
	struct waitlistperson *next;
} SLOT;

typedef struct waitlistgroup
{
	SLOT person;
	SLOT *head;
	SLOT *tail;
}RESSLOT;

void insert(void);
int duplicatename(char *str);
void removeRes(void);
void list(void);
void listgroup(void);
void freeall(void);
void changesize(void);

RESSLOT waitl[4];

int main(void)
{
	char option;
	printf("Welcome to my Surf Lessons.\n");
	//Main loop
	while(1)
	{
		//Choose function to go to from int input
		printf("To add to wait list, type 1. To remove a  waitlist, type 2. To show the wait list, type 3. To show waitlists under a group size, type 4. To change the size of a wait list by name (WARNING: moves you to end of wait list), type 5. To quit, type 0.\n");
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
			case '5':
				//change size of group
				changesize();
				break;
			case '0':
				//end program
				freeall();
				return 0;
			default:
				printf("Please enter 1, 2, 3, 4 or 0.\n");
		}
	}
}

//Insert into waitlist
void insert(void)
{
	SLOT *p;
	if((p=(SLOT *)malloc(sizeof(SLOT))) ==  (SLOT *) NULL)
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
	//Determine which list to use
	i = (group < 7) ? (group-1)/2 : 3;
	//Link the structure
	//Empty list
	if(waitl[i].head == NULL)
	{
		waitl[i].head = p;
		waitl[i].tail = p;
	}
	//There are elements in waitlist
	else
	{
		waitl[i].tail->next = p;
	}
	//Put values in structure
	strcpy(p->names,name);
	p->groupsize = group;
	waitl[i].tail = p;
	p->next = NULL ;
	printf("You have successfully put yourself on the wait list.\n");
	return;
}

//Check for duplicate names
int duplicatename(char *str)
{
	SLOT *p;
	int i;
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p != NULL)
		{
			if(strcmp(str,p->names)==0)
			{
				printf("The name is taken.\n");
				return 0;
			}
			p = p->next;
		}
	}
	return 1;
}

//Remove waitlist with opening slots
void removeRes(void)
{
	//Enter opening size
	int slotsleft;
	printf("How big is the opening size?\n");
	scanf("%d",&slotsleft);
	SLOT *p;
	SLOT *q;
	int i;
	//Check for each wait list in order	smallest to largest
	for(i=0;i<4;i++)
	{
		//Done if there are less slots than in next designated in division
		switch(i)
		{
			case 0:
				if(slotsleft < 1)
				{
					printf("No more slots to remove.\n");
					return;
				}
				break;
			case 1:
				if(slotsleft < 3)
				{
					printf("No more slots to remove.\n");
					return;
				}
				break;
			case 2:
				if(slotsleft < 5)
				{
					printf("No more slots to remove.\n");
					return;
				}
				break;
			case 3:
				if(slotsleft < 7)
				{
					printf("No more slots to remove.\n");
					return;
				}
				break;

		}
		p = waitl[i].head;
		q = waitl[i].head;
		while(p != NULL)
		{
			//Remove if enough slots for group
			if(slotsleft >=  p->groupsize)
			{
				printf("%s with groupsize %d has been successfully placed on a reservation and is now removed from waitlist.\n",p->names,p->groupsize);
				//Update slots left
				slotsleft -= p->groupsize;
				//1 slot
				if(waitl[i].head->next == NULL )
				{
					waitl[i].head = NULL;
					waitl[i].tail = NULL;
					free(p);
					break;
				}
				//head slot
				else if (waitl[i].head == p)
				{
					waitl[i].head = p->next;
					free(q);
					q=waitl[i].head;
					p=q;
				}
				//end slot
				else if (waitl[i].tail == p)
				{
					waitl[i].tail = q;
					q->next = NULL;
					free(p);
					break;
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
		}
	}
	return;
}

//List all wait list slots and their status
void list(void)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		SLOT *p = waitl[i].head;
		j = 1;
		//Show all used waitlisted slots with group size in appropiate division
		while(p != NULL)
		{
			printf("In division %d, the number %d slot is waitlisted by %s with group size %d.\n", i, j , p->names, p->groupsize);
			j++;
			p = p->next;
		}
		printf("\n");
	}
	return;
}

//List all wait listed reservation smaller than a designated groupsize
void listgroup(void)
{
	SLOT *p ;
	int j=1;
	int i;
	int constraint;
	//Enter max groupsize for list
	printf("What is the largest group size you would like to list?\n");
	scanf("%d",&constraint);
	printf("\n");
	//Show waitlisted reservation smaller than group size
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p != NULL)
		{
			if(p->groupsize <= constraint)
			{
				printf("In division %d, the number %d slot is waitlisted by %s with group size %d.\n", i, j , p->names, p->groupsize);
			}
			j++;
			p = p->next;
		}
		printf("\n");
		//Stop group size search if less than next division's group size
		switch(i)
		{
			case 0:
				if(constraint < 3)
				{
					return;
				}
				break;
			case 1:
				if(constraint < 5)
				{
					return;
				}
				break;
			case 2:
				if(constraint < 7)
				{
					return;
				}
				break;
		}
	}
	return;
}

//Free all slots
void freeall(void)
{
	int i;
	SLOT *q, *p;
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		q = waitl[i].head;
		while(p != NULL)
		{
			q = q->next;
			free(p);
			p = q;
		}
	}
	return;
}

//Change size
void changesize(void)
{
	SLOT *p, *q;
	int i,j;
	char name[20];
	int osize;
	int nsize;
	//Get name
	printf("What is the name of the group you want to change size?\n");
	scanf("%s",name);
	//Get old size
	printf("What is the size of the group?\n");
	scanf("%d",&osize);
	//Get new size
	printf("What is the group size you want to change to?\n");
	scanf("%d",&nsize);
	while(1 > nsize)
	{
		printf("Enter a valid group size to change to.");
		scanf("%d",&nsize);
	}
	//Search for name in array
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		q = p;
		while(p != NULL)
		{
			//Found name
			if(strcmp(name,p->names)==0)
			{
				//Check if person is real by comparing old size v actual size
				if(osize != p->groupsize)
				{
					printf("The inputted group size is not the real size.\n");
					return;
				}
				//Don't change list if new size is same
				if(nsize == p->groupsize)
				{
					printf("New size is same as old size.\n");
					return;
				}
				//If original is head, fix head (still becomes NULL if only one)
				if (waitl[i].head == p)
				{
					waitl[i].head = p->next;
				}
				//If original is tail, fix tail
				if (waitl[i].tail == p)
				{
					//Most tail cases, move tail back one
					if(q != waitl[i].tail)
					{
						waitl[i].tail = q;
					}
					//Tail is NULL is only one
					else
					{
						waitl[i].tail = NULL;
					}
				}
				//Make new link
				q->next = p->next;
				p->next = NULL;
				//Determine which list to put to
				j = (nsize < 7) ? (nsize-1)/2 : 3;
				//Put data in
				p->groupsize = nsize;
				p->next = NULL;
				//Link list at end
				if(waitl[j].head == NULL)
				{
					waitl[j].head = p;
				}
				//List at end
				else
				{
					waitl[j].tail->next = p;
				}
				waitl[j].tail = p;
				return;
			}
			q = p;
			p = p->next;
		}
	}
	printf("The name does not exist.\n");
	return;
}
