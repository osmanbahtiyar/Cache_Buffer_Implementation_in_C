#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct node{
	struct node *prev,*next;
	int requestCount;
	char siteAdress[50];
}NODE;

void getInfo(char*,int*,int*,int*,FILE**);
int stringToInteger(char*);
void scanInput(char*,int,FILE**);
void control(char*,NODE**,int*);
NODE* search(NODE**,char*);
void pushFront(NODE**,char*);
void deleteLastNODE(NODE**);
void moveFront(NODE**,NODE*);
void display(NODE*);

int main(){
	
	NODE *head = NULL;
	NODE *theNODE = NULL;
	int inputType;
	int T,L;
	int cacheSize = 0;
	FILE *inputFile;
	char buffer[50];
	
	getInfo(buffer,&T,&L,&inputType,&inputFile);
	while(1){
		do{
			//get input string to buffer
			scanInput(buffer,inputType,&inputFile);
			//control string is a command or not
			control(buffer,&head,&cacheSize);
		}while(!strcmp(buffer,"!clear!"));
		//if the node is already exist return it else return null
		theNODE = search(&head,buffer);
		if(theNODE == NULL){
			//if the node is not exist create it and pushfront
			pushFront(&head,buffer);
			//because of pushing front increase cachesize
			cacheSize++;
			//if cache size s bigger than limit size delete last node and decrease cachesize
			if(cacheSize > L){
				deleteLastNODE(&head);
				cacheSize--;
			}
		}else{
			//if the node is already exist increase its request count
			theNODE->requestCount++;
			if(theNODE->requestCount > T){
				//if request count of the node is bigger than the treshold value make it head
				moveFront(&head,theNODE);
			}
		}
	display(head);	
	}
	fclose(inputFile);
	return 0;
}

void getInfo(char *buffer,int *T,int *L,int *inputType,FILE **inputFile){
	printf("!!Welcome to the program!!\n");
	printf("Please read me!!!\n");
	printf("If you scan inputs from file the file must be T=3 , L=4 vb.format\n");
	printf("Make sure there are spaces around ',' \n");
	printf("After display program asks you want to clear or finish.\n");
	printf("If you want to clear cache type '!clear!'\n");
	printf("If you want to finish program type '!finish!'\n");
	printf("If you want to continue press any key\n");
	printf("1-From Terminal\n");
	printf("2-From File\n");
	printf("Where do you wish get inputs from? : ");
	scanf("%d",inputType); 
	if(*inputType == 1){
		//if inputType == 1 scan form terminal
		printf("T : ");
		scanf("%d",T);//ok
		printf("L : ");
		scanf("%d",L);//ok
	}else if(*inputType == 2){
		//if inputType == 2 scan from file
		printf("FileName : ");
		scanf("%s",buffer);
		*inputFile = fopen(buffer,"r");
		if(*inputFile == NULL){
			//allocation check
			printf("FILE ERROR\n");
			exit(0);
		}
		//get T and L integer value from txt which is T=3 , L=2 normally
		fscanf(*inputFile,"%s",buffer);
		*T = stringToInteger(buffer);
		
		//dummy ','
		fscanf(*inputFile,"%s",buffer);
		
		fscanf(*inputFile,"%s",buffer);
		*L = stringToInteger(buffer);
	}else{
		printf("INVALID INPUT OPTION");
		exit(2);
	}
}

int stringToInteger(char *string){
	//convert string T=4 to int T = 4 form;
	int integer = 0;
	int k = 1;
	int n = strlen(string) -1;
	while(string[n] != '='){
		integer += k*(string[n] - '0');
		k *= 10;
		n--;
	}
	return integer;
}

void scanInput(char *buffer,int inputType,FILE **inputFile){
	// scan input according to inputType 1 terminal 2 file
	if(inputType == 1){
		printf("\n");
		scanf("%s",buffer);
	}else if(inputType == 2){
		//scan from file until the end of the file
		if(!feof(*inputFile)){
			printf("\n");
			fscanf((*inputFile),"%s",buffer);
		}else{
			printf("\n\nEND OF FILE\n");
			exit(2);
		}
	}
}

void control(char *buffer,NODE **head,int *cacheSize){ //ok
	//control input if it is !finish! finish program if it is !clear! clear cache
	//if it is a different input do nothing
	if(!strcmp(buffer,"!finish!")){
		printf("PROGRAM FINISHED!!");
		exit(1);
	}
	if(!strcmp(buffer,"!clear!")){
		//from head to end free each node
		NODE* current = *head;
		NODE* next;
		while(current != NULL){
			next = current->next;
			free(current);
			current = next;
		}
		*head = NULL;
		printf("CACHE CLEANED\n");
		//set cache size 0
		*cacheSize = 0;
	}
}

NODE* search(NODE **head,char *buffer){
	//if input is already exist retrun it else return null
	NODE *current = *head;
	if(current == NULL){
		return NULL;
	}
	while(current != NULL && strcmp(current->siteAdress,buffer)){
		current = current->next;
	}
	if(current == NULL){
		return NULL;
	}else{
		return current;
	}
}

void pushFront(NODE** head,char *buffer){
	//create new node with input and set it head
	NODE* newNODE = (NODE*)malloc(sizeof(NODE));
	if((*head) == NULL){
		newNODE->next = NULL;
		newNODE->prev = NULL;
		newNODE->requestCount = 1;
		strcpy(newNODE->siteAdress,buffer);
		(*head) = newNODE;
	}else{
		newNODE->prev = NULL;
		newNODE->next = (*head);
		(*head)->prev = newNODE;
		newNODE->requestCount = 1;
		strcpy(newNODE->siteAdress,buffer);
		(*head) = newNODE;
	}
}

void deleteLastNODE(NODE **head){
	NODE *del = (*head);
	//if del->next is NULL then del is the last node
	while(del->next != NULL){
		del = del->next;
	}
	del->prev->next = NULL;
	free(del);
}

void moveFront(NODE **head,NODE *theNODE){
	//if one node exceeds treshold value move it head and set relations between its next and prev
	//if the node is already head do nothing
	if(theNODE != (*head)){
		//if the node is the last node
		if(theNODE->next == NULL){
			theNODE->prev->next = NULL;
			theNODE->prev = NULL;
			theNODE->next = (*head);
			(*head)->prev = theNODE;
			(*head) = theNODE;
		}else{
			//if the node is in the middle of linked list
			theNODE->prev->next = theNODE->next;
			theNODE->next->prev = theNODE->prev;
			theNODE->prev = NULL;
			theNODE->next = (*head);
			(*head)->prev = theNODE;
			(*head) = theNODE;
		}
	}
}

void display(NODE *head){
	NODE* current = head;
	//if current->next == NULL then current is the last node
	while(current->next != NULL){
		printf("%s,%d <=> ",current->siteAdress,current->requestCount);
		current = current->next;
	}
	//printing last node
	printf("%s,%d",current->siteAdress,current->requestCount);
}



