#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include    "leak_detector_c.h"
#include <math.h>
//Elier Bermudez, COP3502, Assignment 1
//Keyword TESTING means a comment is just leftover code to see if something worked up to a point

typedef struct monster
{
    int id;
    char *name;
    char *element;
    int population;
} monster;

typedef struct region
{
    char *name;
    int number_of_monsters;
    int total_population;
    monster **monsters;
} region;

typedef struct itinerary
{
    int nregions;
    region **regions;
    int captures;
} itinerary;

typedef struct trainer
{
    char *name;
    itinerary *visits;
} trainer;

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

void read_monster(FILE *ifp, monster *outside_monster, int i)
{
    //Declaring strings to store the monster's name, element, and population/commonality whatever you wanna call it
    char name[128];
    char element[128];
    int population;

    //Scanning in the data 1 monster at a time and then filling in the monster data
    fscanf(ifp, "%s %s %d", name, element, &population);
    outside_monster->id = i;
    outside_monster->name = strdup(name);
    outside_monster->element = strdup(element);
    outside_monster->population = population;
}

void read_region(FILE *ifp, region *r, monster **outside_monster_list, int outside_number_of_monsters)
{
    //Reading in region name then assigning a copy of it to r->name
    char s[128];
    fscanf(ifp, "%s", s);
    r->name = strdup(s);

    //Reading in regional number of monsters then assigning it to r->number_of_monsters
    int regional_number_of_monsters=0;
    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &regional_number_of_monsters);
    r->number_of_monsters = regional_number_of_monsters;

    //Allocating memory for regional monster list
    r->monsters = calloc(regional_number_of_monsters, sizeof(monster*));

    //Reading in monster names while calculating total population and linking the monster pointers to outside monster pointers
    int i=0;
    int j=0;
    for(i=0; i<regional_number_of_monsters; i++)
    {
        //Reading in monster name
        char curr_monster_name[128];
        fscanf(ifp, "%s", curr_monster_name);

        //Running a loop to find when the name of a monster we're scanning matches that of one in outside_monster_list
        for(j=0; j<outside_number_of_monsters; j++)
        {
            if( strcmp(curr_monster_name, (outside_monster_list[j])->name)==0)
            {
                //Taking the monster list from regions, using i to make sure we are on a specific monster entry, then linking
                //the pointer to a monster from outside_monster_list using j
                r->monsters[i]=outside_monster_list[j];

                // Add to total population for region
                r->total_population += r->monsters[i]->population;
            }
        }
    }
}

void read_trainer(FILE *ifp, trainer *t, region **outside_region_list, int outside_number_of_regions, FILE *ofp, int number_of_trainers, int curr_trainer_number)
{
    t->visits=calloc(1, sizeof(itinerary));//Allocating memory for visits in trainers

    //Reading in trainer name then assigning a copy of it to t->name and printing it out
    char s[128];
    fscanf(ifp, "%s", s);
    t->name = strdup(s);

    fprintf(ofp,"%s\n", s);
    //printf("%s\n", s);

    //Reading in trainer number of captures then assigning it to t->visits->captures
    int trainer_number_of_captures=0;
    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &trainer_number_of_captures);
    t->visits->captures = trainer_number_of_captures;

    //Reading in trainer number of regions then assigning it to t->visits->number_of_regions
    int trainer_number_of_regions=0;
    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &trainer_number_of_regions);
    t->visits->nregions = trainer_number_of_regions;

    //Reading in region names while linking the region pointers to outside region pointers,
    //doing the math for captures, and printing them out with monster names next to them
    int i=0;
    int j=0;
    int k=0;
    double region_pop=0;
    double monster_pop=0;
    int curr_captures=0;
    //Running a loop that for each region the trainer is visiting
    for(i=0; i<trainer_number_of_regions; i++)
    {
        //Reading in region name then printing it
        char curr_region_name[128];
        fscanf(ifp, "%s", curr_region_name);
        fprintf(ofp,"%s\n", curr_region_name);
        //printf("%s\n", curr_region_name);

        //Running a loop to find when the name of a region we're scanning matches that of one in outside_region_list
        for(j=0; j<outside_number_of_regions; j++)
        {
            //Checks if my current region matches that of a specific outside region
            if( strcmp(curr_region_name, (outside_region_list[j])->name)==0)
            {
                //Finding the regional population
                region_pop = outside_region_list[j]->total_population;
                //Running a loop through the region to find the captures for each monster
                for(k=0; k<outside_region_list[j]->number_of_monsters; k++)
                {
                    //Finding the population of the monster we are currently on
                    monster_pop = outside_region_list[j]->monsters[k]->population;

                    //Calculating captures for the current monster using the capture formula
                    curr_captures = round((monster_pop/region_pop)*trainer_number_of_captures);

                    //Prints the number of captures and monster name for each monster
                    //Only prints if there are captures for the current monster
                    if(curr_captures!=0)
                    {
                        fprintf(ofp, "%d %s\n", curr_captures, outside_region_list[j]->monsters[k]->name);
                        //printf("%d %s\n", curr_captures, outside_region_list[j]->monsters[k]->name);
                    }
                }
            }
        }
    }
    //Print a newline only if we are not on the last trainer, to prevent an excess new line at the end of the output
    if(curr_trainer_number<number_of_trainers-1)
    {
        fprintf(ofp, "\n");
        //printf("\n");
    }
}

void free_monster(monster *outside_monster)
{
    outside_monster->id = 0;

    free(outside_monster->name);
    outside_monster->name = NULL;

    free(outside_monster->element);
    outside_monster->element = NULL;

    outside_monster->population = 0;
}

void free_monster_array(monster **outside_monster_list, int number_of_monsters)
{
    int i=0;
    for(i = 0; i < number_of_monsters; i++)
    {
        free_monster(outside_monster_list[i]);
        free(outside_monster_list[i]);
    }
    free(outside_monster_list);
}

void free_region(region *outside_region)
{
    free(outside_region->name);
    outside_region->name = NULL;

    free_monster_array(outside_region->monsters, outside_region->number_of_monsters);

    outside_region->number_of_monsters = 0;

    outside_region->total_population = 0;
}

void free_region_array(region **outside_region, int number_of_regions)
{
    int i=0;
    for(i = 0; i < number_of_regions; i++)
    {
        free_region(outside_region[i]);
        free(outside_region[i]);
    }
    free(outside_region);
}

void free_itinerary(itinerary *outside_visits)
{
    outside_visits->captures = 0;

    outside_visits->nregions = 0;
}

void free_trainer(trainer *outside_trainer)
{
    free(outside_trainer->name);
    outside_trainer->name = NULL;

    free_itinerary(outside_trainer->visits);
    free(outside_trainer->visits);
}

void free_trainer_array(trainer **outside_trainer, int number_of_trainers)
{
    int i=0;
    for(i = 0; i < number_of_trainers; i++)
    {
        free_trainer(outside_trainer[i]);
        free(outside_trainer[i]);
    }
    free(outside_trainer);
}

int main(void)
{
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("cop3502-as1-input.txt", "r"); //Opens cop3502-as1-input.tx for reading
    ofp = fopen("cop3502-as1-output-bermudez-elier.txt", "w"); //Creates/Opens cop3502-as1-output-bermudez-elier.txt for writing

    int number_of_monsters;
    number_of_monsters = scan_in_first_number_from_line(ifp); //Gets number of monsters
    // TESTING: number_of_monsters
    //printf("Number of monsters is %d", number_of_monsters);

    //Reading monsters in
    int i=0;
    monster **monsters=calloc(number_of_monsters, sizeof(monster*)); //Allocating memory for list of pointers to all monsters
    for(i=0; i<number_of_monsters; i++)
    {
        monsters[i] = calloc(1, sizeof(monster));
        read_monster(ifp, monsters[i], i);//Reading information into each monster entry
    }
    /* TESTING: monsters
    for(i=0; i<number_of_monsters; i++){
    printf("Id is %d, name is %s, element is %s, population is %d\n", (monsters[i])->id,  (monsters[i])->name,  (monsters[i])->element,  (monsters[i])->population );
    }*/


    int number_of_regions;
    number_of_regions =scan_in_first_number_from_line(ifp); //Gets number of regions
    /* TESTING: number_of_regions
    printf("Number of regions is %d", number_of_regions);*/

    region **regions=calloc(number_of_regions,sizeof(region*));//Allocating memory for list of pointers to all regions
    for(i=0; i<number_of_regions; i++)
    {
        regions[i] = calloc(1, sizeof(region));
        read_region(ifp, regions[i], monsters, number_of_monsters);
    }
    /* TESTING: regions
    for(i=0; i<number_of_regions; i++)
    {
        int j=0;
        printf("Name is %s, number_of_monsters is %d, total population is %d. \n", regions[i]->name, regions[i]->number_of_monsters, regions[i]->total_population);
        for(j=0; j< regions[i]->number_of_monsters; j++)
        {
            monster *curr_monster = regions[i]->monsters[j];
            printf("Id is %d, name is %s, element is %s, population is %d\n", curr_monster->id,  curr_monster->name,  curr_monster->element,  curr_monster->population );
        }
        printf("\n");
    }
    */

    int number_of_trainers;
    number_of_trainers = scan_in_first_number_from_line(ifp); //Gets number of monsters
    /* TESTING: number_of_trainers
    printf("Number of trainers is %d", number_of_trainers);
    */

    //Reading trainers in. read_trainer also begins the printing process
    trainer **trainers=calloc(number_of_trainers, sizeof(trainer*)); //Allocating memory for list of pointers to all monsters
    for(i=0; i<number_of_trainers; i++)
    {
        trainers[i] = calloc(1, sizeof(trainer));
        read_trainer(ifp, trainers[i], regions, number_of_regions, ofp, number_of_trainers, i);//Reading information into each monster entry
    }
    /* TESTING: trainers
    for(i=0; i<number_of_trainers; i++){
    printf("Id is %d, name is %s, element is %s, population is %d\n", (monsters+i)->id,  (monsters+i)->name,  (monsters+i)->element,  (monsters+i)->population );
    }*/

    //Memory freeing begins
    free_monster_array(monsters, number_of_monsters);
    free_region_array(regions, number_of_regions);
    free_trainer_array(trainers, number_of_trainers);


    //Closing scanners
    fclose(ifp);
    fclose(ofp);
}
