/* linkedlists-tail.c - this program demonstrates linked lists with tail pointers.

   Matthew Gerber, 2020, for COP3502. */

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
    char *type;
    int commonality;
    struct monster_struct *next;
};

typedef struct monster_struct monster;

/* This is a structure for a singly linked list of monsters with head and tail nodes.
   head and tail are NULL if and only if the list is empty. */

struct monster_list_struct {
    monster *head;
    monster *tail;
};

typedef struct monster_list_struct monster_list;

/* ------ Functions for monsters. */

/* Allocate and return a new monster. */

monster *new_monster(char *name, char *type, int commonality)
{
    monster *m;

    m = malloc(sizeof(monster));
    m->name = strdup(name);
    m->type = strdup(type);
    m->commonality = commonality;
    m->next = NULL;

    return m;
}

/* Destructor for monster. */

void dispose_monster(monster *m)
{
    free(m->name);
    free(m->type);
    free(m);
}

/* Print out basic info for a monster. */

void print_monster(monster *m)
{
    printf("  Monster %s of type %s with commonality %d\n", m->name, m->type, m->commonality);
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
    ml->tail = NULL;

    return ml;
}

/* Dispose of a list of monsters. */

void dispose_monster_list(monster_list *ml)
{
    free(ml);
}

/* Dispose of a list of monsters *and* all the monsters in it. */

void dispose_monster_list_all(monster_list *ml)
{
    monster *m = ml->head;
    monster *n;

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
   a member of ml or any other monster_list. */

void monster_list_add(monster_list *ml, monster *new_monster)
{
    new_monster->next = ml->head;
    ml->head = new_monster;

    /* tail points at the final item in the list, and when adding to the front of a
       list that already has items, the final item doesn't change.  So tail changes
       only if we added to an empty list. */
    
    if(ml->tail == NULL)
    {
        ml->tail = new_monster;
    }
}

/* Add monster m to the tail of list ml. m must already be an allocated monster and 
   must *not* be a member of ml or any other monster_list.  */

void monster_list_add_tail(monster_list *ml, monster *new_monster)
{
    if(ml->tail == NULL)
    {
        /* If we're adding to an empty list, just set the head equal to the tail equal
           to the new monster. */

        ml->head = new_monster;
        ml->tail = new_monster;
    } else {
        /* If the list isn't empty, there's a tail, so... */

        ml->tail->next = new_monster;
        ml->tail = new_monster;
    }
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

        /* If we're deleting the head, tail only changes if we just emptied the
           list.  This is the only place where we have to worry about emptying
           the list, since the only element of a list is always both the head 
           and the tail. */

        if(ml->head == NULL) {
            ml->tail = NULL;
        }

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
        if(m->next == m_to_delete)
        {
            n = m->next->next;
            dispose_monster(m->next);
            m->next = n;

            /* If we just deleted the tail, we need to repair it.  Both these pointers
               are dead, but they still have the same *values*, so we can get away with
               comparing them as long as we don't dereference them. */

            if(m_to_delete == ml->tail)
            {
                /* We deleted m->next, and the previous item from m->next is m, so we
                   can just set tail to m. */

                ml->tail = m;
            }
        }
    }
}

int main(void) {
    monster *m; 
    monster_list *ml = new_monster_list();

    m = new_monster("WholeWheat", "Bread", 10);
    monster_list_add(ml, m);
    m = new_monster("Rye", "Bread", 8);
    monster_list_add(ml, m);
    m = new_monster("SunDriedTomato", "Bread", 3);
    monster_list_add(ml, m);

    print_monster_list(ml);

    dispose_monster_list_all(ml);
    return 0;
}