#include <stdio.h>
#include <stdlib.h>

struct tree_node_struct {
  int payload;
  struct tree_node_struct *parent;
  struct tree_node_struct **children;
  unsigned int nchildren;
};

typedef struct tree_node_struct tree_node;

tree_node *new_tree_node(int p) {
  tree_node *t = malloc(sizeof(tree_node));
  t->payload = p;
  t->parent = NULL;
  t->children = NULL;
  t->nchildren = 0;
  return t;
}

void add_child(tree_node *parent, tree_node *child) {
  parent->nchildren++;
  parent->children = realloc(parent->children, parent->nchildren * sizeof(tree_node));
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
    if(parent->nchildren) {
      parent->children = realloc(parent->children, parent->nchildren * sizeof(tree_node));
    } else {
      free(parent->children);
      parent->children = NULL;
    }
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