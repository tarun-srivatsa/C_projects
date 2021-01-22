#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct node{
    int present;
    struct node *next[26];
} tnode;
/*each node can hold a ptr "next" to any letter if it's inserted*/

tnode* create();
void insert(tnode*,char*);
int find(tnode*,char*);
char* clean(char*);
int isalpha(char*);

void wordcheck(tnode*);
void sentence(tnode*);
void testrun(tnode*);

int main(){
	printf("\n\t\t\tDICTIONARY TREE (TRIE) WITH FUNCTIONALITIES\n\n");
	printf("\t\t\tBy: Vaibhav Porwal & Tarun Srivatsa V S\n");
	printf("\t\t\tRV COLLEGE OF ENGINEERING, BANGALORE\n\n");
	
	char s[100];
	tnode *root=create();
	for(int i=0;i<3;i++) {
        printf("\t#\t#\t#\t#");
       	sleep(1);
	}
	
    FILE *DPTR=fopen("dictionary list.txt","r");
	if(DPTR==NULL){
		printf("No such file.");
		return 0;
    } else {
	 	while(fscanf(DPTR,"%s",s)==1) insert(root,s);
	    fclose(DPTR);
    }
    
    FILE *NEWF=fopen("newinsert.txt","a+");
	if(NEWF==NULL){
		printf("\n\nNo such file..\n");
		return 0;
    } else {
	 	while(fscanf(NEWF,"%s",s)==1) insert(root,s);
	    fclose(NEWF);
    }
    
    printf("\n\n\t\t# # # DICTIONARY READY # # #\n\npress ANY KEY to continue..");
    /*dictionary 370,105 words, 3,494,712 letters, this trie has 1,027,819 nodes*/
    getchar();
      
    while(1){
    	int choice;
       	printf("\n\n1. DICTIONARY WORD CHECK\n2. SENTENCE CHECK\n3. RUN THROUGH \"test1.txt\" FILE\n0. EXIT\nchoice: ");
        scanf("%d",&choice);
		if(choice==0) break;
		switch(choice){
        	case 1: wordcheck(root);
			break;
			
			case 2: sentence(root);
			break;
			
			case 3: testrun(root);
			break;
		}
		printf("\n\npress ANY KEY to continue..");
		getchar();
		system("cls");
	}
	return 0;
}

tnode* create(){
	int i;
    tnode *n=(tnode*)malloc(sizeof(tnode));
    n->present=0;
    for( i=0;i<26;i++) n->next[i]=NULL;
    return n;
}

void insert(tnode *root,char *s){
    int i,l=strlen(s),loc;
    tnode *temp=root;
    for(i=0;i<l;i++)
	{
        loc=(int)s[i]-'a';
        if(temp->next[loc]==NULL) temp->next[loc]=create();
        temp=temp->next[loc];
        if(i==l-1) temp->present=1;
    }
}

int find(tnode *root,char *s){		//returns 1 for found, 0 for not found
    int i=0,l=strlen(s),loc;
    tnode *temp=root;
    if(isalpha(s)==0) return 1;
    //ignoring strings which don't contain english alphabet
    for(i=0;i<l;i++){
        loc=(int)s[i]-'a';
        if(temp->next[loc]==NULL) return 0;
        temp=temp->next[loc];
    }
    if(temp->present) return 1;
    return 0;
}

char* clean(char *s){				//returns ptr, careful if it's returning just '\0'
	char *k=s;			//k ptr increments if any character is removed initially
	int i,word=0;
	for(i=0;s[i]!='\0';i++) 
		if((int)s[i]-'0'>=0 && (int)s[i]-'0'<=9) return k;
	i=0;
	while(s[i]!='\0'){
		if(s[i]>='A'&&s[i]<='Z'||s[i]>='a'&&s[i]<='z') word=1;
		if(s[i]>='A'&&s[i]<='Z') s[i]=s[i]+32;
		
		if(s[i]<'a'||s[i]>'z'){
			if(word==0) k=(s+i+1);
			else {s[i]='\0'; break;}
		}
		i++;
	}
	return k;
}

int isalpha(char *s){
	int i;
	for(i=0;s[i]!='\0';i++)
		if(!(s[i]>='a'&&s[i]<='z' || s[i]>='A'&&s[i]<='Z')) return 0;
	return 1;
}

void wordcheck(tnode *root){
	char s[32],*w;
	printf("\nENTER SINGLE WORD: ");
    scanf("%s",s);
    if(!isalpha(s)){
    	printf("\nInvalid characters\n");
    	return;
	}
	w=clean(s);
    if(!find(root,w)){
		printf("\tWORD NOT PRESENT IN DICTIONARY\n");
		printf("\tInsert this Word into Dictionary? (Y/N)     ");
		char c;
		scanf(" %c",&c);
		
		if(c=='Y'||c=='y'){
			insert(root,w);
			FILE *NEWF=fopen("newinsert.txt","a+");
			fprintf(NEWF,"%s\n",w);
           	fclose(NEWF);
		}  	
	} else printf("\n\tWORD PRESENT IN DICTIONARY\n");
	getchar();
}

void sentence(tnode *root){
	char s[200],cs[50],*w;
	int count=0;
	printf("\tEnter Sentence: ");
	getchar();
	fgets(s,200,stdin);
	FILE *SENF=fopen("sentence input.txt","w");
	fprintf(SENF,"%s",s);
	fclose(SENF);
	
	printf("\n\tCLEANED SENTENCE: ");
	SENF=fopen("sentence input.txt","r");
	while(fscanf(SENF,"%s",s)==1){
		strcpy(cs,s);
		w=clean(cs);	
		if(!find(root,w)){
			printf("##%s## ",w);
			count++;
		} else printf("%s ",w);
    }
	fclose(SENF);
	
	if(count==0){
		printf("\n\tSENTENCE PASSED\n");
		return;
	} else {
		printf("\n\t(ERRORS HIGHLIGHTED AS ##ERROR##)\n\n");
		
	}
}

void testrun(tnode *root){
	int count=0;
	FILE *TPT=fopen("test1.txt","r");
	if(TPT==NULL){
    	printf("No such file..");
		return;
	}
	printf("\n\n\t");
	char cs[100],s[100],*w;
	while(fscanf(TPT,"%s",s)==1){
		strcpy(cs,s);
		w=clean(cs);	
		if(!find(root,w)){
			printf("##%s## ",w);
			count++;
		} else printf("%s ",w);
    }
	fclose(TPT);
	if(count==0){
		printf("\n\tPARAGRAPH PASSED\n");
		return;
	}
	
	printf("\n\t\t(ERRORS HIGHLIGHTED AS ##ERROR##)\n\n");
	printf("\tLIST OF WORDS NOT FOUND: \n");
	count=0;
	TPT=fopen("test1.txt","r");
	while(fscanf(TPT,"%s",s)==1){
		strcpy(cs,s);
		w=clean(cs);	
		if(!find(root,w)){
			count++;
			printf("\t%d.\t%s\n",count,w);
		}
    }
    fclose(TPT);
    getchar();
}
