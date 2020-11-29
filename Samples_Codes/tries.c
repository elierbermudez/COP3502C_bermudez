#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tries.c - string reTRIEval tree. */

// We'll only store lowercase letters.
#define MAX_CHILDREN 26

int which_child(char c) {
  if(isalpha(c)) {
    char lc = tolower(c);
    return lc - 'a';       // converts a to 0, b to 1, etc.
  } else {
    return -1;
  }
}

struct trie_node_struct {
  int is_word;
  int which_child;
  struct trie_node_struct *parent;
  struct trie_node_struct *children[MAX_CHILDREN];
  unsigned int nchildren; // Can never be > 26
};

typedef struct trie_node_struct trie_node;

trie_node *new_trie_node(int which_child) {
  trie_node *t = malloc(sizeof(trie_node));
  t->is_word = 0;
  t->parent = NULL;
  t->which_child = which_child;
  for(int i = 0; i < MAX_CHILDREN; i++)
  {
    t->children[i] = NULL;
  }
  t->nchildren = 0;
  return t;
}

void add_child_at(trie_node *parent, trie_node *child, int which_child) {
  if(parent->children[which_child] == NULL) {
    parent->nchildren++;
    parent->children[which_child] = child;
    child->parent = parent;
  } else {
    printf("Hm.  That shouldn't be.\n");
  }
}

void add_word(trie_node *root, char *word) {
  int l = strlen(word);

  printf("Trying to add the word %s.\n", word);

  if(l == 0) {
    root->is_word = 1;
  } else {
    char c = word[0];
    char nc = which_child(c);

    /* If c isn't a letter, ignore it.  So "alice-bob" is the same as
       "alicebob" for our purposes. */
    if(nc == -1) {
      add_word(root, word + 1);
      return;
    }

    trie_node *next = root->children[nc];

    /* If we don't have a child node for the letter c, we need to create
       it. */
    if(next == NULL) {
      printf("  I don't have a child for %c.  Making one.\n", c);
      next = new_trie_node(nc);
      add_child_at(root, next, nc);
    }

    // printf("  Recursing down to child for %c.\n", c);
    add_word(next, word + 1);
  }
}

int is_word(trie_node *root, char *word) {
  int l = strlen(word);

  printf("Trying to find the word %s.\n", word);

  if(l == 0) {
    printf("  %s might be a word!  %s\n", word, 
           root->is_word ? "IT IS!" : "But it's not.");
    return root->is_word;
  } else {
    char c = word[0];
    char nc = which_child(c);

    /* If c isn't a letter, ignore it.  So "alice-bob" is the same as
       "alicebob" for our purposes. */
    if(nc == -1) {
      return is_word(root, word + 1);
    }

    trie_node *next = root->children[nc];

    /* If we don't have a child node for the letter c, we need to create
       it. */
    if(next == NULL) {
      printf("  %s is not a word.\n", word);
      return 0;
    }

    // printf("  Recursing down to child for %c.\n", c);
    return is_word(next, word + 1);
  }
}

void actually_delete(trie_node *child) {
  trie_node *parent = child->parent;

  if(child->is_word || child->nchildren > 0 || parent == NULL) {
    return;
  } else {
    // This node has no children and is not the root.  Delete it.
    parent->nchildren--;
    parent->children[child->which_child] = NULL;
    free(child);
    printf("    Deleted node\n");

    // Now, we need to keep going upward and keep deleting *IF* the
    // parent is eligible for deletion.
    actually_delete(parent);
  }
}

void delete_word(trie_node *root, char *word) {
  int l = strlen(word);

  printf("Trying to find the word %s to delete.\n", word);

  if(l == 0) {
    printf("  Okay, we found it.  Is it a word?  %s\n", 
           root->is_word ? "Yes." : "No.");
    if(root->is_word) {
      root->is_word = 0;
      actually_delete(root);
      printf("  Deleted.\n");
    } else {
      printf("  Nothing to delete.\n");
      return;
    }
  } else {
    char c = word[0];
    char nc = which_child(c);

    /* If c isn't a letter, ignore it.  So "alice-bob" is the same as
       "alicebob" for our purposes. */
    if(nc == -1) {
      delete_word(root, word + 1);
    }

    trie_node *next = root->children[nc];

    /* If we don't have a child node for the letter c, we need to create
       it. */
    if(next == NULL) {
      printf("  Nothing to delete.\n");
      return;
    }

    // printf("  Recursing down to child for %c.\n", c);
    delete_word(next, word + 1);
  }
}

int main(void) {
  trie_node *root = new_trie_node(-1);

  add_word(root, "hello");
  printf("\n");
  add_word(root, "he");
  printf("\n");
  (void) is_word(root, "he");
  printf("\n");
  (void) is_word(root, "hello");
  printf("\n");
  delete_word(root, "he");
  printf("\n");
  (void) is_word(root, "he");
  printf("\n");
  (void) is_word(root, "hello");
  printf("\n");
  delete_word(root, "he");
  printf("\n");
  delete_word(root, "hello");
  printf("\n");

  return 0;
}