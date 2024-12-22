/*-------------------------------------------
Program 3:  Bit Big Bug Tug - Word Ladder Builder
Description: Builds a word ladder game using command-line and interactive inputs
System: Zybooks IDE
Course: CS 211, Fall 2024, UIC
Author: Yousuf Hussain
-------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Compares two words and returns the number of differing characters.
int strCmpCnt(char* word1, char* word2) {
    int count = 0; // Counter to keep track
    // Loop through until one of the words reaches null character
    while (*word1 != '\0' || *word2 != '\0') {
        // If one character has a difference increment count
        if (*word1 != *word2) {
            count++;
        }
        // Move to next character
        if (*word1 != '\0') {
            word1++;
        }
        // Move to next character
        if (*word2 != '\0') {
            word2++;
        }
    
    }
    
    return count;
}

// Compares two words and returns the index of the first differing character.
int strCmpInd(char* word1, char* word2) {
    
    int idx = 0; // Counter to keep track
    // // Loop through until both of the words reaches null character
    while (*word1 != '\0' && *word2 != '\0') {
        // Return index of the first difference
        if (*word1 != *word2) {
            return idx;
        }
        // Increment everything
        word1++;
        word2++;
        idx++;
    }

    // If one string is longer, return the first index where the extra characters start
    if (*word1 != '\0' || *word2 != '\0') {
        return idx;
    }

    // Else return -1
    return -1;
   
}

// Appends a new word to the dynamic array of words, expanding the array if needed.
void appendWord(char*** pWords, int* pNumWords, int* pMaxWords, char* newWord) {
    // Checks if the current number of words is equal to the capacity (*pMaxWords)
    if (*pNumWords == *pMaxWords) {
        // Allocates a new array with double the size of *pMaxWords
        char** temp = (char**)malloc(sizeof(char*) * (*pMaxWords * 2));

        // Copies the pointers to the words from the old array to the new array
        for (int i = 0; i < *pNumWords; i++) {
            temp[i] = (*pWords)[i];
        }

        free(*pWords);
        // Makes *pWords point to the double-capacity block of memory
        (*pWords) = temp;
        // Doubles the capacity since we already allocated more capacity
        *pMaxWords *= 2;
    }
    
    // Allocates space for the new word including the null character with the +1
    (*pWords)[*pNumWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char));
    
    // Copies newWord and puts it into the allocated memory
    strcpy((*pWords)[*pNumWords], newWord);
    // Increments *pNumWords since we added a new word
    (*pNumWords)++;

    
}

// Performs a linear search for a word in the array of words 
// And returns the index of the word if found, or -99 if not found
int linSearchForWord(char** words, int numWords, char* findWord) { 
    for (int i = 0; i < numWords; i++) {
        if (strcmp(words[i], findWord) == 0) {
            return i; // Return index if found
        }
    }
    return -99; // // Return -99 if not found
}

// Validates if aWord can be added to the ladder by checking multiple questions
bool checkForValidWord(char** words, int numWords, int wordLen, char** ladder, int ladderHeight, char* aWord) {
        // Case 1: User wants to stop
        if (strcmp(aWord, "DONE") == 0) {
            printf("Stopping with an incomplete word ladder...\n");
            return true;
        }

        // Case 2: Incorrect word length
        if (strlen(aWord) != wordLen) {
            printf("Entered word does NOT have the correct length. Try again...\n");
            return false;
        }

        // Case 3: Word not in dictionary
        if (linSearchForWord(words, numWords, aWord) == -99) {
            printf("Entered word NOT in dictionary. Try again...\n");
            return false;    
        }

        // Case 4: Not a one-character difference
        char* previousWord = ladder[ladderHeight - 1];
        if (strCmpCnt(previousWord, aWord) != 1) {
            printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
            return false;
        }

        // Word is valid
        printf("Entered word is valid and will be added to the word ladder.\n");
        return true;

}

// Checks if the last word in the ladder matches the final word, indicating the ladder is complete.
bool isLadderComplete(char** ladderWords, int height, char* finalWord) {

    char* lastWord = ladderWords[height - 1];
    // Compare the last word in the ladder to the final word
    if (strcmp(lastWord, finalWord) == 0) {
        return true;;
    }
    else {
        return false;
    }
}


// Displays the incomplete word ladder with three dots at the top to indicate it's incomplete.
void displayIncompleteLadder(char** ladderWords, int height) {
    printf("  ...\n");
    printf("  ...\n");
    printf("  ...\n");
    // Print the ladder from bottom to top
    for (int i = height - 1; i >= 0; i--) {
        printf("  %s\n", ladderWords[i]);
    }
}

// Displays the complete word ladder with the character change ('^') between each rung.
void displayCompleteLadder(char** ladderWords, int height) {
    // Loop through the ladder and display each word with '^' indicating character change
    for (int i = height - 1; i >= 0; i--) {
        printf("  %s\n", ladderWords[i]); 
        if (i > 0) {
            int diffIdx = strCmpInd(ladderWords[i], ladderWords[i-1]);

            // Display the '^' aligned with the differing character
            printf(" ");
            for (int j = 0; j < diffIdx; j++) {
                printf(" ");
            }
            printf(" ^");
            for (int k = strlen(ladderWords[i]) - 1; k > diffIdx; k--) {
                printf(" ");
            }
            printf("\n");
            

        }
        
    }
}


int main(int argc, char* argv[]) {

    printf("\nProcessing command-line arguments...\n");

    int wordLen = 0;
    int maxLadder = 1;
    char dict[100] = "notAfile";     
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid"; 

    // Parsing command-line arguments
    for (int i = 1; i < argc; i++) {
        if ((argv[i][0] == '-')){
            switch (argv[i][1]) {
                case 'n': // Word Length
                    if (i + 1 < argc) {
                        wordLen = atoi(argv[i+1]);
                    }
                    break;
                case 'm': // Max ladder height
                    if (i + 1 < argc) {
                        maxLadder = atoi(argv[i+1]);
                    }
                    break;
                case 'd': // Dictionary file (string)
                    if (i + 1 < argc) {
                        strcpy(dict, argv[i+1]);
                    }
                    break;
                case 's': // Start word (string)
                    if (i + 1 < argc) {
                        strcpy(startWord, argv[i+1]);
                    }
                    break;
                case 'f': // Final word 
                    if (i + 1 < argc) {
                        strcpy(finalWord, argv[i+1]);
                    }
                    break;
            }
        }

    }

    printf("Interactively (re)setting invalid parameters:\n");

    // set word length using interactive user-input if necessary
    int numInputs = 1;
    while (numInputs > 0 && (wordLen < 2 || wordLen > 20)) {
        printf("Invalid word length = %d\n", wordLen);
        printf("Enter a valid word length (must be 2-20): ");
        numInputs = scanf("%d",&wordLen);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }


    // set max ladder height using interactive user-input if necessary
    numInputs = 1;
    while (numInputs > 0 && maxLadder < 2) {
        printf("Invalid max ladder height = %d\n", maxLadder);
        printf("Enter a valid max ladder height (must be >1): ");
        numInputs = scanf("%d",&maxLadder);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    FILE* fileTry = fopen(dict,"r");
    numInputs = 1;
    while (numInputs > 0 && fileTry == NULL) {
        printf("Dictionary file %s not found...\n", dict);
        printf("Enter filename for dictionary: ");
        numInputs = scanf("%s", dict);
        printf("\n");
        fileTry = fopen(dict,"r");
    }
    fclose(fileTry);
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }
    
    // build the [words] array, a heap-allocated array of C-strings
    // where the words are a read-in from the dictionary file
    // and only those words of the desired length [wordLen] 

    int numWords = 0; // initially, no words in the array
    int maxWords = 4; // initially, capacity = 4

    char** words = (char**)malloc(maxWords*sizeof(char*));

    FILE* myFile = NULL;
    myFile = fopen(dict, "r");
    if (myFile == NULL) {
        printf("Could not open file %s.txt\n", dict);
        return -1;
    }

    char buffer[100];
    while (fscanf(myFile, "%s", buffer) != EOF) {
        if (strlen(buffer) == wordLen) {
            appendWord(&words, &numWords, &maxWords, buffer);
        }
    }
    
    fclose(myFile);

    printf("The dictionary contains %d words of length %d.\n",numWords,wordLen);
    printf("Max size of the dynamic words array is %d.\n",maxWords);
    printf("\n");

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("Dictionary %s contains insufficient %d-letter words...\n",dict,wordLen);
        printf("Terminating program...\n");
        return -1;
    }

    // resetting start word using interactive user-input if necessary
    numInputs = 1;
    while (numInputs > 0 && linSearchForWord(words,numWords,startWord) < 0) {
        printf("Start word %s is not valid...\n", startWord);
        printf("Enter a valid start word: ");
        numInputs = scanf("%s", startWord);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }

    // resetting final word using interactive user-input if necessary
    numInputs = 1;
    while (numInputs > 0 && linSearchForWord(words,numWords,finalWord) < 0 ) {
        printf("Final word %s is not valid...\n", finalWord);
        printf("Enter a valid final word: ");
        numInputs = scanf("%s", finalWord);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }


    printf("\nWelcome to the CS 211 Word Ladder Builder!\n");
    printf("\n"); 

    printf("Your goal is to make a word ladder between two ");
    printf("%d-letter words: \n  %s -> %s\n\n",wordLen, startWord,finalWord);  
    
    // Allocating the word ladder for the maximum allowed height
    char** ladder = (char**)malloc(maxLadder*sizeof(char*));

    int ladderHeight = 0; // initially, the ladder is empty
    
    // add the start word to the ladder, i.e. the "bottom rung"
    appendWord(&ladder,&ladderHeight,&maxLadder,startWord);
    
    char aWord[30] = "XYZ";
    printf("\n");


    while (ladderHeight < maxLadder && strcmp(aWord, "DONE") != 0 && !isLadderComplete(ladder, ladderHeight, finalWord)) {   // modify this line 
        printf("The goal is to reach the final word: %s\n",finalWord);
        printf("The ladder is currently: \n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Current ladder height: %d\n",ladderHeight);
        printf("Maximum ladder height: %d\n",maxLadder);
        printf("Enter the next word (or DONE to stop): ");
        scanf("%s",aWord);
        printf("\n");

        // Make sure the entered word is valid for the next ladder rung;
        // if not, repeatedly allow user to enter another word until one is valid
        while (!checkForValidWord(words, numWords, wordLen, ladder, ladderHeight, aWord)) {
            printf("Enter another word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
        }

        // add the entered word to the ladder (unless it is "DONE")
        if (strcmp(aWord,"DONE") != 0) {
            appendWord(&ladder,&ladderHeight,&maxLadder,aWord);
        }
        printf("\n");

    }
    

    // Check if the built word ladder is complete and 
    // display the word ladder appropriately. 
    if (isLadderComplete(ladder, ladderHeight, finalWord)) {
        printf("Word Ladder complete!\n");
        displayCompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you find a shorter Word Ladder next time??? \n");
    } else {
        if (ladderHeight == maxLadder) printf("The Word Ladder reached the maximum height.\n");
        printf("The final Word Ladder is incomplete:\n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you complete the Word Ladder next time??? \n");
    }
    
    // Free dynamically allocated memory to avoid memory leaks

    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
    for (int i = 0; i < ladderHeight; i++) {
        free(ladder[i]);
    }
    free(ladder);
    return 0;
}