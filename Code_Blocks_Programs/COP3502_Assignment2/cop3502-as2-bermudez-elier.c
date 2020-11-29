#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include    "leak_detector_c.h"
#include <math.h>
//Elier Bermudez, COP3502, Assignment 2
//Keyword TESTING means a comment is just leftover code to see if something worked up to a point
//Keyword CHECK means I forgot to delete something that I was still working on and was meant to check before turning in
//Keyword CHECKULTRA means I forgot to fix something and my code is probably broken because of it
//Even though we need to use a "queue" for the purposes of this program we only REALLY need the dequeue and isEmpty parts of a queue
//Since we would never add MORE failfish past the initial setup, or need to know the last failfish in a pond, so enqueue and peek become useless

//Gerber function
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    // t begins at the null sentinel at the end of s.

    t--;

    while ((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0'; // Clobber the character t is pointing at.
        t--;      // Decrement t.
    }
}

//Gerber function
void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}

//Gerber function
int scan_in_first_number_from_line(FILE *ifp)
{
    //Setting up variables, a string and an integer
    char s[128];
    int number=0;;

    //Scanning the whole line into f then scanning it for the first integer we see
    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &number);

    return number;
}

struct failfish_struct
{
    int fail_number;
    struct failfish_struct *next;
    struct failfish_struct *prev;
};
typedef struct failfish_struct failfish;

typedef struct
{
    failfish *head;
} failfish_list;

//Same as a failfish_list but trying to change my code would lead to problems
typedef struct
{
    failfish *head;
    failfish *tail;
    char *pondname;
    int n;
    int e;
    int th;
} failfish_queue;

typedef struct
{
    int pond_number;
    char *name;
    int failfish_number;
    int eating_counter_number;
    int minimum_failgroup_number;
    failfish_list *fish;
} pond;

failfish *new_failfish(int fail_number)
{
    failfish *ff = malloc(sizeof(failfish));
    ff->fail_number = fail_number;
    ff->next = NULL;
    ff->prev = NULL;
    return ff;
}

void dispose_failfish(failfish *ff)
{
    ff->fail_number = 0;
    free(ff);
}

failfish_list *new_failfish_list(void)
{
    failfish_list *ffl;

    ffl = malloc(sizeof(failfish_list));
    ffl->head = NULL;

    return ffl;
}

failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th)
{
    failfish_queue *ffl;

    ffl = malloc(sizeof(failfish_queue));
    ffl->head = NULL;
    ffl->tail = NULL;
    ffl->pondname = strdup(pondname);
    ffl->n = n;
    ffl->e = e;
    ffl->th = th;

    return ffl;
}

void dispose_failfish_list(failfish_list *ffl)
{
    free(ffl);
}

void dispose_failfish_list_all(failfish_list *ffl)
{
    failfish *ff_curr = ffl->head;
    failfish *ff_temp;

    if(ff_curr == NULL)
    {
        dispose_failfish_list(ffl);
        return;
    }

    do
    {
        ff_temp = ff_curr->next;
        dispose_failfish(ff_curr);
        ff_curr = ff_temp;
    }
    while(ff_curr != ffl->head);

    dispose_failfish_list(ffl);
}

void failfish_list_add(failfish_list *ffl, failfish *new_failfish)
{
    //If ffl is empty, set ffl's head to new_failfish and link new_failfish to itself in both directions
    if(ffl->head == NULL)
    {
        new_failfish->next = new_failfish;
        new_failfish->prev = new_failfish;
        ffl->head = new_failfish;
    }
    //We COULD encase this entire thing as an else statement but conveniently enough
    //even when ff1->head == NULL initially, the rest of this will work and give the correct result
    failfish *ff_curr = ffl->head;

    //This while loop will find the "tail" element of ffl, which is the last element before ffl->head
    do
    {
        ff_curr = ff_curr->next;
    }
    while(ff_curr->next != ffl->head);

    //Paraphrased Gerber comment:
    // ff_curr is now pointing at the first element of ffl which points at ffl->head, which unless
    // ffl is broken, should be the tail element of ffl.

    ff_curr->next = new_failfish;   //The tail's next element is now the new fish
    new_failfish->prev = ff_curr;   //The new fish's prev element is now the "tail"
    new_failfish->next = ffl->head; //The new fish's next element is now the head
    ffl->head->prev = new_failfish; //The head's previous element is now new_failfish.
}

//Gerber function, slightly modified
//If dispose, frees m_to_delete.  Otherwise, set both its links to null.
void clear_links_or_dispose(failfish *ff_to_delete, int dispose)
{
    ff_to_delete->next = NULL;
    ff_to_delete->prev = NULL;
    if(dispose != 0)
    {
        dispose_failfish(ff_to_delete);
    }
}

//Slightly modified Gerber function
//Delete a monster from a list of monsters.  Always removes it from the list; if
//dispose, also disposes the monster structure from memory.
void failfish_list_delete(failfish_list *ffl, failfish *ff_to_delete, int dispose)
{
    // First thing to verify is if this will empty the list.

    if(ff_to_delete->next == ff_to_delete) // We could also use if(ffl->head == ffl->tail)
    {
        clear_links_or_dispose(ff_to_delete, dispose);
        ffl->head = NULL;
        return;
    }

    /* We now know we're not deleting the only item in the list.  To make this less
       messy, we're going to repair the head and/or tail pointers *before* the actual
       linkbreaking and deletion. */

    if(ffl->head == ff_to_delete)
    {
        ffl->head = ff_to_delete->next;
    }

    /* We now know we're deleting an item that is not the only item in the list, and
       that is neither the head nor the tail.  We don't have to worry about head or
       tail pointers, just about our next and prev pointers - which makes the link
       repair really easy. */

    ff_to_delete->prev->next = ff_to_delete->next;
    ff_to_delete->next->prev = ff_to_delete->prev;

    /* Nothing in the list points to us any more, so we can now safely delete. */
    clear_links_or_dispose(ff_to_delete, dispose);
}

failfish_list *populate_failfish_list(failfish_list *ffl, int failfish_number)
{
    int i = 1;
    failfish *ff;
    for(i=1; i<failfish_number+1; i++)
    {
        ff = new_failfish(i);
        failfish_list_add(ffl, ff);
    }
    return ffl;
}

pond *new_pond(int pond_number, char *name, int failfish_number, int eating_counter_number, int minimum_failgroup_number){
    //This line works. It wastes more memory than needed but the more efficient lines I tried aren't working
    //Attempted:        pond *p = malloc(sizeof(*pond));
    //                  pond *p = malloc(4);
    pond *p = malloc(sizeof(pond));
    p->pond_number = pond_number;
    p->name = strdup(name);
    p->failfish_number = failfish_number;
    p->eating_counter_number = eating_counter_number;
    p->minimum_failgroup_number = minimum_failgroup_number;
    p->fish = new_failfish_list();
    populate_failfish_list(p->fish, p->failfish_number);
    return p;
}

void dispose_pond(pond *p)
{
    p->pond_number = 0;
    free(p->name);
    p->name = NULL;
    p->failfish_number = 0;
    p->eating_counter_number = 0;
    p->minimum_failgroup_number = 0;
    //Free the list of fish inside
    //dispose_failfish_list_all(p->fish);
    //This line being commented causes a memory leak but letting it run crashes my code.
    //free(p);
}

void firstcourse(FILE *ofp, pond *p)
{
    //Initialize variables
    failfish *ff_curr = p->fish->head;
    failfish *ff_curr_temp = p->fish->head;
    int population = p->failfish_number;
    int minimum_failgroup = p->minimum_failgroup_number;
    int step_size = (p->eating_counter_number)-1;
    int i = 0;

    //Print pond name
    fprintf(ofp, "\n\nPond %d: %s", p->pond_number, p->name);
    //Bonus step is needed because my method of doing this jumps by 1 step too little
    //the first time that the failfish eat each other.
    int bonus_step = 1;
    //When step_size = 0 bonus_step needs to be ignored to prevent errors
    if(step_size == 0)
    {
        bonus_step = 0;
    }
    //Run a loop as long as the current population of the pond is larger than the minimum
    while(population > minimum_failgroup)
    {
        //Go to the next failfish an amount of times equal to step_size
        for(i=0; i<step_size+bonus_step; i++)
        {
            ff_curr = ff_curr->next;
            bonus_step = 0;
        }
        //Delete the fish you end up on while also keeping your place in the list by going to the next fish.
        //My method for doing this naturally moves forward by 1 when something is deleted
        ff_curr_temp = ff_curr;
        ff_curr = ff_curr->next;
        fprintf(ofp, "\nFailfish %d eaten", ff_curr_temp->fail_number);
        failfish_list_delete(p->fish, ff_curr_temp, 1);
        population--;
    }
    p->failfish_number = population;
}

void fprint_failfish(FILE *ofp, failfish *ff)
{
    fprintf(ofp, " %d", ff->fail_number);
}

//This print will only work for a circly linked list. Queue uses a different print
void fprint_failfish_list(FILE *ofp, failfish_list *ffl)
{
    failfish *ff_curr = ffl->head;

    if(ff_curr == NULL)
    {
        return;
    }

    //This do while loop will stop printing once it loops back around the ffl->head
    do
    {
        fprint_failfish(ofp, ff_curr);
        ff_curr = ff_curr->next;
    }
    while(ff_curr != ffl->head);
}

void fprint_pond(FILE *ofp, pond *p)
{
    fprintf(ofp,"%d %s", p->pond_number, p->name);
    fprint_failfish_list(ofp, p->fish);
    fprintf(ofp, "\n");
}

//This function I only didn't use because I started working with new_failfish and I would likely cause an error by trying to replace the name and being careless
failfish *create_failfish(int sequence_number)
{
    failfish *ff = malloc(sizeof(failfish));
    ff->fail_number = sequence_number;
    ff->next = NULL;
    ff->prev = NULL;
    return ff;
}

int isEmpty(failfish_queue *q)
{
    if(q->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void enqueue(failfish_queue *q, failfish *ff)
{
    //If it's empty, ff becomes the head AND tail
    if(isEmpty(q)==1)
    {
        q->head = ff;
        q->tail = ff;
    }
    //If it's not empty, the tail gets linked to ff, ff gets linked to tail, and ff becomes the new tail
    else
    {
        q->tail->next = ff;
        ff->prev = q->tail;
        q->tail = ff;
    }
}

void dequeue(failfish_queue *q)
{
    //If it's empty, do nothing
    if(isEmpty(q)==1)
    {
    }
    //If it's not empty, see if there's only one element
    if(q->head == q->tail)
    {
        dispose_failfish(q->head);
        q->head == NULL;
        q->tail == NULL;
    }
    //If there are at least 2 elements, set head's next to be head instead and free the memory that the dequeued failfish took up
    else
    {
        failfish *ff_temp = q->head;
        q->head = q->head->next;
        dispose_failfish(ff_temp);
    }
}

failfish *peek(failfish_queue *q)
{
    return (q->head);
}

//void print_failfish_queue(failfish_queue *q);
void print_failfish_queue(FILE *ofp, failfish_queue *q)
{
    failfish *ff_curr = q->head;

    if(ff_curr == NULL)
    {
        return;
    }

    //This do while loop will stop printing once it reaches the tail
    //ff_curr should never equal NULL but I threw it in just in case
    do
    {
        fprint_failfish(ofp, ff_curr);
        ff_curr = ff_curr->next;
    }
    while((ff_curr != NULL) && (ff_curr != q->tail));
}

int main(void)
{
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("cop3502-as2-input.txt", "r"); //Opens cop3502-as2-input.tx for reading
    ofp = fopen("cop3502-as2-output-bermudez-elier.txt", "w"); //Creates/Opens cop3502-as2-output-bermudez-elier.txt for writing

    //We make an array of pond pointers
    pond *pond_list[10];

    //Scan in the number of populated ponds
    int number_of_populated_ponds = scan_in_first_number_from_line(ifp);

    //Create counter and temporary variables. t_ stands for temp
    int i, j, t_pond_number, t_failfish_number, t_eating_counter_counter, t_minimum_failgroup_number = 0;
    char t_name[128];

    //Initialize everything to NULL. Prevents some errors I was having.
    for(i=0; i<10; i++)
    {
        pond_list[i] = NULL;
    }
    //Run a loop to scan in the information for each pond
    for(i=0; i<number_of_populated_ponds; i++)
    {
        //Scan the next line and scan the input into each temporary variables
        fscanf(ifp, "%d %s %d %d %d", &t_pond_number, t_name, &t_failfish_number, &t_eating_counter_counter, &t_minimum_failgroup_number);
        //Run a loop to sync up t_pond number with that entry of pond_list[j]
        for(j=0; j<10; j++)
        {
            //Subtract 1 from t_pond_number because pond 1 should be entry 0
            if(j==(t_pond_number-1))
            {
                //Create a new_pond and link pond_list[j] to it
                pond_list[j] = new_pond(t_pond_number, t_name, t_failfish_number, t_eating_counter_counter, t_minimum_failgroup_number);
            }
        }
    }
    //pond_list now should have each pond in it and we should be done with scanning in input

    //Print the initial pond status
    fprintf(ofp,"%s\n", "Initial Pond Status");
    for(i=0; i<10; i++)
    {
        if(pond_list[i]!=NULL)
        {
            fprint_pond(ofp, pond_list[i]);
        }
    }

    //Begin first course
    fprintf(ofp, "\nFirst Course");
    //Run a loop to access each pond
    for(i=0; i<10; i++)
    {
        if(pond_list[i]!=NULL)
        {
            firstcourse(ofp, pond_list[i]);
        }
    }

    //Print the pond status after the first course
    fprintf(ofp,"%s\n", "\n\nEnd of Course Pond Status");
    for(i=0; i<10; i++)
    {
        if(pond_list[i]!=NULL)
        {
            fprint_pond(ofp, pond_list[i]);
        }
    }

    //Begin second course
    fprintf(ofp, "\nSecond Course");
    failfish *ff_largest    = NULL;
    failfish *ff_curr       = NULL;

    int ff_largest_fail    = 0;
    int ff_smallest_pond_queue= 2147483647;

    int last_fail = 0;
    int last_pond = 0;

    int lonely_ponds = 0;
    //This while loop will run as long as ff_largest does not equal ff_curr
    //Largest will only be the same as curr
    //Once largest is the same as curr that means we have run through the entire loop and kept something since everything else is null
    while(lonely_ponds<10)
    {
        for(i=0; i<10; i++)
        {
            //Check that pond_list[i] is not NULL (had nothing to begin with)
            if(pond_list[i] != NULL)
            {
                //Check that pondlist[i]->fish->head is not NULL (has become empty)
                if(pond_list[i]->fish->head != NULL)
                {
                    //Set ff_curr to be the head of pond_list[i]
                    ff_curr = pond_list[i]->fish->head;

                    //Check that ff_curr is not NULL
                    if(ff_curr != NULL)
                    {
                        //If ff_largest is NULL, update all values
                        if(ff_largest == NULL)
                        {
                            ff_largest = ff_curr;
                            ff_largest_fail = ff_curr->fail_number;
                            ff_smallest_pond_queue = i;
                        }
                        //If ff_curr is larger than ff_largest, update all values
                        else if(ff_curr->fail_number > ff_largest_fail)
                        {
                            ff_largest = ff_curr;
                            ff_largest_fail = ff_curr->fail_number;
                            ff_smallest_pond_queue = i;
                        }
                        //If ff_curr is the same size as ff_largest but has a smaller pond_queue_size, update all values
                        else if((ff_curr->fail_number == ff_largest_fail)   &&  (i < ff_smallest_pond_queue))
                        {
                            ff_largest = ff_curr;
                            ff_largest_fail = ff_curr->fail_number;
                            ff_smallest_pond_queue = i;
                        }
                    }
                }
                else
                {
                    lonely_ponds++;
                }
            }
            else
            {
                lonely_ponds++;
            }
        }
        //After going through all that, now ff_largest is the correct failfish
        //We delete ff_largest from the list that it's in.
        //ff_smallest_pond_queue also doubles as an index of which pond we found ff_largest in
        //so we use that to specify the correct pond to extract a failfishlist from
        //Eaten: Failfish 4 from pond 1
        //This print statement would print 0, 2123123123 if I am correct so maybe move it inside the if statement CHECK
        if( (ff_largest_fail!=0) && (ff_smallest_pond_queue!= 2147483647) )
        {
            last_fail = ff_largest_fail;
            last_pond = ff_smallest_pond_queue;

            //Only prints if it's NOT the last entry to be deleted
            if(!(lonely_ponds>8   &&  ff_largest->next==ff_largest))
            {
                //CHECK: This gives too much output. One way to know that we are on the last entry is if we are on the last pond, meaning loney_ponds = 9
                //Even if on the last pond we would have to know we're on the last entry, meaning that head->next=null
                fprintf(ofp, "\nFailfish %d from pond %d", ff_largest_fail, ff_smallest_pond_queue+1);
            }
            else
            {
                fprintf(ofp, "\n\nFailfish %d from pond %d remains", last_fail, last_pond+1);
            }
        }
        //The problem is we're trying to get pond_list[ff_smallest_pond_queue but smallest_pond_queue is very large and causing an index out of bounds error
        if(ff_smallest_pond_queue != 2147483647)
        {
            if(pond_list[ff_smallest_pond_queue] != NULL)
            {
                if(pond_list[ff_smallest_pond_queue]->fish != NULL)
                {
                    failfish_list_delete(pond_list[ff_smallest_pond_queue]->fish, ff_largest, 1);
                }
            }
        }
        //CHECK The problem is that once we reach the correct final thing, this line over here ends up crashing the program for trying to delete something that doesn't exist
        //Maybe moving it inside of that if statement would help
        //failfish_list_delete(pond_list[ff_smallest_pond_queue]->fish, ff_largest, 1);

        //Reset Variables
        ff_largest    = NULL;
        ff_curr       = NULL;
        ff_largest_fail    = 0;
        ff_smallest_pond_queue= 2147483647;
        if(lonely_ponds<10)
        {
            lonely_ponds = 0;
        }
    }

    //Closing scanners
    fclose(ifp);
    fclose(ofp);
}
