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

extern RESSLOT waitl[4];
extern pthread_mutex_t lock;

void * AutoSave (void * arg);
void listAutosave (char *name);
void readfile(char *fname, char *keyn);
void savefile(char *fname, char *keyn);
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
