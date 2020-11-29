//Elier Bermudez, COP3502 Fall 2020, Assignment3

#include <stdio.h>
#include <stdlib.h>
#include    "leak_detector_c.h"

struct stack_node_struct
{
    int payload;
    struct stack_node_struct *next;
};

typedef struct stack_node_struct stack_node;

typedef struct
{
    stack_node *head;
} stack;

//The instructions confused me slightly, not sure if you wanted just those 4 functions or fully working codd.
//Here are some "extra" functions which are necessary to make the code work

stack_node *new_stack_node(int input_payload)
{
    stack_node *internal_node = malloc(sizeof(stack_node));
    internal_node->payload = input_payload;
    internal_node->next = NULL;

    return internal_node;
}

stack *new_stack(void)
{
    stack *internal_stack = malloc(sizeof(stack));
    internal_stack->head = NULL;

    return internal_stack;
}

//END--------------------------------

void push(stack *st, int i)
{
    stack_node *internal_node = new_stack_node(i);

    if(st->head == NULL)
    {
        st->head = internal_node;
    }
    else
    {
        internal_node->next = st->head;
        st->head = internal_node;
    }
}

int pop(stack *st)
{
    if(is_empty(st)==1)
    {
        return -1;
    }
    else
    {
        stack_node *internal_node = st->head;
        int internal_payload = 0;

        //Moving things around
        st->head = st->head->next;
        internal_payload = internal_node->payload;

        //Freeing stuff
        internal_node->next = NULL;
        internal_node->payload = 0;
        free(internal_node);

        return internal_payload;
    }
}

int stack_peek(stack *st)
{
    return st->head->payload;
}

int is_empty(stack *st)
{
    if(st->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(void)
{
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has

    //Create a new stack and push 1, 2, 3, and 4 onto it
    stack *test_stack = new_stack();
    push(test_stack, 1);
    push(test_stack, 2);
    push(test_stack, 3);
    push(test_stack, 4);

    //Print out the stack. Returns 4, 3, 2, then 1 since stacks reverse things. A print_stack function might be
    //useful here but it was not asked for.

    printf("Stack payload 1: %d\n", test_stack->head->payload);
    printf("Stack payload 2: %d\n", test_stack->head->next->payload);
    printf("Stack payload 3: %d\n", test_stack->head->next->next->payload);
    printf("Stack payload 4: %d\n\n", test_stack->head->next->next->next->payload);

    //Pops everything from the stack and prints out it's payload as well.
    while(is_empty(test_stack) == 0)
    {
        printf("Stack payload: %d\n", pop(test_stack));
    }

    //Frees the test_stack
    test_stack->head = NULL;
    free(test_stack);
}
