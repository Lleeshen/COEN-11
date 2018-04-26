//Lyman Shen 2/23/17

#include "link.h"

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
	while(fread(&p,sizeof(SLOT),1,infp) == 1)
	{
		printf("%s has a waitlist with size %d.\n",p.names,p.groupsize);
	}
	pthread_mutex_unlock(&lock);
	return;
}

//Read file from argument
void readfile(char *fname, char *keyn)
{
	//Make sure file opens successfully
	FILE *infp;
	if((infp = fopen(fname, "r")) == NULL)
	{
		printf("Error opening file, starting with empty wait list.\n");
		return;
	}
	char line[100];
	char name[20];
	char groups[5];
	int group;
	int i;
	char *q;
	q = line;
	int used;
	int move = 0;
	int j;
	char key = atoi(keyn);
	while(fgets(line,100,infp) != NULL)
	{
		printf("Start decryption.\n");
		//Decrypt
		while(*q != '\0')
		{
			*q ^= key;
			q++;
		}
		printf("Decryption complete.\n");
		while (sscanf(line + move, "%s%s%n", name, groups,&used) > 1)
		{
			//Scan at right place
			move += used;
			printf("Check for duplicate name\n");
			i = duplicatename(name);
			if(i == 0)
			{
				continue;
			}
			printf("Start insert.\n");
			group = atoi(groups);
			if(group != 0)
			{
				printf("Converted group to integer.\n");
				insert(name, group);
				printf("Insert successful.\n");
			}
		}
		printf("Insert complete.\n");
	}
	printf("Waiting list file successfully loaded.\n");
	list();
	fclose(infp);
	return;
}

//Update or create file
void savefile(char *fname, char *keyn)
{
	char key = (char) atoi(keyn);
	char line[100];
	char *q;
	int length = 0;
	//Make sure file successfully opens
	FILE *outfp;
	if((outfp = fopen(fname, "w")) == NULL)
	{
		printf("Error saving wait list into file.\n");
		return;
	}
	//Insert each name, group size in rows
	int i;
	SLOT *p;
	for(i=0;i<4;i++)
	{
		p = waitl[i].head;
		while(p != NULL)
		{
			//Put every person on same line
			length += sprintf(line+length, "%s %d ",p->names, p->groupsize);
			p = p->next;
		}
	}
	//Decrypt
	q = line;
	while(*q != '\0')
	{
		*q ^= key;
		q++;
	}
	fprintf(outfp,"%s",line);
	sprintf(line+length,"\0");
	fclose(outfp);
	return;
}
