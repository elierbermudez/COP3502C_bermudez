//Elier Bermudez, EEL3801, Project 2 C Code

#include <stdio.h>
#include <stdlib.h>
int findCount(char *input_testSentence, char input_lowercaseVersion, char input_uppercaseVersion){
    int i=0;
    int count=0;
    while(input_testSentence[i] != '\0'){
        //If the lowercase or uppercase version is found, increment count.
        if(input_testSentence[i] == input_lowercaseVersion)
        {
            count++;
        }
        else if(input_testSentence[i] == input_uppercaseVersion)
        {
            count++;
        }

        i++;
    }

    return count;
}

int printHistogram(int count){
    int i=0;
    for(i=0; i<count; i++){
        printf("#");
    }
    printf("\n");
}
int main()
{
    char testSentence[500];
    int letterCounts[8];
    int i, count;

    //Take in user input
    printf("Enter any sentence: ");
    gets(testSentence);

    //Find the letter counts
    letterCounts[0] = findCount(testSentence, 'c', 'C');
    letterCounts[1] = findCount(testSentence, 'h', 'H');
    letterCounts[2] = findCount(testSentence, 'a', 'A');
    letterCounts[3] = findCount(testSentence, 'r', 'R');
    letterCounts[4] = findCount(testSentence, 'g', 'G');
    letterCounts[5] = findCount(testSentence, 'e', 'E');
    letterCounts[6] = findCount(testSentence, 'o', 'O');
    letterCounts[7] = findCount(testSentence, 'n', 'N');

    //Print the letter counts
    printf("C: %d\n", letterCounts[0]);
    printf("H: %d\n", letterCounts[1]);
    printf("A: %d\n", letterCounts[2]);
    printf("R: %d\n", letterCounts[3]);
    printf("G: %d\n", letterCounts[4]);
    printf("E: %d\n", letterCounts[5]);
    printf("O: %d\n", letterCounts[6]);
    printf("N: %d\n\n", letterCounts[7]);

    //Print the histograms
    printf("C:");
    printHistogram(letterCounts[0]);
    printf("H:");
    printHistogram(letterCounts[1]);
    printf("A:");
    printHistogram(letterCounts[2]);
    printf("R:");
    printHistogram(letterCounts[3]);
    printf("G:");
    printHistogram(letterCounts[4]);
    printf("E:");
    printHistogram(letterCounts[5]);
    printf("O:");
    printHistogram(letterCounts[6]);
    printf("N:");
    printHistogram(letterCounts[7]);
    return 0;
}
