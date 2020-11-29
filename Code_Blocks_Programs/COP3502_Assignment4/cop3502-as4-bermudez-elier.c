#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include    "leak_detector_c.h"
#include <math.h>
//Elier Bermudez, COP3502, Assignment 2
//Keyword TESTING means a comment is just leftover code to see if something worked up to a point
//Keyword CHECK means I forgot to delete something that I was still working on and was meant to check before turning in
//Keyword CHECKULTRA means I forgot to fix something and my code is probably broken because of it

//NOT ALLOWED TO BE CHANGED
//--------------------------------------------------------
struct item_node_struct
{
    char name[32];
    int count;
    struct item_node_struct *left, *right;
};
typedef struct item_node_struct item_node;

struct tree_name_node_struct
{
    char treeName[32];
    struct tree_name_node_struct *left, *right;
    item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;
//--------------------------------------------------------
//From here things may be changed

//CHECKULTRA: Functions imported from other projects. May be unnecessary.
//--------------------------------------------------------
//Gerber function
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    // t begins at the null sentinel at the end of s.

    t--;

    while ((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0'; // Clobber the character t is pointing at.
        t--;      // Decrement t.
    }
}

//Gerber function
void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}
//--------------------------------------------------------

//item_node functions
item_node *new_item_node(char* input_name, int input_count)
{
    item_node *item = malloc(sizeof(item_node));
    //CHECKULTRA: This way of assigning the name might not actually work
    strcpy(item->name, input_name);
    item->count = input_count;
    item->left = NULL;
    item->right = NULL;
    return item;
}

void item_set_left_child(item_node *parent, item_node *child)
{
    parent->left = child;
}

void item_set_right_child(item_node *parent, item_node *child)
{
    parent->right = child;
}

//CHECKULTRA: These remove functions might be unnecessary since the deletion function should cover everything
//Note: This remove function will completely sever the link to the left child, only use it when the left child is a leaf
item_node *item_remove_left_child(item_node *parent)
{
    item_node *item = parent->left;
    parent->left = NULL;
    return item;
}

//Note: This remove function will completely sever the link to the right child, only use it when the right child is a leaf
item_node *item_remove_right_child(item_node *parent)
{
    item_node *item = parent->right;
    parent->right = NULL;
    return item;
}

item_node *item_bst_insert(item_node *parent, item_node *new_node)
{
    //If new_node's name comes before the parent node
    if(strcmp(new_node->name, parent->name)<1)
    {
        //Recurse if there is already something to the parent's left
        if(parent->left != NULL)
        {
            return item_bst_insert(parent->left, new_node);
        }
        //Otherwise, insert it to the parent's left
        else
        {
            item_set_left_child(parent, new_node);
            return new_node;
        }
    }
    //If new_node's name comes after the parent node
    else
    {
        //Recurse if there is already something to the parent's right
        if(parent->right != NULL)
        {
            return item_bst_insert(parent->right, new_node);
        }
        //Otherwise, insert it to the parent's right
        else
        {
            item_set_right_child(parent, new_node);
            return new_node;
        }
    }
}
//We don't really search by value, we search by name.
//Just use strcmp and if it's 0 they're equal
item_node *item_bst_find(item_node *parent, char* input_name)
{
    if(parent == NULL)
    {
        return NULL;
    }
    else if(strcmp(input_name, parent->name)==0)
    {
        return parent;
    }
    else if(strcmp(input_name, parent->name)<0)
    {
        return item_bst_find(parent->left, input_name);
    }
    else
    {
        return item_bst_find(parent->right, input_name);
    }
}
//CHECKULTRA: These prints aren't actually how the output should be printed at all
//Only use them for testing!
//Gerber Function
void print_indent(int depth)
{
    for(int i = 0; i < depth; i++)
    {
        printf(" ");
    }
}

//Gerber Function
void item_print_tree_inorder(item_node *item, int depth)
{
    if(item->left)
    {
        print_indent(depth);
        printf("descending left...\n");
        item_print_tree_inorder(item->left, depth + 1);
    }

    print_indent(depth);
    printf("Name and count: %s %d\n", item->name, item->count);

    if(item->right)
    {
        print_indent(depth);
        printf("descending right...\n");
        item_print_tree_inorder(item->right, depth + 1);
    }
}

//tree_name_node functions
tree_name_node *new_tree_name_node(char* input_name)
{
    tree_name_node *tree = malloc(sizeof(tree_name_node));
    //CHECKULTRA: This way of assigning the name might not actually work
    strcpy(tree->treeName, input_name);
    tree->theTree = NULL;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

void tree_name_set_left_child(tree_name_node *parent, tree_name_node *child)
{
    parent->left = child;
}

void tree_name_set_right_child(tree_name_node *parent, tree_name_node *child)
{
    parent->right = child;
}

//CHECKULTRA: These remove functions might be unnecessary since the deletion function should cover everything
//Note: This remove function will completely sever the link to the left child, only use it when the left child is a leaf
tree_name_node *tree_name_remove_left_child(tree_name_node *parent)
{
    tree_name_node *tree = parent->left;
    parent->left = NULL;
    return tree;
}

//Note: This remove function will completely sever the link to the right child, only use it when the right child is a leaf
tree_name_node *tree_name_remove_right_child(tree_name_node *parent)
{
    tree_name_node *tree = parent->right;
    parent->right = NULL;
    return tree;
}

tree_name_node *tree_name_bst_insert(tree_name_node *parent, tree_name_node *new_node)
{
    //If new_node's name comes before the parent node
    if(strcmp(new_node->treeName, parent->treeName)<1)
    {
        //Recurse if there is already something to the parent's left
        if(parent->left != NULL)
        {
            return tree_name_bst_insert(parent->left, new_node);
        }
        //Otherwise, insert it to the parent's left
        else
        {
            tree_name_set_left_child(parent, new_node);
            return new_node;
        }
    }
    //If new_node's name comes after the parent node
    else
    {
        //Recurse if there is already something to the parent's right
        if(parent->right != NULL)
        {
            return tree_name_bst_insert(parent->right, new_node);
        }
        //Otherwise, insert it to the parent's right
        else
        {
            tree_name_set_right_child(parent, new_node);
            return new_node;
        }
    }
}
//We don't really search by value, we search by name.
//Just use strcmp and if it's 0 they're equal
tree_name_node *tree_name_bst_find(tree_name_node *parent, char* input_name)
{
    if(parent == NULL)
    {
        return NULL;
    }
    else if(strcmp(input_name, parent->treeName)==0)
    {
        return parent;
    }
    else if(strcmp(input_name, parent->treeName)<0)
    {
        return tree_name_bst_find(parent->left, input_name);
    }
    else
    {
        return tree_name_bst_find(parent->right, input_name);
    }
}

//Gerber Function
void tree_name_print_tree_inorder(tree_name_node *tree, int depth)
{
    if(tree->left)
    {
        print_indent(depth);
        printf("descending left...\n");
        tree_name_print_tree_inorder(tree->left, depth + 1);
    }

    print_indent(depth);
    printf("Name: %s\n", tree->treeName);

    if(tree->right)
    {
        print_indent(depth);
        printf("descending right...\n");
        tree_name_print_tree_inorder(tree->right, depth + 1);
    }
}
int main(void)
{
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("cop3502-as4-input.txt", "r"); //Opens cop3502-as4-input.txt for reading
    ofp = fopen("cop3502-as4-output-bermudez-elier.txt", "w"); //Creates/Opens cop3502-as4-output-bermudez-elier.txt for writing

    //Scan in nTrees, nItems, and nCommands
    int nTrees, nItems, nCommands, i = 0;
    fscanf(ifp, "%d %d %d", &nTrees, &nItems, &nCommands);
    //We need to create the root then add to that root.
    //But first what we do in every loop is to make a tree node.
    //Setup top half of tree
    for(i=0; i<nTrees; i++)
    {
        //Scan in an ntrees amount of lines, make a tree of trees out of them (first hard step, basic tree implementation at this point)
    }

    //NOTE: You can put bottom items in a bottom item tree and top items in a top item tree,
    //but so far there is not actually any implementation for putting

    /*
    //TESTING: item_node tree works
    item_node *item = new_item_node("root", 5);
    item_bst_insert(item, new_item_node("bob", 15));
    item_bst_insert(item, new_item_node("carson", 20));
    item_bst_insert(item, new_item_node("cara", 5));
    item_bst_insert(item, new_item_node("boa", 3));
    item_bst_insert(item, new_item_node("wilson", 10));
    //item_print_tree_inorder(item, 0);

    //TESTING: tree_node tree works
    tree_name_node *tree = new_tree_name_node("root");
    tree_name_bst_insert(tree, new_tree_name_node("bob"));
    tree_name_bst_insert(tree, new_tree_name_node("carson"));
    tree_name_bst_insert(tree, new_tree_name_node("cara"));
    tree_name_bst_insert(tree, new_tree_name_node("boa"));
    tree_name_bst_insert(tree, new_tree_name_node("wilson"));
    tree_name_print_tree_inorder(tree, 0);
    */

    //CHECKULTRA: Free up your memory here
    /*
    //EXAMPLE OF FREEING UP MEMORY
    void dispose_failfish(failfish *ff)
    {
        ff->fail_number = 0;
        free(ff);
    }
    */
    //Closing scanners
    fclose(ifp);
    fclose(ofp);
}
