/* linkedlists-circular.c - this program demonstrates circular doubly linked lists.

   Matthew Gerber, 2020, for COP3502. */

#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is a monster structure for a circular doubly linked list of monsters.  
   It contains basic information about a monster, as well as a link to a next 
   monster and a previous monster.
   
   next is null if and only if the monster has not been added to a list yet.
   prev is null if and only if the monster has not been added to a list yet.

   */

struct monster_struct {
    char *name;
    char *type;
    int commonality;
    struct monster_struct *next;
    struct monster_struct *prev;
};

typedef struct monster_struct monster;

/* This is a structure for a doubly linked list of monsters with head and tail nodes.
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
    m->prev = NULL;

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
    if(m->prev == NULL) {
        printf("    No link back\n");
    } else {
        printf("    Link back to: %s\n", m->prev->name);
    }
    if(m->next == NULL) {
        printf("    No link forward\n");
    } else {
        printf("    Link forward to: %s\n", m->next->name);
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

    /* First, check if the list is empty.  If it is, we just have to dispose the shell. */

    if(m == NULL) 
    {
      dispose_monster_list(ml);
      return;
    }

    /* We now know the list isn't empty, so we're safe freeing the first element... */

    do {
        n = m->next;
        dispose_monster(m);
        m = n;
    } while(m != ml->head); // at the end ml->head is a dangling pointer, but we can
                            // still compare it as long as we don't dereference it

    dispose_monster_list(ml);
}

/* Print every monster in a list of monsters. */

void print_monster_list(monster_list *ml)
{
    monster *m = ml->head;

    /* First, check if the list is empty.  If it is, we just return. */

    if(m == NULL) {
      return;
    }

    /* We now know the list isn't empty, so we're safe printing the first element... */

    do {
        print_monster(m);
        m = m->next;
    } while(m != ml->head);
}

/* Add monster m to list ml.  m must already be an allocated monster and must *not* be
   a member of ml or any other monster_list. */

void monster_list_add(monster_list *ml, monster *new_monster)
{
    if(ml->head == NULL)
    {
        /* If we're adding to an empty list, just set the head equal to the tail equal
           to the new monster. */

        ml->head = new_monster;
        ml->tail = new_monster;
        new_monster->prev = new_monster;
        new_monster->next = new_monster;
    } else {
        /* If the list isn't empty, there's a head, so... */

        new_monster->prev = ml->tail;
        new_monster->next = ml->head;

        ml->head->prev = new_monster;
        ml->tail->next = new_monster;

        ml->head = new_monster;
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
        new_monster->prev = new_monster;
        new_monster->next = new_monster;
    } else {
        /* If the list isn't empty, there's a tail, so... */

        new_monster->prev = ml->tail;
        new_monster->next = ml->head;

        ml->head->prev = new_monster;
        ml->tail->next = new_monster;

        ml->tail = new_monster;
    }
}

/* If dispose, frees m_to_delete.  Otherwise, set both its links to null. */

void clear_links_or_dispose(monster *m_to_delete, int dispose)
{
  if(dispose != 0) {
    dispose_monster(m_to_delete);
  } else {
    m_to_delete->next = NULL;
    m_to_delete->prev = NULL;
  }
}

/* Delete a monster from a list of monsters.  Always removes it from the list; if
   dispose, also disposes the monster structure from memory. */

void monster_list_delete(monster_list *ml, monster *m_to_delete, int dispose)
{
    monster *m = ml->head;
    monster *n;

    /* First thing to check is if this will empty the list. */

    if(m_to_delete->next == m_to_delete) // We could also use if(ml->head == ml->tail)
    {
      clear_links_or_dispose(m_to_delete, dispose);
      ml->head = NULL;
      ml->tail = NULL;
      return;
    }

    /* We now know we're not deleting the only item in the list.  To make this less 
       messy, we're going to repair the head and/or tail pointers *before* the actual
       linkbreaking and deletion. */

    if(ml->head == m_to_delete)
    {
      ml->head = m_to_delete->next;
    }

    if(ml->tail == m_to_delete) 
    {
      ml->tail = m_to_delete->prev;
    }

    /* We now know we're deleting an item that is not the only item in the list, and
       that is neither the head nor the tail.  We don't have to worry about head or
       tail pointers, just about our next and prev pointers - which makes the link
       repair really easy. */

    m_to_delete->prev->next = m_to_delete->next;
    m_to_delete->next->prev = m_to_delete->prev;

    /* Nothing in the list points to us any more, so we can now safely delete. */

    clear_links_or_dispose(m_to_delete, dispose);
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