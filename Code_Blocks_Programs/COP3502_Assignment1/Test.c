struct monster_struct
{
    char *name;
    int commonality;
    int weight;
    struct monster_struct *next;
};

typedef struct monster_struct monster;

typedef struct
{
    monster *head;
} monster_list;


void print_mosnter_list(monster_list *m1)
{
    monster *m = m1->head;
    while(m != NULL)
    {
        print_monster(m);
        m = m->next;
    }
}

monster *second_most_common(monster_list *ml)
{
    //Creating integers to store the useful common values
    int most_common_value = 0;
    int second_most_common_value = 0;
    int current_common_value = 0;

    //Creating a monster pointer that points at the second_most_common_monster
    monster *second_most_common_monster = 0;

    //Getting the first monster from the monster list
    monster *m = m1->head;

    //Using a while loop to continue as long as we haven't run out of monsters
    while (m != NULL)
    {
        //Updating current_common_value
        current_common_value = m->commonality;

        //Checking if most common then updating
        if(current_common_value >)

        //Going to the next monster
        m = m->next
    }
}

monster *third_lightest(monster_list *ml)
{

}
