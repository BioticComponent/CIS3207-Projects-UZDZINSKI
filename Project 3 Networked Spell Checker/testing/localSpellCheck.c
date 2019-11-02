//joe uzdzinski

//a local spell check program with no network capabilities
//gets word from standard input (keyboard) and responds with found or not found

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    
    char checkWord[50];
    char dictWord[50];
    int found = 0;
    int running = 1;
    
    while(running) {
        //get word
        printf("enter word: ");
        scanf(" %[^\n]", checkWord);
        //if user presses esc, program terminates
        if (checkWord[0] == 27) {
            printf("program terminated\n");
            running = 0;
        } else {
            FILE* fp = fopen("words.txt", "r");
            found = 0;
            while(fgets(dictWord, 50, fp)) {
                //make checked and dictionary word lowercase
                for(int i = 0; checkWord[i]; i++){
                    checkWord[i] = tolower(checkWord[i]);
                }
                int j = 0;
                for(j = 0; dictWord[j]; j++){
                    dictWord[j] = tolower(dictWord[j]);
                }
                //remove new line character from dictionary word
                dictWord[j - 1] = '\0';
                
                //compare words
                if (strcmp(checkWord,dictWord) == 0) {
                    printf("found\n");
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                printf("not found\n");
            }
            fclose(fp);
        }
    }
}