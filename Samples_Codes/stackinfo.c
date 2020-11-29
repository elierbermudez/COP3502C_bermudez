/* A stack is any data structure that supports a FILO/LIFO list with the ability to push and
   pop to and from the top of the list.  It's also easy to implement a stack with a linked
   list - you just add to the head and also remove from the head. */

void push(stack *st, item *it);
item *pop(stack *st);
item *stack_peek(stack *st);
int is_empty(stack *st);
