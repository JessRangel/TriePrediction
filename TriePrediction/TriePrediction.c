/****************************************************************

 <br> Subject:            Trie
 <br> Author:             Jessica Rangel
 <br> Creation Date:      Nov 26, 2020
 <br> Last Modification:  Nov 28, 2020

 ****************************************************************/

// header file
#include "TriePrediction.h"

// libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// constants
#define MAX_CHARACTERS (MAX_CHARACTERS_PER_WORD * MAX_WORDS_PER_LINE)


/****************************************************************
* Prototypes
*/
TrieNode* createTrieNode                  (void);

void      destroyTrieNode                 (TrieNode* node);

void      printTrie                       (TrieNode* root);

void      printTrieNode                   (TrieNode* node, char* tabString);

void      printTrieSimpleFormat           (TrieNode* root);

void      printTrieNodeWordsSimpleFormat  (TrieNode* node, char* word);

bool      hasChildren                     (TrieNode* node);

int       getNumberOfChildren             (TrieNode* node);

TrieNode* getTrieNode                     (TrieNode* root, char* word);

void      insertPhrase                    (TrieNode* root, char* phrase);

int       getIndex                        (char letter);

TrieNode* insertWord                      (TrieNode* node, char* word);

void      runFileCommands                 (TrieNode* root, char* filename);

void      eventCommand1                   (TrieNode* root);

void      eventCommand2                   (TrieNode* root, char* phrase);

void      eventCommand3                   (TrieNode* root, char* word);

void      getTrieWords                    (TrieNode* root, char* listOfWords);

void      getTrieNodeWords                (TrieNode* node, char* listOfWords, char* word);

TrieNode* getMostFrequentWord             (TrieNode* node);

void      stripPunctuators                (char* string);

void      getTextPrediction               (TrieNode* root, TrieNode* node, int counter);

/****************************************************************
 * Main function to start the application.
 *
 * @param		numberOfArguments		number of arguments used to run the application
 * @param		arguments						array of arguments used to run the application
 *
 * @return	int									program exit status
 */
int main (int numberOfArguments, char* arguments[])
{
	// variables
  TrieNode* root;         // root of the trie
  char*     filename1;    // name of the file with the words for the trie
  char*     filename2;    // name of the file with the commands

  // consistency
  if(numberOfArguments < 3)
  {
    printf("Missing arguments!\n");
    printf("Trie requires a text file to function properly.\n");
  }

  filename1 = arguments[1];
  filename2 = arguments[2];

  // creates trie from specified file
  root = buildTrie(filename1);

  // runs command from input file
  runFileCommands(root, filename2);

  // deallocates memory
  destroyTrie(root);

	// indicates that the program is closing without any problem
	return 0;
}

/****************************************************************
 * Builds trie root.
 *
 * @param		filenname		  name of the file with words for creation of the trie
 *
 * @return	TrieNode*     root of the new trie
 */
TrieNode* buildTrie (char* filename)
{
  TrieNode* root;                     // root of the trie
  FILE*     file;                     // file with the words
  char      phrase[MAX_CHARACTERS];   // string with words

  // consistency
  if (filename == NULL)
    return NULL;

  // opens file
  file = fopen(filename, "r");

  // consistency
  if (file == NULL)
  {
    printf("\nError: Unable to open file %s.\n\n", filename);
    return NULL;
  }

   // creates root
  root = createTrieNode();

  // reads file line-by-line
  while (fgets(phrase, MAX_CHARACTERS, file) != NULL)
  {
    // inserts phrase into trie
    insertPhrase(root, phrase);
  }

  // closes file
  fclose(file);

  // returns trie root
  return root;
}

/****************************************************************
 * Creates and initializes trie node.
 *
 * @return	TrieNode*     new trie node
 */
TrieNode* createTrieNode (void)
{
  TrieNode* node;   // node of the trie

  // Allocates and initializes root and children to null
  node = calloc (1, sizeof(TrieNode));

  return node;
}

/****************************************************************
 * Destroys trie root.
 *
 * @param		root		      root of the trie to be deleted with all children
 *
 * @return	TrieNode*     root of the trie
*/
TrieNode* destroyTrie (TrieNode* root)
{
  // consistency
  if (root == NULL)
    return NULL;

  // recursively destroys children nodes
  for (int i = 0; i < 26; i++)
    destroyTrie(root->children[i]);

  // frees subtrie pointer
  destroyTrie(root->subtrie);

  // frees top level pointer
  free(root);

  return NULL;

}

/****************************************************************
 * Prints all trie contents on screen.
 *
 * @param		root		      root of the trie
 */
void printTrie (TrieNode* root)
{
  int numberOfChildren;   // number of children of the root

  // consistency
  if (root == NULL)
    return;

  // gets number of children
  numberOfChildren = getNumberOfChildren(root);

  printf("root: \n");
  printf("count.............: %d \n", root->count);
  printf("number of children: %d \n", numberOfChildren);

  if (numberOfChildren > 0)
  {
    printf("children..........: \n");

    for (int i = 0; i < 26; i++)
    {
      if (root->children[i] != NULL)
      {
        printf("  child index.......: %d \n", i);
        printf("  letter............: %c \n", ('a' + i));
        printTrieNode(root->children[i], " ");
      }
    }
  }
  printf("subtrie...........: %p \n", root->subtrie);
}

/****************************************************************
 * Auxiliary function. Prints trie node and its subnodes on screen.
 *
 * @param		node		      node of the trie
 * @param		tabString		  string with tab characters
 */
void printTrieNode (TrieNode* node, char* tabString)
{
  int   numberOfChildren;                       // number of children of the node
  char  string[2 * MAX_CHARACTERS_PER_WORD];    // string with word and tab

  // consistency
  if (node == NULL)
    return;

  // gets number of children
  numberOfChildren = getNumberOfChildren(node);

  strcpy(string, tabString);
  strcat(string, " ");

  printf(string);
  printf("count.............: %d \n", node->count);

  printf(string);
  printf("number of children: %d \n", numberOfChildren);

  if (numberOfChildren > 0)
  {
    printf(string);
    printf("children: \n");

    for (int i = 0; i < 26; i++)
    {
      if (node->children[i] != NULL)
      {
        printf(string);
        printf(" child index.......: %d \n", i);

        printf(string);
        printf(" letter............: %c \n", ('a' + i));

        printTrieNode(node->children[i], string);
      }
    }
  }

  printf(string);
  if (node->subtrie != NULL)
  {
    printf("subtrie...........: %p \n", node->subtrie);
    printTrieNode(node->subtrie, string);
  }
  else
  {
    printf("subtrie...........: none \n");
  }

}

/****************************************************************
 * Prints all trie contents on screen.
 *
 * @param		root		      root of the trie
 */
void printTrieSimpleFormat (TrieNode* root)
{
   /*
    Creates string called rootWord to hold first letter of the word.
    Cycles recursively through nodes to append the letters into the word string.
    Once the count is greater than zero, which indicates end-of-word,
    that is when the word and its count are printed on screen.

    Since each node index represents a letter, it converts that index
    into a char to be appended to the word string.
 */

  int   numberOfChildren;                     // number of children of root node
  char  rootWord[MAX_CHARACTERS_PER_WORD];    // word to be completed by node content
  char  letter[2];                            // first letter of the word

  // consistency
  if (root == NULL)
    return;

  // gets number of children
  numberOfChildren = getNumberOfChildren(root);

  // if a child exists, cycles pointer through children
  if (numberOfChildren > 0)
  {
    for (int i = 0; i < 26; i++)
    {
      if (root->children[i] != NULL)
      {
        // takes first letter of word and creates a string
        letter[0] = 'a' + i;
        letter[1] = '\0';

        // inserts first letter into word string
        strcpy(rootWord, letter);

        // recursive call into child nodes
        printTrieNodeWordsSimpleFormat(root->children[i], rootWord);
      }
    }
  }
}

/****************************************************************
 * Auxiliary function. Prints trie node words on screen.
 *
 * @param		node		      node of the trie
 * @param		word		      buffer to store the letters of the word, through the nodes
 */
void printTrieNodeWordsSimpleFormat (TrieNode* node, char* word)
{
  int   numberOfChildren;                     // number of children of this node
  char  nodeWord[MAX_CHARACTERS_PER_WORD],    // word from this node forward
        letter[2];                            // letter corresponding to this node

  // consistency
  if (node == NULL)
    return;

  // when the count is greater than zero, it has reached the end of a word
  if (node->count > 0)
    printf("%s (%d)\n", word, node->count);

  // gets number of children
  numberOfChildren = getNumberOfChildren(node);

  // if a child exists, cycles through nodes to append letters to the word
  if (numberOfChildren > 0)
  {
    for (int i = 0; i < 26; i++)
    {
      if (node->children[i] != NULL)
      {
        // current letter
        letter[0] = 'a' + i;
        letter[1] = '\0';

        // copies received partial word into final word string
        strcpy(nodeWord, word);

        // appends next letter into final word string
        strcat(nodeWord, letter);

        // recursive call to print child nodes
        printTrieNodeWordsSimpleFormat(node->children[i], nodeWord);
      }
    }
  }
}

/****************************************************************
 * Checks if node has children.
 *
 * @param		node		      node of the trie
 *
 * @return  bool          true if has children; otherwise, false
 */
bool hasChildren (TrieNode* node)
{
  // consistency
  if (node == NULL)
    return false;

  // checks if there is at least one child
  for (int i = 0; i < 26; i++)
  {
    // returns true
    if (node->children[i] != NULL)
      return true;
  }

  // returns false
  return false;
}

/****************************************************************
 * Gets number of child nodes.
 *
 * @param		node		      node of the trie
 *
 * @return  int           number of children
 */
int getNumberOfChildren (TrieNode* node)
{
  int numberOfChildren = 0;   // number of children of the node

  // consistency
  if (node == NULL)
    return 0;

  // traverses the array of children
  for (int i = 0; i < 26; i++)
  {
    // if it is not null, increments counter
    if (node->children[i] != NULL)
      numberOfChildren++;
  }

  // returns counter
  return numberOfChildren;
}

/****************************************************************
 * Gets node.
 *
 * @param		root		      root of the trie
 * @param		word		      string with the word to be found
 *
 * @return  TrieNode*     node of the trie
 */
TrieNode* getTrieNode (TrieNode* root, char* word)
{
  TrieNode* node = NULL;    // node of the trie
  int       length,         // number of letters of the word
            index;          // index of the node

  // consistency
  if ((root == NULL) || (word == NULL))
    return NULL;

  // trie only accepts lowercase
  strlwr(word);

  // removes punctuation marks if they exist
  stripPunctuators(word);

  // gets length
  length = strlen(word);

  // consistency
  if (length == 0)
    return NULL;

  // starts search at root
  node = root;

  // cycles through children
  for (int i = 0; i < length; i++)
  {
    // searches for next letter
    index = word[i]-'a';

    // if the pointer is at the last non-null node, it found the word
    if (node->children[index] != NULL)
    {
      node = node->children[index];
    }
    else
    {
      return NULL;
    }
  }

  return node;
}

/****************************************************************
 * Inserts phrase into trie root.
 *
 * @param		root		      root of the trie
 * @param		phrase		    string with words
 */
void insertPhrase (TrieNode* root, char* phrase)
{
  TrieNode* previousWordNode = NULL;          // previous node of the word
  int       length,                           // number of letters of the word
            index2 = 0;                       // index of a letter of the word
  char      word[MAX_CHARACTERS_PER_WORD],    // word
            delimiter = ' ';                  // separator of words

  // consistency
  if ((root == NULL) || (phrase == NULL))
    return;

  // calculates phrase length
  length = strlen(phrase);

  // consistency
  if (length == 0)
    return;

  // transforms phrase into lowercase
  strlwr(phrase);

  // removes punctuation from phrase
  stripPunctuators(phrase);

  // traverses characters of phrase
  for (int index1 = 0; index1 <= strlen(phrase); index1++)
  {
     // checks if it is a valid letter
     if ((phrase[index1] != delimiter) && (phrase[index1] != '\0'))
     {
       // adds letter to word
       word[index2] = phrase[index1];

       // increments index
       index2++;
     }
     else
     {
       // finalizes word string
       word[index2] = '\0';

       // inserts word into previous word subtrie
       if (previousWordNode != NULL)
       {
          if (previousWordNode->subtrie == NULL)
            previousWordNode->subtrie = createTrieNode();

          insertWord(previousWordNode->subtrie, word);
       }

       // recursively inserts word into root
       previousWordNode = insertWord(root, word);

       // resets index
       index2 = 0;
     }
  }
}
/****************************************************************
 * Calculates character index.
 *
 * @param		letter		    letter
 * @param		index		      index related to the letter, from 0 to 25
 */
int getIndex (char letter)
{
  // trie only accepts lowercase
  int index = tolower(letter) - 'a';    // index of a node, from 0 to 25

  return index;
}

/****************************************************************
 * Inserts a word into trie node. Returns last node.
 *
 * @param		node		      node of the trie
 * @param		word		      word to be inserted in the trie
 *
 * @return  TrieNode*     node of the trie that contains the last letter of the word
 */
TrieNode* insertWord (TrieNode* node, char* word)
{
  TrieNode* child;    // child of the node
  int       length;   // number of letters of the word
  char      letter;   // letter

  // consistency
  if ((node == NULL) || (word == NULL))
    return NULL;

  // gets length
  length = strlen(word);

  // consistency
  if (length == 0)
    return NULL;

  // gets first letter
  letter = word[0];

  // checks if letter already exists
  if (node->children[getIndex(letter)] == NULL)
  {
    // creates node
    child = createTrieNode();

    // links root and child
    node->children[getIndex(letter)] = child;
  }
  else
  {
    // we get the existing child
    child = node->children[getIndex(letter)];
  }

  // checks if there are more characters
  if (length > 1)
  {
    char substring[length-1];

    // creates substring without already inserted letter
    strcpy(substring, word +1);

    // recursive call to add next letter
    return insertWord(child, substring);
  }
  // length = 1 is the last letter
  else
  {
    // increments word count
    child->count++;

    // returns last node of the word
    return child;
  }
}

/****************************************************************
 * Receives and runs command from file.
 *
 * @param		root		      root of the trie
 * @param		filename		  name of the file with the commands
 */
void runFileCommands (TrieNode* root, char* filename)
{
  FILE* file;                       // file with commands
  char  command[MAX_CHARACTERS];    // command
  int   length;                     // number of letters

  if ((root == NULL) || (filename == NULL))
    return;

  // opens file
  file = fopen(filename, "r");

  // consistency
  if (file == NULL)
  {
    printf("\nError: Unable to open file %s.\n\n", filename);
    return;
  }

  // reads file line-by-line
  while (fgets(command, MAX_CHARACTERS, file) != NULL)
  {
    length = strlen(command);

    // checks if a command was received
    if (length > 0 )
    {
      if (command[0] == '!')
      {
        // passes root of trie to be printed
        eventCommand1(root);
      }
      else if (command[0] == '@')
      {
        // passes command without @: Contains string and number
        eventCommand2(root, command+2);

        // fixes display for multiple calls to text prediction command
        printf("\n");
      }
      else
      {
        // passes string to be searched
        eventCommand3(root, command);
      }
    }
  }

  // closes file
  fclose(file);
}

/****************************************************************
 * Executes command to print trie.
 *
 * @param		root		      root of the trie
 */
void eventCommand1 (TrieNode* root)
{
  // prints trie with defined format
  printTrieSimpleFormat(root);
}

/****************************************************************
 * Executes command for text prediction in trie.
 *
 * @param		root	        root of the trie
 * @param		phrase		    string with words
 */
void eventCommand2 (TrieNode* root, char* phrase)
{
  TrieNode* node = NULL;                      // node of the trie
  int       numberOfWords,                    // number of words
            wordIndex = 0,                    // index of the word
            numberIndex = 0;                  // index of the number
  char      word[MAX_CHARACTERS_PER_WORD],    // word of the command
            number[MAX_CHARACTERS_PER_WORD],  // number of the command
            delimiter;                        // separator of words

  // consistency
  if ((root == NULL) || (phrase == NULL))
    return;

  // initialize strings
  strcpy(word, "");
  strcpy(number, "");

  delimiter = ' ';

  // removes word and number from phrase
  for (int i = 0; i < strlen(phrase); i++)
  {
    if (isalpha(phrase[i]) && phrase[i] != delimiter)
    {
      word[wordIndex] = phrase[i];
      wordIndex++;
    }
    else if (isdigit(phrase[i]) && phrase[i] != delimiter)
    {
      number[numberIndex] = phrase[i];
      numberIndex++;
    }
  }

  // adding end-of-line character
  word[wordIndex] = '\0';
  number[numberIndex] = '\0';

  // converting string to integer
  numberOfWords = atoi(number);

  printf("%s", word);

  // gets node with given word
  node = getTrieNode(root, word);

  // consistency
  if (node == NULL)
    return;

  else if (node->subtrie == NULL)
    return;

  else
    getTextPrediction (root, node, numberOfWords);
}

/****************************************************************
 * Executes command for word search in trie.
 *
 * @param		root	        root of the trie
 * @param		word		      string with word
 */
void eventCommand3 (TrieNode* root, char* word)
{
  TrieNode* node = NULL;    // node of the trie

  // consistency
  if ((root == NULL) || (word == NULL))
    return;

  // prints received string command on screen
  printf("%s", word);

  // searches for word in trie
  node = getTrieNode(root, word);

  // checks if word was found in root trie. If not, exit
  if (node == NULL)
  {
    printf("(INVALID STRING)\n");

    // if node is null, it has no subtrie
    return;
  }

  // checks if word has a subtrie
  if (node->subtrie == NULL)
    printf("(EMPTY)\n");

  // uses recursive call with hyphen as root
  else
    printTrieNodeWordsSimpleFormat(node->subtrie, "- ");
}

/****************************************************************
 * Gets a list of all words in a node subtrie.
 *
 * @param		root	        root of the trie
 * @param		listOfWords   string to be filled with the words, separated by whitespace
 */
void getTrieWords (TrieNode* root, char* listOfWords)
{
  int   numberOfChildren;                     // number of children of root node
  char  rootWord[MAX_CHARACTERS_PER_WORD],    // word to be completed by node content
        letter[2];                            // first letter of the word

  // consistency
  if (root == NULL)
    return;

  // gets number of children
  numberOfChildren = getNumberOfChildren(root);

  // if a child exists, cycles pointer through children
  if (numberOfChildren > 0)
  {

    for (int i = 0; i < 26; i++)
    {
      if (root->children[i] != NULL)
      {
        // takes first letter of word and creates a string
        letter[0] = 'a' + i;
        letter[1] = '\0';

        // inserts first letter into word string
        strcpy(rootWord, letter);

        // recursive call into child nodes
        getTrieNodeWords(root->children[i], listOfWords, rootWord);
      }
    }
  }
}

/****************************************************************
 * Auxiliary function. Gets a list of all words in a node subtrie.
 *
 * @param		node	        node of the trie
 * @param		listOfWords   string to be filled with the words, separated by whitespace
 * @param		word          string to be filled with letters of the word
 */
void getTrieNodeWords (TrieNode* node, char* listOfWords, char* word)
{
  int   numberOfChildren;                     // number of children of this node
  char  nodeWord[MAX_CHARACTERS_PER_WORD],    // word from this node forward
        letter[2];                            // letter corresponding to this node

  // consistency
  if (node == NULL)
    return;

  // when the count is greater than zero, it has reached the end of a word
  if (node->count > 0)
  {
    strcat(listOfWords, word);
    strcat(listOfWords, " ");
  }

  // gets number of children
  numberOfChildren = getNumberOfChildren(node);

  // if a child exists, cycles through nodes to append letters to the word
  if (numberOfChildren > 0)
  {
    for (int i = 0; i < 26; i++)
    {
      if (node->children[i] != NULL)
      {
        // current letter
        letter[0] = 'a' + i;
        letter[1] = '\0';

        // copies received partial word into final word string
        strcpy(nodeWord, word);

        // appends next letter into final word string
        strcat(nodeWord, letter);

        // recursive call to print child nodes
        getTrieNodeWords(node->children[i], listOfWords, nodeWord);
      }
    }
  }
}


/****************************************************************
 * ...
 * @param		node	        node of the trie
 *
 * @return  TrieNode*     node with the most frequent word
 */
void getTextPrediction (TrieNode* root, TrieNode* node, int counter)
{
  int       wordIndex = 0,                             // index of the word
            maxFrequency = 0;                          // count of most frequent word
  char      word[MAX_CHARACTERS_PER_WORD],             // word of the command
            mostFrequentWord[MAX_CHARACTERS_PER_WORD], // word of the command
            listOfWords[MAX_CHARACTERS],               // list of all subtrie words
            delimiter;                                 // separator of words
  TrieNode* nextWord = NULL;


  // consistency
  if ((node == NULL) || (counter == 0))
    return;

  // sets space as delimiter
  delimiter = ' ';

  // initialize strings
  strcpy(listOfWords, "");

  // resets word index
  wordIndex = 0;

  // gets list of words in subtrie
  getTrieWords(node->subtrie, listOfWords);

  // searches most frequent word from list of words
  // reads word by word from list of words
  for (int i = 0; i < strlen(listOfWords); i++)
  {
    // gets word, using space and EOL as delimiters
    if ((listOfWords[i] != delimiter) && listOfWords[i] != '\0')
    {
      word[wordIndex] = listOfWords[i];
      wordIndex++;
    }
    // if a space or EOL is found, finalize string
    else if ((listOfWords[i] == delimiter) || listOfWords[i] == '\0')
    {
      word[wordIndex] = '\0';

      if (getTrieNode(node->subtrie, word) != NULL)
      {
        if (maxFrequency < getTrieNode(node->subtrie, word)->count)
        {
          maxFrequency = getTrieNode(node->subtrie, word)->count;
          strcpy(mostFrequentWord, word);
        }
      }
      strcpy(word, "");
      wordIndex = 0;
    }
  }

  // prints word on screen
  printf(" %s", mostFrequentWord);

  // gets next word from root
  nextWord = getTrieNode(root, mostFrequentWord);

  // consistency
  if (nextWord == NULL)
    return;

  else if (nextWord->subtrie == NULL)
    return;

  else getTextPrediction(root, nextWord, counter-1);
}

/****************************************************************
 * Removes punctuation marks from string.
 *
 * @param		string	      string with letters and punctuators
 */
void stripPunctuators (char* string)
{
  int   index = 0;                    // index
  char  newString[strlen(string)];    // new string without punctuators

  // consistency
  if (string == NULL)
    return;

  // traverses string
  for (int counter = 0; counter < strlen(string); counter++)
  {
    // If the char is alphabetic, copy into new string
    if(isalpha(string[counter]) || (string[counter]==' '))
    {
      // transforms char into lowercase
      newString[index] = tolower(string[counter]);
      index++;
    }
  }

  // adds end-of-line character
  newString[index] = '\0';

  // copies new string into original string
  strcpy(string, newString);
}

/****************************************************************
 * Returns the difficulty rating of this project
 *
 * @return  double        difficulty rating of the project
 */
double difficultyRating (void)
{
  return 5;
}

/****************************************************************
 * Returns the number of hours spent doing this project.
 *
 * @return  double        hours spent doing this project
 */
double hoursSpent (void)
{
  return 18.0;
}

