#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	"leak_detector_c.h"
//Elier Bermudez, COP3502, Assignment 1
//Hey elier on my end I right clicked on this filed and went to options and disabling compiling
//If you go back to this version it will never

typedef struct monster {
	int id;
	char *name;
	char *element;
	int population;
} monster;

typedef struct region {
	char *name;
	int number_of_monsters;
	int total_population;
	//This double pointer is essentially a pointer that goes to *monsters meaning that this is essentially an array of monsters
	monster **monsters;
} region;

typedef struct itinerary {
int nregions;
    //This is essentially an array of regions so treat it the same way you did the monsters above
	region **regions;
	int captures;
} itinerary;

typedef struct trainer {
	char *name;
	itinerary *visits;
} trainer;
//Do not change any code above this line as per code requirements.

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
//Above 2 functions are copied from given code

int get_number_of_monsters(FILE *ifp)
{
    //I tried to obfuscate my code to be funny because I couldn't figure out how to make this any different
    //The fact that C makes it so difficult to initialize variable sized arrays makes this very difficult
    //Anyways here's what my final attempt was. It should be exactly 128 characters if I wrote my other program correctly
    //char s[]={'I','_','d','o','n','t','_','k','n','o','w','_','h','o','w','_','t','o','_','m','a','k','e','_','t','h','i','s','_','f','u','n','c','t','i','o','n','_','d','i','f','f','e','r','e','n','t','_','f','r','o','m','_','y','o','u','r','s','_','s','i','n','c','e','_','i','t','s','_','s','o','_','s','i','m','p','l','e','_','s','o','_','h','e','r','e','s','_','s','o','m','e','_','m','i','n','o','r','_','o','b','f','u','s','c','a','t','i','o','n','_','f','o','r','_','y','o','u','r','_','t','r','o','u','b','l','e','s'};
    char s[128];
    int number_of_monsters=0;;

    // Gets the entire first line of ifp file
    //Limited by the fact that it can only read up to 127 characters
    fgets(s, 127, ifp);

    // The line should be formatted as "<number> monsters".  Pull off the number.
    //The first argument is the string we're reading stuff from, the second is the format
    //of what we're looking for (an integer in this case), and the third is where the number
    //is gonna get stored. In this case, the number variable
    sscanf(s, "%d", &number_of_monsters);
    return number_of_monsters;
}

void read_monster(FILE *ifp, monster *m)
{
    //Declaring strings to store the monster's name, element, and population/commonality whatever you wanna call it
    char name[128];
    char element[128];
    int population;

    //Scanning in the data 1 monster at a time and then filling in the monster array.
    //It's only 3 lines to fill in the monster array so I didn't make a separate function for it
    fscanf(ifp, "%s %s %d", name, element, &population);
    m->name = strdup(name);
    m->element = strdup(element);
    m->population = population;
}

//DELETEME once code is over
void print_monster(FILE *ofp, monster *m)
{
    fprintf(ofp, "Monster: %s   Element: %s   Population: %d\n",
            m->name, m->element, m->population);
}

void print_monster_console(monster *m)
{
    printf("Monster: %s   Element: %s   Population: %d\n",
            m->name, m->element, m->population);
}

void free_monster_array(monster *m, int number_of_monsters)
{
    int i=0;
    for(i = 0; i < number_of_monsters; i++)
    {
        free_monster(m + i);
    }

    free(m);
}

void free_monster(monster *m)
{
        free(m->name);
        m->name = NULL;

        free(m->element);
        m->element = NULL;

        m->population = 0;
}
int get_number_of_regions(FILE *ifp)
{
    char s[128];
    int number_of_regions=0;;

    // Gets the entire first line of ifp file
    //Limited by the fact that it can only read up to 127 characters
    get_next_nonblank_line(ifp, s, 127);

    // The line should be formatted as "<number> regions".  Pull off the number.
    //The first argument is the string we're reading stuff from, the second is the format
    //of what we're looking for (an integer in this case), and the third is where the number
    //is gonna get stored. In this case, the number variable
    sscanf(s, "%d", &number_of_regions);
    return number_of_regions;
}

void read_monster_second(FILE *ifp, monster *monster_current, monster *m, int complete_number_of_monsters)
{
    //Note that *monsters is the current monster from the list going into read regions, *m is the fat list of every monster.
    //Declaring strings to store the monster's name, element, and population/commonality whatever you wanna call it
    char name[128];
    char element[128];
    int population;

    //Scanning in the data 1 monster at a time and then filling in the monster array.
    fscanf(ifp, "%s", name);
    monster_current->name = strdup(name);
    /* These 2 will be more complex
    monster_current->element = strdup(element);
    monster_current->population = population;
    */
    /*Now we have a name for our monster. We have to search through the big list, find that name,
    then take ALL it's info, and shove it into the little list.*/
    /*Let's test if our big list is still being accessed correctly over here
    int i=0;
    for (i = 0; i < 4; i++)
    {
        printf("look at me");
        print_monster_console(m + i);
    }
    Yeah everything still seems fine.
    */
    /*
    printf("Monster: %s   Element: %s   Population: %d\n",
            m->name, m->element, m->population);
            */
    /*We have one name and now we want to go through the entire monster array list looking for the one that matches
    */
    int i=0;
    for(i=0; i<complete_number_of_monsters; i++){
            /* Testing to see if I'm reading the right names
            printf("Current monster name is %s\n", name);
            printf("Monster name we're comparing against is %s\n", (m+i)->name);
            printf("\n\n");
            */
            //If this is true, the names match so we found the monster
            if( strcmp(name, (m+i)->name)==0){
                    monster_current->element = strdup(  (m+i)->element);
                    monster_current->population = (m+i)->population;
            }
    }
    //At this point if everything is correct then we should have easily added at least the name of a monster to the monster list for a region
    //Additionally if the for loop works then we also should have the element of the monster and the population updated as well
}

void read_regions(FILE *ifp, region *r, monster *m, int complete_number_of_monsters)
{
    //Declaring strings to store the region's name, number_of_monsters, and population/commonality whatever you wanna call it
    //*m is the entire monster list, *monsters is the monster list we are populating for just this region
    char name[128];
    fscanf(ifp, "%s", name);
    r->name = strdup(name);

    //I was having a lot of trouble scanning stuff in right for some reason so I just copied a method over here
    //I think the problem has to do with scanning in 2 different ways
    char s[128];
    //Number of monsters 2 is the number of monsters for the specific region that I am dealing with
    int number_of_monsters_2=0;;
    fgets(s, 127, ifp);
    //I put this twice because my scanner is full or scanning a new line or something like that basically don't remove this it's important
    fgets(s, 127, ifp);
    sscanf(s, "%d", &number_of_monsters_2);
    int n = number_of_monsters_2;
    r->number_of_monsters = n;



    int total_population;
    //This should be just big enough to store the right amount of monsters. For sample input 1 that would be 4 monsters.
    monster *monsters=calloc(n, sizeof(monster));

    /*If this works then we have access to the monster array list and the pointers and stuff were all read into this function correctly
    int i=0;
    for (i = 0; i < n; i++)
    {
        print_monster_console(m + i);
    }
    Hey it actually worked ok next step
    */
    int i=0;
    for (i = 0; i < n; i++)
    {
        read_monster_second(ifp, monsters + i, m, complete_number_of_monsters);
    }
    /*Testing to see if our monster list inside of the region is accurate
        for (i = 0; i < n; i++)
    {
        print_monster_console( monsters + i);
    }
    Eureka it actually works wow
    */
    //Now to get total population
    int total_pop=0;
    for (i = 0; i < n; i++)
    {
        total_pop = total_pop +(monsters+i)->population;
    }
    r->total_population=total_pop;
    /*
    Testing everything
    printf("Name is %s\n",name);
    printf("N is %d\n",n);
    printf("Total pop is %d\n", total_pop);
    Works fine so far
    */

}


int find_correct_region(FILE *ifp, region *r, int captures, int regions_that_exist){
    //Declaring strings to store the region's name
    char name[128];

    //Scanning in the next string (which should be a region) and storing it in name
    fscanf(ifp, "%s", name);
    /*Now we have a name for our region. Let's go find the region in the region list *r which matches it*/
    int i=0;
    for(i=0; i<regions_that_exist; i++){
            /* Testing to see if I'm reading the right names
            printf("Current monster name is %s\n", name);
            printf("Monster name we're comparing against is %s\n", (m+i)->name);
            printf("\n\n");
            */
            //If this is true, the names match so we found the monster
            if( strcmp(name, (r+i)->name)==0){
                    /* This printed 2, 3, then 1 times for the sample input so I know it works and is comparing regions effectively
                    printf("Something matched\n");
                    */
                    //Now we know that whatever region we need to work with is r+i, so if we return i we know that r+i will be the right region.
                    return i;
            }
    }
}

void read_trainers(FILE *ifp, trainer *curr_trainer, region *r, int regions_that_exist)
{
	//Each trainer should get their own itinerary
	//Scanning in trainer name
    char name[128];
    fscanf(ifp, "%s", name);
    curr_trainer->name = strdup(name);
    curr_trainer->visits = malloc(sizeof(itinerary*));
    // (&curr_trainer).visits
    //The arrow is basically shorthand for doing the above command

    char s[128];
    int number_of_captures=0;
    fgets(s, 127, ifp);
    //I put this twice because my scanner is full or scanning a new line or something like that basically don't remove this it's important
    fgets(s, 127, ifp);
    sscanf(s, "%d", &number_of_captures);
    curr_trainer->visits->captures = number_of_captures;

    //Lord forgive me for I'm about to sin, I can't trust my methods so I'm just gonna copy the above code and change around variable names
    //To read in the number of regions
    char s2[128];
    int number_of_regions=0;
    //Now this time it works when I don't scan twice, still not sure why
    fgets(s2, 127, ifp);
    sscanf(s2, "%d", &number_of_regions);

    /*Testing everything
    printf("Name is %s\n",name);
    printf("Number of captures is is %d\n",number_of_captures);
    printf("Number of regions is is %d\n",number_of_regions);
    Works fine
    */
    /*Really if we just go straight into printing then we don't really need to keep track of the regions
    in the itinerary at all so I'm just gonna go straight into printing at this point so there's
    one less thing to deal with
    */
    printf("%s\n", name);
    //Let's find what region we're going to
    int j=0;
    int regioncode=0;
    for(j=0; j<number_of_regions; j++){
        regioncode = find_correct_region(ifp, r, number_of_captures, regions_that_exist);
        //Now for each region we gotta go in, access some variables in there, and do some math
        print_captures(r, regioncode, number_of_captures);
    }
    printf("\n");
}

int print_captures(region *r, int regioncode, int captures){
    /*
    To estimate the number of a given monster M a trainer will capture in a region R:
o Divide the relative population of M in R by R's total relative population.
o Multiply the result by the total number of captures the trainer intends per region.
o Round this result to the nearest integer. .5 rounds up, so you can use round() and its friends.
    */
    //First we print the region's name itself
    printf("%s\n", (r+regioncode)->name);
    int totalpop=(r+regioncode)->total_population;
    int current_monster_captures=0;
    //Works so far
    //We wanna repeat this based on the number of monsters in the region
    int i=0;
    for(i=0; i<(r+regioncode)->number_of_monsters; i++){
        //This loop is successfully repeating a number of times based on the number of monsters
        //Now the last hard part is gonna be finding the population of each monster and its name
        //Luckily region has its own monster list so we can actually just use that and go in order
        /*What I'm trying to do is to go to a specific region specified by r+regioncode
        then go to its list of monsters and go to a specific entry in that list specified by monsters+i
        then go to that monster's population value
        */
        //This defaults to giving back the number_of_monsters for the first region if you don't add +i so try
        //messing around to get it working before you do +i?
        //Aria's first region is rome
        //Rome's first monster is st augustine
        //St augustin's population is 12
        //If your code is working, you should be able to get current_monster_captures to be equal to 12
        /* What if we separated it into 2 lines of code and had a temp monster array a the ready?*/
        //Temp monster array that holds up to 5 monsters
        //current_monster_captures = monsters_temp->population;
        //Code below here is the one gerber gave you
        //current_monster_captures = (r+regioncode)->monsters[i]->population;
        //current_monster_captures = ((r+regioncode)->monsters)[i]->population;
        //printf("Current monster cap is %d\n", current_monster_captures);
        //printf("r+regcode num monsters would be %d\n",(r+regioncode)->number_of_monsters);
        //printf("I'm on monster number %d\n", (i+1));
    }
    printf("I'm done with the monster loop for regions\n");
}

int main(void) {
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    /*
    Opens cop3502-as1-input.tx for reading
    Creates/Opens cop3502-as1-output-bermudez-elier.txt for writing
    */
    ifp = fopen("cop3502-as1-input.txt", "r");
    ofp = fopen("cop3502-as1-output-bermudez-elier.txt", "w");

    //Get number of monsters
    int number_of_monsters;
    number_of_monsters = get_number_of_monsters(ifp);
    /*Testing to see if code works so far.
    printf("Number of monsters is %d", number_of_monsters);
    */
    int i=0;
    monster *monsters=calloc(number_of_monsters, sizeof(monster));
    for (i = 0; i < number_of_monsters; i++)
    {
        read_monster(ifp, monsters + i);
    }
    /*
    ==========
    “arr[i]” is treated as *(arr + i) by the compiler
    ==========

    monster **monsters = calloc(num_monsters, sizeof(monster*);
    for (i=0 ; i < num; i++)
        monsters[i]->population;

        &monsters[i].population;
        This would




    */
    //Change this section to be print_regions, or print_trainers to check your work so far
    /*
    for (i = 0; i < number_of_monsters; i++)
    {
        print_monster(ofp, monsters + i);
    }
    */

    //Get number of regions
    int number_of_regions;
    number_of_regions = get_number_of_regions(ifp);
    /*Testing to see if code works so far.
    printf("Number of regions is %d", number_of_regions);
    */
    i=0;
    region *regions=calloc(number_of_regions, sizeof(region));;
    for (i = 0; i < number_of_regions; i++)
    {
        read_regions(ifp, regions + i, monsters, number_of_monsters);
    }

    //Get number of trainers
    int number_of_trainers;
    //Not sure why I bothered giving region it's own method when I could reuse the same ones but now I don't want to change
    //it so that nothing breaks but this isn't a mistake I meant to use the regions method
    number_of_trainers = get_number_of_regions(ifp);
    /*Testing to see if code works so far.
    printf("Number of trainers is %d", number_of_trainers);
    */
    i=0;
    trainer *trainers=calloc(number_of_trainers, sizeof(trainers));
    for (i = 0; i < number_of_trainers; i++)
    {
        read_trainers(ifp, trainers + i, regions, number_of_regions);
    }

    //Freeing everything
    //Have not freed anything in region so far
    //free_monster_array(monsters, number_of_monsters);
    //fclose(ifp);
    //fclose(ofp);
    //Maybe this'll free the memory up
    //abort();
}
