#include <stdio.h>
#include <stdlib.h>

/* binary-trees.c - basics of binary trees. */

//This is only different from the actual code in that you have an int payload instead of a char name
struct tree_node_struct {
  int payload;
  struct tree_node_struct *parent;
  struct tree_node_struct *left;
  struct tree_node_struct *right;
};

typedef struct tree_node_struct tree_node;

tree_node *new_tree_node(int p) {
  tree_node *t = malloc(sizeof(tree_node));
  t->payload = p;
  t->parent = NULL;
  t->left = NULL;
  t->right = NULL;
  return t;
}

void set_left_child(tree_node *parent, tree_node *child) {
  parent->left = child;
  child->parent = parent;
}

void set_right_child(tree_node *parent, tree_node *child) {
  parent->right = child;
  child->parent = parent;
}

tree_node *remove_left_child(tree_node *parent) {
  tree_node *n = parent->left;
  parent->left = NULL;
  return n;
}

tree_node *remove_right_child(tree_node *parent) {
  tree_node *n = parent->right;
  parent->right = NULL;
  return n;
}

/* Insert a node into a binary search tree. */

tree_node *bst_insert(tree_node *parent, tree_node *new_node) {
  if(new_node->payload < parent->payload) { // value <, go left
    if(parent->left != NULL) {              // node exists?
      return bst_insert(parent->left, new_node); // recurse
    } else {
      set_left_child(parent, new_node);     // insert here
      return new_node;                      // we're done
    }
  } else {                                  // value >=, go right
    if(parent->right != NULL) {             // node exists?
      return bst_insert(parent->right, new_node); // recurse
    } else {
      set_right_child(parent, new_node);    // insert here
      return new_node;                      // we're done
    }
  }
}

/* Find a value in a binary search tree. */

tree_node *bst_find(tree_node *parent, int value) {
  if(parent == NULL) {
    return NULL;
  } else if(value == parent->payload) {
    return parent;
  } else if(value < parent->payload) {
    return bst_find(parent->left, value);
  } else {
    return bst_find(parent->right, value);
  }
}

void print_indent(int depth)
{
  for(int i = 0; i < depth; i++) {
    printf(" ");
  }
}

void print_tree_inorder(tree_node *t, int depth) {
  if(t->left) {
    print_indent(depth);
    printf("descending left...\n");
    print_tree_inorder(t->left, depth + 1);
  }

  print_indent(depth);
  printf("payload: %d\n", t->payload);

  if(t->right) {
    print_indent(depth);
    printf("descending right...\n");
    print_tree_inorder(t->right, depth + 1);
  }
}

int main(void) {
  tree_node *t = new_tree_node(10);

  bst_insert(t, new_tree_node(10));
  bst_insert(t, new_tree_node(5));
  bst_insert(t, new_tree_node(7));
  bst_insert(t, new_tree_node(1));

  print_tree_inorder(t, 0);

  return 0;
}
