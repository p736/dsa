#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include <stdbool.h>
#include"function.h"
#define WORDS 30

pos type[WORDS];

int len_of_sen;

char type_of_word[9][20]={"nouns.txt","verbs.txt","adverbs.txt","adjectives.txt","interjection.txt","pronoun.txt","article.txt","preposition.txt","conjuction.txt"};

char speech_parts[9][15]={"noun","verb","adverb","adjective","interjection","pronoun","article","preposition","conjunction"};

int lines_no(char *file)
{
	int count=0;
	FILE *fptr;
	char ch;

	fptr = fopen(file,"r");

	while(ch!=EOF)
	{
		ch=fgetc(fptr);
		if(ch=='\n')
		count++;
	}
	fclose(fptr);
	return count;
}
//This function is to store from the file and search in a trie the word inputted by user

void file_search() {

    node *head=NULL,*curr,*currin;
    char find[100],temp[100];
    int no_of_lines,word_length;
    FILE *fptr;
    int i,j,flag,k,m;

    //calling function user_input
    user_input();

	//for loop runs 9 times because there are 9 different files of 9 parts of speech
	for(m=0;m<9;m++)
	{
		head=NULL;

	    // Allocating head for the first time
	    if(head == NULL)
	    {
	    	head = malloc( sizeof(node) );
		    head->word='*';
		    curr = head;
	    }


		//calculating no of lines using lines no function
		no_of_lines=lines_no(type_of_word[m]);

		//opening corresponding file in read mode
		fptr = fopen(type_of_word[m],"r");

		//checking  is file is opened sucessfully
		if(fptr == NULL)
		{
			printf("Cannont open file");
		}

		//file opened sucessfully
		else
		{
			//loop1 runs till no_of_lines are there in files
			for( j=1 ; j<=no_of_lines ; j++ )
			{
				currin=curr;
				//reading  a word from the file
				fscanf(fptr, "%s",find);

					//loop2 runs till length of word is reached
					word_length=strlen(find);
					for( i = 0 ; i<=word_length; i++ )
					{
						//creating trie structure
						if(find[i] != currin->word)
						{
							for(;currin->word!='*' && find[i]!=currin->word;currin=currin->hori);

							if(currin->word == '*')
							{
								currin->word = find[i];
								// Side ways allocation of memory
								currin->hori=malloc(sizeof(node));
								currin->hori->word='*';
								currin->hori->hori=NULL;
								currin->hori->child=NULL;

								// Downwards allocation of memory
								currin->child=malloc(sizeof(node));
								currin->child->word='*';
								currin->child->hori=NULL;
								currin->child->child=NULL;


								// Getting inside created branch
								currin=currin->child;
							}

							if(find[i] == currin->word)
							{
								//getting inside previously created branch
								currin=currin->child;
							}

						}

						else if(currin->word==find[i])
						{
							//getting inside previously created branch
							currin=currin->child;
						}
					}//end of loop2
	    		}//end of loop1

		fclose(fptr);
		}

		//herer find is used to search for each word of sentence corresponding to opened files
		for(k=1, strcpy(find,type[0].input); k<=len_of_sen; strcpy(find,type[k].input), k++)
		{
			currin=head;

			word_length=strlen(find);

			for(i=0,flag=0;i<=word_length;i++)
			{
				if(find[i]=='\0' && currin->word=='\0')
		    		{
		    			type[k-1].flag[m]=1;
		    			//below comment is to show accuracy without secondary search
		    			//printf("\n%s-->%s\n",find,speech_parts[m]);
		    			break;
		    		}

				if(find[i]==currin->word)
				{
					currin=currin->child;
				}

				else if(find[i]!=currin->word)
				{
					//for loop runs till horizontally end of trie ie * or word is found
					//if(currin->hori != NULL)
					for(currin=currin->hori;currin->word!='*'&&find[i]!=currin->word&&currin->hori!=NULL;currin=currin->hori);
					if(find[i]==currin->word && currin->word!='\0')
						currin=currin->child;

		    			if(find[i]=='\0' && currin->word=='\0')
		    			{
		    				type[k-1].flag[m]=1;
		    				//below comment is to show accuracy without secondary search
		    				//printf("%s-->%s\n",find,speech_parts[m]);
		    				break;
		    			}


					if(find[i]!=currin->word && currin->word=='*')
					{
						type[k-1].flag[m]=0;
						break;
					}
				}
			}
	    	}
	}//loop reruns to check for new file of different parts of speech

	//calling function secondary search
	secondary_search();
}


//This function is used to take input from the user
void user_input()
{

	int i,j;
	char ch;

	printf("\n>>> Enter a Sentence\n");

	for(i=0;ch!='\n' && i<WORDS;i++)
	{
		//Assigning ch=a as ch can garbage value or empty string so must be assigned a character ie a
		ch='a';
		for(j=0;isalpha(ch) && ch!='\n' && j<50;j++)
		{
			ch=getchar( );
			type[i].input[j]=ch;
		}
		//Allocating last element of string as null character
		type[i].input[j-1]='\0';
	}

	//to know the length of sentence
	len_of_sen=i;
}

//this function is used to cross check and confirm part of speech allocated
void secondary_search()
{
	//count is to count that no of times the word occurs in different files
	int i,m,count,flag=0;
	char temp[100];


	for(m=0;m<=len_of_sen;m++)
	{
		flag=0;
		count=0;

		for(i=0;i<9;i++)
		{
			if(type[m].flag[i])
				count++;
		}

		if(count==1)
		{
			for(i=0;i<9;i++)
			{
				if(type[m].flag[i])
				printf("\n... %s is %s\n",type[m].input,speech_parts[i]);
			}
		}

		else if(count>1 )
		{
			//printf("\n");
			strcpy(temp,type[m].input);

			//To check if the word is found in two files
			if(type[m].flag[0] &&( !strcmp(type[m-1].input,"a") || !strcmp(type[m-1].input,"an") || !strcmp(type[m-1].input,"the") || !strcmp(type[m-1].input,"this") || !strcmp(type[m-1].input,"those") || !strcmp(type[m-1].input,"that") || !strcmp(type[m-1].input,"these") || !strcmp(type[m-1].input,"any") || !strcmp(type[m-1].input,"no") || !strcmp(type[m-1].input,"few") || !strcmp(type[m-1].input,"several")))
			{
				printf("\n... %s is  Noun*\n",type[m].input);
			}

			else if(type[m].flag[1] && type[m+1].flag[3] || type[m+1].flag[6])
			{
				flag=1;
				printf("\n... %s is  Verb*",type[m].input);
			}

			else if(type[m].flag[2] && !type[m-1].flag[1] )
			{
				printf("\n... %s is  adverb*\n",type[m].input);
			}

			else if(type[m].flag[4] && (type[m-1].flag[4] || type[m+1].flag[4]))
			{
				printf("\n... %s is  interjection*\n",type[m].input);
			}

			else if(type[m].flag[5] && (type[m-1].flag[5] || type[m+1].flag[5]))
			{
				printf("\n... %s is  pronoun*\n",type[m].input);
			}

			else if(type[m].flag[7] && (type[m-1].flag[7] || type[m+1].flag[7]))
			{
				printf("\n... %s is  preposition*\n",type[m].input);
			}
			else if(type[m].flag[8] && (type[m-1].flag[8] || type[m+1].flag[8]))
			{
				printf("\n... %s is  conjunction*\n",type[m].input);
			}

			if(flag==0)
			{
				printf("\ncan't guess -----%s it can be ",type[m].input);
				for(i=0;i<=9;i++)
				{
					if(type[m].flag[i])
					printf(",%s",speech_parts[i]);
				}
			}
			printf("\n");

		}


	}
}

