#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is a monster structure for a singly linked list of monsters.  It contains basic 
   information about a monster, as well as a link to a next monster.  next is null if and only
   if one of the following things is true:

    * The monster has not been added to a list yet
    * The monster sits at the tail of the list, i.e. the other end of the list from the 
    * head, so there is no next monster from it */

struct monster_struct {
    char *name;
    struct monster_struct *next;
};

//This is giving monster_struct the nickname of monster
typedef struct monster_struct monster;

/* This is a structure for a singly linked list of monsters.  It contains only a head node.
   head is NULL if and only if the list is empty.  Otherwise, head points to the most recently
   added monster. */

struct monster_list_struct {
    monster *head;
};

//This is giving monster_list_struct the nickname of monster_list
typedef struct monster_list_struct monster_list;

/* ------ Functions for monsters. */

/* Allocate and return a new monster. */

monster *new_monster(char *name)
{
    monster *m;

    m = malloc(sizeof(monster));
    m->name = strdup(name);
    m->next = NULL;

    return m;
}

/* Destructor for monster. */

void dispose_monster(monster *m)
{
    free(m->name);
    free(m);
}

/* Print out basic info for a monster. */

void print_monster(monster *m)
{
    printf("  Monster %s\n", m->name);
    if(m->next == NULL) {
        printf("    Monster has no link\n");
    } else {
        printf("    Monster has link to: %s\n", m->next->name);
    }
}

/* ------ Functions for monster_lists. */

/* Allocate a new, empty linked list of monsters. */

monster_list *new_monster_list(void)
{
    monster_list *ml;

    ml = malloc(sizeof(monster_list));
    ml->head = NULL;

    return ml;
}

/* Dispose of a list of monsters. THIS DOES NOT DISPOSE THE MONSTERS IN IT */

void dispose_monster_list(monster_list *ml)
{
    free(ml);
}

/* Dispose of a list of monsters *and* all the monsters in it. */

void dispose_monster_list_all(monster_list *ml)
{
    monster *m = ml->head;
    monster *n;
    
    /*Loop to destroy all monsters in the list.
    Finds the next monster, stores it in n, destroys the current one in m, sets m to the next one, repeats until current one is NULL*/
    while(m != NULL)
    {
        n = m->next;
        dispose_monster(m);
        m = n;
    }

    dispose_monster_list(ml);
}

/* Print every monster in a list of monsters. */

void print_monster_list(monster_list *ml)
{
    monster *m = ml->head;

    while(m != NULL)
    {
        print_monster(m);
        m = m->next;
    }
}

/* Add monster m to list ml.  m must already be an allocated monster and must *not* be
   a member of ml or any other monster_list. This adds the monster to the front of the list */

void monster_list_add(monster_list *ml, monster *new_monster)
{
    new_monster->next = ml->head;
    ml->head = new_monster;
}

/* Add monster m to the tail of list ml. m must already be an allocated monster and 
   must *not* be a member of ml or any other monster_list.  */

void monster_list_add_tail(monster_list *ml, monster *new_monster)
{
    monster *m = ml->head;

    /* Find the tail element of ml. */

    while(m->next != NULL)
    {
        m = m->next;
    }

    /* m is now pointing at the first element of ml with a null next pointer - which, unless
       ml is broken, should be the tail element of ml. */

    m->next = new_monster;
    new_monster->next = NULL; // *should* have been done when creating the monster
}

/* Delete a monster from a list of monsters. */

void monster_list_delete(monster_list *ml, monster *m_to_delete)
{
    monster *m = ml->head;
    monster *n;

    /* Deleting the head is a special case. */

    if(ml->head == m_to_delete) {
        ml->head = ml->head->next;
        dispose_monster(m_to_delete);
        return;
    }

    /* Otherwise, linked list deletion always works the same way.  You need to:

       * Find the item in the list
       * Set the pointer(s) that reference the item to reference the next item
         instead
       * Dispose the item

       The trick is making sure you do it IN THAT ORDER. */

    while(m->next != NULL)
    {
    	//Once we reach the monster BEFORE the one we want to delete,
        if(m->next == m_to_delete)
        {
        	//Temporarily save monster 2 MONSTERS DOWN from the current monster inside of n
            n = m->next->next;
            //Delete the next monster
            dispose_monster(m->next);
            //Set the current monster's next monster to the one 2 MONSTERS DOWN that we saved in n
            m->next = n;
        }
    }
}

int main(void) {
    monster *m; 
    monster_list *ml = new_monster_list();

    m = new_monster("WholeWheat");
    monster_list_add(ml, m);
    m = new_monster("Rye";
    monster_list_add(ml, m);
    m = new_monster("SunDriedTomato");
    monster_list_add(ml, m);

    print_monster_list(ml);

    dispose_monster_list_all(ml);
    return 0;
}
