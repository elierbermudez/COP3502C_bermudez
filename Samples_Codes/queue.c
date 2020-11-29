/* queue.c - extremely basic queue example */

#include <stdio.h>
#include <stdlib.h>

struct item_struct {
  int value;
  struct item_struct *next;
};

typedef struct item_struct item;

typedef struct {
  item *head;
  item *tail;
} item_queue;

item *new_item(int value)
{
  item *it = malloc(sizeof(item));

  it->value = value;
  it->next = NULL;
  return it;
}

void dispose_item(item *it)
{
  free(it);
}

item_queue *new_item_queue(void)
{
  item_queue *q = malloc(sizeof(item_queue));

  q->head = NULL;
  q->tail = NULL;

  return q;
}

void dispose_item_queue(item_queue *q)
{
  free(q);
}

/* To enqueue, we just add to the tail. */

void enqueue(item_queue *q, item *it)
{
  // Make sure our item's next pointer isn't garbage
  it->next = NULL;

  // Is the list empty?
  if(q->head == NULL) {
    // The list is empty.  Set head = tail = new.
    q->head = it;
    q->tail = it;
  } else {
    // The list is not empty.  Just add new to the end.
    q->tail->next = it;
    q->tail = it;
  }
}

/* To dequeue, we just remove from the head. */

item *dequeue(item_queue *q)
{
  item *it = q->head;

  // Advance the head if the list isn't empty
  if(q->head != NULL) {
    q->head = q->head->next;
  }

  // Did we just empty the list?
  if(q->head == NULL) {
    q->tail = NULL;
  }

  return it;
}

item *peek(item_queue *q)
{
  return q->head;
}

int is_empty(item_queue *q)
{
  if(peek(q) == NULL)
  {
    return 1;
  } else {
    return 0;
  }
}

int main(void)
{
  printf("Hello, world.\n");

  item_queue *q = new_item_queue();

  enqueue(q, new_item(3));
  enqueue(q, new_item(4));
  enqueue(q, new_item(5));
  enqueue(q, new_item(6));
  enqueue(q, new_item(7));
  enqueue(q, new_item(8));

  while(is_empty(q) == 0) {
    item *it = dequeue(q);
    printf("  Item value: %d\n", it->value);
    dispose_item(it);
  }

  return 0;
}