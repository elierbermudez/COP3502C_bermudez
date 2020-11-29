/* A queue is any data structure that supports a FIFO list with the ability to enqueue at the
   back and dequeue from the front.  It's easy to implement a queue with a linked list - you
   just add to the head and remove from the tail (or vice versa)... */

/* Your failfish queue is going to be a circular linked list.  You may use our circular linked
   list work from class as a template, but you'll need to implement (at least) the following
   functions yourself.... */

void enqueue(failfish_queue *q, failfish *f);

    // Add f to the tail of q.

failfish *dequeue(failfish_queue *q);

    // Pull an item off the head of q - don't delete it - and return it.

failfish *queue_peek(failfish_queue *q);

    // Look at the item on the head of q - don't delete it or remove it - and return it.

int is_empty(failfish_queue *q);

    // Look to see if q->head == NULL.

/* In terms of assignment 2, you will actually be accessing your failfish_queue as a circular
   linked list in the first course, and as a queue in the second course.  Using the circular
   linked list to implement the queue allows you to do both at once. */
