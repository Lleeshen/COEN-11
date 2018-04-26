//Lyman Shen 2/23/17

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct waitlistperson
{
	char names[20];
	int groupsize;
	struct waitlistperson *next;
} SLOT;

typedef struct waitlistgroup
{
	SLOT *head;
	SLOT *tail;
}RESSLOT;

void * AutoSave (void * arg);
void listAutosave (char *name);
void readfile(char *fname);
void savefile(char *fname);
void insert(char *name, int group);
void removeRes(void);
int duplicatename(char *str);
void list(void);
void listgroup(void);
void freeall(void);
void free_recur(int place, SLOT *begin);
void changesize(void);
void showListBackward(int list, SLOT *wl);
void showNameBackward(int list, SLOT *wl, char *ptr);

RESSLOT waitl[4];
pthread_mutex_t lock;

int main(int argc, char *argv[])
{
	char option;
	char name [20];
	int group;
	int i;
	printf("Welcome to my Surf Lessons.\n");
	if(argc == 1)
	{
		printf("No wait list data availible. Trying to load default file.\n");
		readfile("init");
	}
	else
	{
		readfile(argv[1]);
	}
	//Create thread
	pthread_t thr;
	if(argc == 3)
	{
		pthread_create(&thr,NULL,AutoSave,(void *) argv[2]);
	}
	else
	{
		pthread_create(&thr,NULL,AutoSave,(void *) "temp.bin");
	}
	pthread_mutex_init(&lock,NULL);
	//Main loop
	while(1)
	{
		//Choose function to go to from int input
		printf("To add to wait list, type 1. To remove a  waitlist, type 2. To show the wait list, type 3. To show waitlists under a group size, type 4. To change the size of a wait list by name (WARNING: moves you to end of wait list), type 5. To show every list backward, type 6. To show every name backward, type 7. To list the reservations in the auto-save file, type 8. To quit, type 0.\n");
		fflush(stdin);
		scanf("%c",&option);
		switch(option)
		{
			case '1':
				//insert waitlist
				//Enter names
				printf("What is the reservation name under?\n");
				scanf("%s",name);
				//Enter group size
				printf("What is size of the reservation group?\n");
				scanf("%d",&group);
				while(group < 1)
				{
					printf("Use a valid size for reservation group.\n");
					scanf("%d",&group);
				}
				//Check for used names, exit if used
				i = duplicatename(name);
				if(i == 1)
				{
					insert(name, group);
				}
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
			case '6':
				//Show lists backward
				showListBackward(0,waitl[0].head);
				break;
			case '7':
				//Show names backward
				showNameBackward(0, waitl[0].head, waitl[0].head->names);
				putchar('\n');
				break;
			case '8':
				//List all waitlists in autosave file
				if(argc == 3)
				{
					listAutosave(argv[2]);
				}
				else
				{
					listAutosave("temp.bin");
				}
				break;
			case '0':
				//end program
				//Make file
				if(argc == 1)
				{
					savefile("init");
					printf("Successfully saved waitlist.\n");
				}
				//Update file
				else
				{
					savefile(argv[1]);
					printf("Successfully updated waitlist.\n");
				}
				//End thread
				pthread_mutex_lock(&lock);
				printf("Starting to join thread.\n");
				pthread_cancel(thr);
				printf("Successfully joined thread.\n");
				pthread_mutex_unlock(&lock);
				printf("Successfully unlocked thread.\n");
				//Free first node to start chain to free all
				free_recur(0, waitl[0].head);
				return 0;
			default:
				printf("Please enter 1, 2, 3, 4, 5, 6, 7, 8 or 0.\n");
		}
	}
}

//AutoSave File
void *AutoSave (void * arg)
{
	int i;
	SLOT *p;
	FILE *outfp;
	while(1)
	{
		sleep(15);
		if((outfp = fopen((char *) arg, "wb")) == NULL)
		{
			printf("Error loading autosave file.\n");
		}
		else
		{
			pthread_mutex_lock(&lock);
			for(i=0;i<4;i++)
			{
				p = waitl[i].head;
				while(p != NULL)
				{
					fwrite(p,sizeof(SLOT),1,outfp);
					p = p->next;
				}
			}
			printf("Autosave is successful.\n");
			pthread_mutex_unlock(&lock);
		}
		fclose(outfp);
	}
}

//List reservations in autosave file
void listAutosave (char *name)
{
	SLOT p;
	FILE *infp;
	pthread_mutex_lock(&lock);
	if((infp = fopen(name,"rb")) == NULL)
	{
		printf("Error opening file.\n");
		return;
	}
	pthread_mutex_unlock(&lock);
	while(fread(&p,sizeof(SLOT),1,infp) == 1)
	{
		printf("%s has a waitlist with size %d.\n",p.names,p.groupsize);
	}
	return;
}

//Read file from argument
void readfile(char *fname)
{
	//Make sure file opens successfully
	FILE *infp;
	if((infp = fopen(fname, "r")) == NULL)
	{
		printf("Error opening file, starting with empty wait list.\n");
		return;
	}
	//35 chars to start
	fseek(infp,35,SEEK_SET);
	char name[20];
	int group;
	int i;
	while(fscanf(infp,"%s%d",name,&group) == 2)
	{
		//Check for used names, skip if used
		i = duplicatename(name);
		if(i == 0)
		{
			continue;
		}
		insert(name, group);
	}
	printf("Waiting list file successfully loaded.\n");
	list();
	fclose(infp);
	return;
}

//Update or create file
void savefile(char *fname)
{
	//Make sure file successfully opens
	FILE *outfp;
	if((outfp = fopen(fname, "w")) == NULL)
	{
		printf("Error saving wait list into file.\n");
		return;
	}
	//Put header
	fprintf(outfp,"Name\tGroup Size\n------------------\n");
	//Insert each name, group size in rows
	int i;
	SLOT *p;
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p != NULL)
		{
			fprintf(outfp,"%s\t%d\n",p->names,p->groupsize);
			p = p->next;
		}
	}
	fclose(outfp);
	return;
}

//Insert into waitlist
void insert(char *name, int group)
{
	SLOT *p;
	if((p=(SLOT *)malloc(sizeof(SLOT))) ==  (SLOT *) NULL)
	{
		printf("Memory cannot be allocated.\n");
		return;
	}
	int i;
	//Determine which list to use
	i = (group < 7) ? (group-1)/2 : 3;
	//Link the structure
	pthread_mutex_lock(&lock);
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
	pthread_mutex_unlock(&lock);
	printf("Inserting into waitlist is successful.\n");
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
				pthread_mutex_lock(&lock);
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
				pthread_mutex_unlock(&lock);
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
		//Tell user if head is NULL
		if(p == NULL)
		{
			printf("There are no waitlist in division %d.\n",i);
		}
		else
		{
			//Show all used waitlisted slots with group size in appropiate division
			while(p != NULL)
			{
				printf("In division %d, the number %d slot is waitlisted by %s with group size %d.\n", i, j , p->names, p->groupsize);
				j++;
				p = p->next;
			}
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
}

//Free all slots recursively
void free_recur(int place, SLOT *p)
{
	//Free next list's head if at last NODE and not at last list
	if(p->next == NULL && place != 3)
	{
		free_recur(place+1,waitl[place+1].head);
	}
	//Free next slot if availible
	else if (p->next != NULL)
	{
		free_recur(place, p->next);
	}
	free(p);
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
				pthread_mutex_lock(&lock);
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
				pthread_mutex_unlock(&lock);
				return;
			}
			q = p;
			p = p->next;
		}
	}
	printf("The name does not exist.\n");
	return;
}

//Use recursive function to show list backward
//Get position of list every call
void showListBackward(int i, SLOT *p)
{
	int j = i;
	//Move to next linked list at end of current list
	if(p->next == NULL && i != 0)
	{
		j++;
		showListBackward(j,waitl[j].head);
	}
	//Show next item on list before showing current item
	else if(p -> next != NULL)
	{
		showListBackward(i,p->next);
	}
	printf("%s\n", p->names);
	return;
}

//Use recursive function to every name backward
//Get position of char every call
void showNameBackward(int i, SLOT* p, char *ptr)
{
	//Move to next list
	if(p->next == NULL && i !=3 && *ptr == '\0')
	{
		showNameBackward(i+1,waitl[i+1].head,waitl[i+1].head->names);
		putchar('\n');
	}
	//Move to next NODE
	else if(p->next != NULL && *ptr == '\0')
	{
		showNameBackward(i,p->next,p->next->names);
		putchar('\n');
	}
	//Print next character before printing current character
	else if(*ptr != '\0')
	{
		showNameBackward(i,p, ptr+1);
		putchar(*ptr);
	}
	else
	{
		putchar('\n');
	}
	return;
}
