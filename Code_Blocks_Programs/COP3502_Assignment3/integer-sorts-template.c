//Elier Bermudez, COP3502 Fall 2020, Assignment3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

struct stack_node_struct {
    int payload;
    struct stack_node_struct *next;
};

typedef struct stack_node_struct stack_node;

typedef struct {
    stack_node *head;
}stack;

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

void push(stack *st, int i){
    stack_node *internal_node = new_stack_node(i);

    if(st->head == NULL){
        st->head = internal_node;
    } else{
        //This might not work. Test it out.
        internal_node->next = st->head;
        st->head = internal_node;
    }
}

int pop(stack *st){
    stack_node *internal_node = st->head;
    if(internal_node == NULL){
        return -1;
    }
    else{
        st->head = st->head->next;
        return internal_node;
    }
}

int stack_peek(stack *st){
    return st->head->payload;
}

int is_empty(stack *st){
    if(st->head == NULL){
        return 1;
    } else{
    return 0;}
}

int main(void)
{
    stack *test_stack = new_stack();
    push(test_stack, 1);
    push(test_stack, 2);
    push(test_stack, 3);
    push(test_stack, 4);

    /*
  while(is_empty(test_stack) == 0) {
    item *it = dequeue(q);
    printf("  Item value: %d\n", it->value);
    dispose_item(it);
  }
  */
  //This should print 4321 since stacks reverse your input order, and it does!
    printf("Stack payload 1: %d\n", test_stack->head->payload);
    printf("Stack payload 2: %d\n", test_stack->head->next->payload);
    printf("Stack payload 3: %d\n", test_stack->head->next->next->payload);
    printf("Stack payload 4: %d\n\n", test_stack->head->next->next->next->payload);

    pop(test_stack);
    printf("Stack payload 1: %d\n", test_stack->head->payload);
    printf("Stack payload 2: %d\n", test_stack->head->next->payload);
    printf("Stack payload 3: %d\n", test_stack->head->next->next->payload);
    printf("Stack payload 4: %d\n", test_stack->head->next->next->next->payload);

}
