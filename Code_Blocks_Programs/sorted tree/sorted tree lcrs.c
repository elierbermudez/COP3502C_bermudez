#include <stdio.h>
#include <stdlib.h>

/* Here we're going to use a modified linked-list concept to store a
   tree: we're going to store the leftmost child and the right sibling. */

struct tree_node_struct
{
    int payload;
    struct tree_node_struct *child;
    struct tree_node_struct *sibling;
    unsigned int nchildren;
};

typedef struct tree_node_struct tree_node;

tree_node *new_tree_node(int p)
{
    tree_node *t = malloc(sizeof(tree_node));
    t->payload = p;
    t->child = NULL;
    t->sibling = NULL;
    t->nchildren = 0;
    return t;
}

//Write a function to add a child to a parent, but maintain the property that the children
//of any given parent are sorted by payload value in increasing order.

void add_child(tree_node *parent, tree_node *child)
{
    parent->nchildren++;
    //If the list is empty, add the child in
    if(parent->child == NULL)
    {
        parent->child = child;
    }
    else
        //If the new child is less than the first child, simply place it on the left.
        if(parent->child->payload > child->payload)
        {
            child->sibling = parent->child;
            parent->child = child;
            return;
        }
    //If not, look for the first child that our new child is less than
        else
        {
            tree_node *current_child = parent->child->sibling;
            tree_node *previous_child = parent->child;
            //Continue as long as our current child has a sibling
            //This is a do while to prevent it from breaking when the number of children is 2
            do
            {
                if(current_child->payload > child->payload)
                {
                    //Set the previous_child's sibling to be the new child
                    previous_child->sibling = child;
                    //Set the current child's sibling to be the current_child
                    child->sibling = current_child;
                    return;
                }
                else
                {
                    previous_child = current_child;
                    current_child = current_child->sibling;
                }
            }
            while(current_child->sibling!=NULL);
            //If the new child was not less than anything else, we place it at the end
            previous_child->sibling = child;
        }

}

tree_node *remove_child(tree_node *parent, tree_node *child)
{
    int removed = 0;

    // This is just a linked list removal function with parent->child
    // acting as the head.

    if(child == parent->child)
    {
        parent->child = child->sibling;
        removed = 1;
    }
    else
    {
        tree_node *find = parent->child;

        while(find->sibling != NULL)
        {
            if(find->sibling == child)
            {
                find->sibling = find->sibling->sibling;
                removed = 1;
            }
            find = find->sibling;
        }
    }

    if(removed)
    {
        parent->nchildren--;
        return child;
    }
    else
    {
        return NULL;
    }
}

/* This will dispose of a node *and* all its children.  Don't use this
   on a node you stlil have on your tree or bad things will happen.  you
   *can* use this to get rid of a whole tree. */

void dispose_node(tree_node *n)
{
    tree_node *child = n->child;

    while(child)
    {
        tree_node *next = child->sibling;
        dispose_node(child);
        child = next;
    }

    free(n);
}

void remove_and_dispose(tree_node *parent, tree_node *child)
{
    tree_node *ret = remove_child(parent, child);
    if(ret)
    {
        dispose_node(ret);
    }
}

void print_tree(tree_node *t, int depth)
{
    for(int i = 0; i < depth; i++)
    {
        printf(" ");
        printf(" ");
        printf(" ");
        printf(" ");
    }
    printf("payload: %d\n", t->payload);
    if(t->nchildren)
    {
        for(int i = 0; i < depth; i++)
        {
            printf(" ");
            printf(" ");
            printf(" ");
            printf(" ");
        }
        printf("  descending...\n");
        tree_node *n = t->child;
        while(n)   // Same as while(n != NULL)
        {
            print_tree(n, depth + 1);
            n = n->sibling;
        }
    }
}

int main(void)
{
    //t is some node with value 1
    tree_node *t = new_tree_node(1);
    tree_node *n;
    tree_node *to_delete;
    //n is some node with value 19
    n = new_tree_node(19);
    //19 is now 1's kid
    add_child(t, n);

    //print_tree(t, 0);

    //n is some node with value 3
    n = new_tree_node(3);
    //6 is now 3's kid
    add_child(n, new_tree_node(20));
    //7 is now 3's kid
    add_child(n, new_tree_node(7));
    //3 is now 1's kid
    add_child(n, new_tree_node(13));
    add_child(n, new_tree_node(0));
    add_child(t, n);
    //So far it's
    //1
    //2   3
    //(blank) (6, 7)

    //n is some node with value 17
    n = new_tree_node(17);
    //4 is marked for deletion for later
    to_delete = n;
    //4 is now 1's kid
    add_child(t, n);

    //So far it's
    //1
    //2   3   4
    //(blank) (6, 7)

    //n is some node with value 5
    n = new_tree_node(5);
    //5 is now 1's kid
    add_child(t, n);

    print_tree(t, 0);

    remove_and_dispose(t, to_delete);
    //print_tree(t, 0);

    dispose_node(t);

    return 0;

    /*
        //t is some node with value 1
    tree_node *t = new_tree_node(1);
    tree_node *n;
    tree_node *to_delete;
      //n is some node with value 2
    n = new_tree_node(2);
      //2 is now 1's kid
    add_child(t, n);

      //n is some node with value 3
    n = new_tree_node(3);
      //6 is now 3's kid
    add_child(n, new_tree_node(6));
      //7 is now 3's kid
    add_child(n, new_tree_node(7));
      //3 is now 1's kid
    add_child(t, n);
    //So far it's
    //1
    //2   3
    //(blank) (6, 7)

      //n is some node with value 4
    n = new_tree_node(4);
      //4 is marked for deletion for later
    to_delete = n;
      //4 is now 1's kid
    add_child(t, n);

      //So far it's
    //1
    //2   3   4
    //(blank) (6, 7)

      //n is some node with value 5
    n = new_tree_node(5);
      //5 is now 1's kid
    add_child(t, n);
      //So far it's
    //1
    //2   3   4   5
    //(blank) (6, 7)  (blank) (blank)


    print_tree(t, 0);

    remove_and_dispose(t, to_delete);
    //print_tree(t, 0);

    dispose_node(t);

    return 0;
    */
}
