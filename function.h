#include <stdbool.h>
struct trie
{
        char word;			//alphabet which is used to be compared while searching
        struct trie *child;		//to point bottom node ie child
        struct trie *hori;		//to point in horizontal direction of parent
};

// Structure in which we store the input from the user and store the part of speech in which the word lies
struct storage
{
	char input[50];		//to store the input taken from the user
	bool flag[9];		//to store the type of part of speech
};

typedef struct storage pos; 	//changing name of structure storage to pos
typedef struct trie node;	//changing name of structure trie to node
int lines_no(char *file);
void file_search();
void user_input();
void secondary_search();

