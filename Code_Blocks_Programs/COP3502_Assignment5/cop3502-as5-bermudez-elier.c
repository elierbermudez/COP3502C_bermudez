
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	"leak_detector_c.h"
//Elier Bermudez, COP3502, Assignment 1


void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    // t begins at the null sentinel at the end of s.

    t--;

    /* t is now at the last character of s - unless s didn't contain any characters, in which
       case, t is now *BEFORE* s.  We have to keep checking for that. */

    /* We repeat until EITHER t slides to the left of s, OR we find a character that is not a
       line feed (\n) or a carriage return (\r). */

    while ((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0'; // Clobber the character t is pointing at.
        t--;      // Decrement t.
    }
}

/* Get the next line from an input file that isn't blank, and leave it in s.  Will clobber
   s no matter what happens.  Will crash if there isn't a next blank line.

   YOU ARE ALLOWED TO COPY THIS FUNCTION. */

void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}

int main(void) {
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    /*
    Opens cop3502-as5-input.txt for reading
    Creates/Opens cop3502-as5-output-bermudez-elier.txt for writing
    */
    ifp = fopen("cop3502-as5-input.txt", "r");
    ofp = fopen("cop3502-as5-output-bermudez-elier.txt", "w");

    //Get number of integers
    int number_of_integers;
    fscanf(ifp, "%d", &number_of_integers);

    int *array;

    array = (int*)malloc(number_of_integers * sizeof(int));

    //Example of scanning in then printing some numbers
    printf("Enter 3 numbers\n");
    int i = 0;
    for(i = 0; i<3; i++){
        scanf("%d", &array[i]);
    }
    printf("Here are your 3 numbers\n");
    //The values are
    for( i = 0; i<3; i++){
        printf("%d\t", array[i]);
    }
    /*
    int i=0;
    monster *monsters=calloc(number_of_monsters, sizeof(monster));
    for (i = 0; i < number_of_monsters; i++)
    {
        read_monster(ifp, monsters + i);
    }
    */




    //Remember to free up memory
/*
void free_monster(monster *m)
{
        free(m->name);
        m->name = NULL;

        free(m->element);
        m->element = NULL;

        m->population = 0;
}
*/

    //fclose(ifp);
    //fclose(ofp);
}
