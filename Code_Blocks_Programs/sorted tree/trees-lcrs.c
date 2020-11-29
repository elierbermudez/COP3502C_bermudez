#include <stdio.h>
#include <stdlib.h>

/* Here we're going to use a modified linked-list concept to store a 
   tree: we're going to store the leftmost child and the right sibling. */

struct tree_node_struct {
  int payload;
  struct tree_node_struct *parent;
  struct tree_node_struct *child;
  struct tree_node_struct *sibling;
  unsigned int nchildren;
};

typedef struct tree_node_struct tree_node;

tree_node *new_tree_node(int p) {
  tree_node *t = malloc(sizeof(tree_node));
  t->payload = p;
  t->parent = NULL;
  t->child = NULL;
  t->sibling = NULL;
  t->nchildren = 0;
  return t;
}

void add_child(tree_node *parent, tree_node *child) {
  parent->nchildren++;
  child->sibling = parent->child;
  parent->child = child;
  child->parent = parent;
}

tree_node *remove_child(tree_node *parent, tree_node *child) {
  int removed = 0;

  // This is just a linked list removal function with parent->child
  // acting as the head.

  if(child == parent->child) {
    parent->child = child->sibling;
    removed = 1;
  } else {
    tree_node *find = parent->child;

    while(find->sibling != NULL) {
      if(find->sibling == child) {
        find->sibling = find->sibling->sibling;
        removed = 1;
      }
      find = find->sibling;
    }
  }

  if(removed) {
    parent->nchildren--;
    return child;
  } else {
    return NULL;
  }
}

/* This will dispose of a node *and* all its children.  Don't use this
   on a node you stlil have on your tree or bad things will happen.  you
   *can* use this to get rid of a whole tree. */

void dispose_node(tree_node *n) {
  tree_node *child = n->child;

  while(child) {
    tree_node *next = child->sibling;
    dispose_node(child);
    child = next;
  }

  free(n);
}

void remove_and_dispose(tree_node *parent, tree_node *child) {
  tree_node *ret = remove_child(parent, child);
  if(ret) {
    dispose_node(ret);
  }
}

void print_tree(tree_node *t, int depth) {
  for(int i = 0; i < depth; i++) {
    printf(" ");
  }
  printf("payload: %d\n", t->payload);
  if(t->nchildren) {
    printf("descending...\n");
    tree_node *n = t->child;
    while(n) { // Same as while(n != NULL)
      print_tree(n, depth + 1);
      n = n->sibling;
    }
  }
}

int main(void) {
  tree_node *t = new_tree_node(1);
  tree_node *n;
  tree_node *to_delete;

  n = new_tree_node(2);
  add_child(t, n);

  n = new_tree_node(3);
  add_child(n, new_tree_node(6));
  add_child(n, new_tree_node(7));
  add_child(t, n);

  n = new_tree_node(4);
  to_delete = n;
  add_child(t, n);

  n = new_tree_node(5);
  add_child(t, n);

  print_tree(t, 0);

  remove_and_dispose(t, to_delete);
  print_tree(t, 0);

  dispose_node(t);

  return 0;
}