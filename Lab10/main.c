//Lyman Shen 2/23/17

#include "link.h"

RESSLOT waitl[4];
pthread_mutex_t lock;

int main(int argc, char *argv[])
{
	char option;
	char name [20];
	int group;
	int i;
	printf("Welcome to my Surf Lessons.\n");
	readfile(argv[1],argv[3]);
	printf("Files read.\n");
	//Create thread
	pthread_mutex_init(&lock,NULL);
	pthread_t thr;
	pthread_create(&thr,NULL,AutoSave,(void *) argv[2]);
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
				listAutosave(argv[2]);
				break;
			case '0':
				//end program
				savefile(argv[1],argv[3]);
				printf("Successfully saved waitlist.\n");
				//End thread
				pthread_mutex_lock(&lock);
				printf("Passed through lock.\n");
				pthread_cancel(thr);
				printf("Thread deleted.\n");
				pthread_mutex_unlock(&lock);
				printf("Thread unlocked.\n");
				//Free first node to start chain to free all
				free_recur(0, waitl[0].head);
				printf("Freeing done.\n");
				return 0;
			default:
				printf("Please enter 1, 2, 3, 4, 5, 6, 7, 8 or 0.\n");
		}
	}
}
