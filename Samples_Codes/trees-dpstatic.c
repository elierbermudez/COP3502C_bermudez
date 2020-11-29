#include <stdio.h>
#include <stdlib.h>

/* Here we use what a whole lot of programs actually use for trees. */

#define MAX_CHILDREN 8

struct tree_node_struct {
  int payload;
  struct tree_node_struct *parent;
  struct tree_node_struct *children[MAX_CHILDREN];
  unsigned int nchildren; // Can never be > 8
};

typedef struct tree_node_struct tree_node;

tree_node *new_tree_node(int p) {
  tree_node *t = malloc(sizeof(tree_node));
  t->payload = p;
  t->parent = NULL;
  for(int i = 0; i < MAX_CHILDREN; i++)
  {
    t->children[i] = NULL;
  }
  t->nchildren = 0;
  return t;
}

void add_child(tree_node *parent, tree_node *child) {
  if(parent->nchildren == MAX_CHILDREN)
  {
    return;
  }
  parent->nchildren++;
  parent->children[parent->nchildren - 1] = child;
  child->parent = parent;
}

tree_node *remove_child(tree_node *parent, tree_node *child) {
  int removed = 0;

  for(int i = 0; i < parent->nchildren; i++) {
    if(parent->children[i] == child) {
      removed = 1;
    } else if(removed) {
      parent->children[i-1] = parent->children[i];
    }
  }

  if(removed) {
    parent->nchildren--;
    return child;
  } else {
    return NULL;
  }
}

void print_tree(tree_node *t) {
  printf("payload: %d\n", t->payload);
  if(t->nchildren) {
    printf("descending...\n");
    for(int i = 0; i < t->nchildren; i++) {
      print_tree(t->children[i]);
    }      
  }
}

int main(void) {
  tree_node *t = new_tree_node(1);

  add_child(t, new_tree_node(2));
  add_child(t, new_tree_node(3));
  add_child(t, new_tree_node(4));
  add_child(t, new_tree_node(5));
  print_tree(t);

  remove_child(t, t->children[2]);
  print_tree(t);

  return 0;
}