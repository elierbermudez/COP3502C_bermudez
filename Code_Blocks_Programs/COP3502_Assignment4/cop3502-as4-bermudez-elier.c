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

//item_node functions
item_node *new_item_node(char* input_name, int input_count)
{
    item_node *item = malloc(sizeof(item_node));
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

//CHECK: This remove function may be unnecessary. The proper deletion function should cover everything
//Note: This remove function will completely sever the link to the left child, only use it when the left child is a leaf
item_node *item_remove_left_child(item_node *parent)
{
    item_node *item = parent->left;
    parent->left = NULL;
    return item;
}

//CHECK: This remove function may be unnecessary. The proper deletion function should cover everything
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

void item_print_tree_inorder(item_node *item, FILE *ofp)
{
    if(item->left)
    {
        item_print_tree_inorder(item->left, ofp);
    }
    fprintf(ofp, "%s ", item->name);

    if(item->right)
    {
        item_print_tree_inorder(item->right, ofp);
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

//CHECK: This remove function may be unnecessary. The proper deletion function should cover everything
//Note: This remove function will completely sever the link to the left child, only use it when the left child is a leaf
tree_name_node *tree_name_remove_left_child(tree_name_node *parent)
{
    tree_name_node *tree = parent->left;
    parent->left = NULL;
    return tree;
}

//CHECK: This remove function may be unnecessary. The proper deletion function should cover everything
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

void tree_name_print_tree_inorder(tree_name_node *tree, FILE *ofp)
{
    if(tree->left)
    {
        tree_name_print_tree_inorder(tree->left, ofp);
    }
    fprintf(ofp, "%s ", tree->treeName);

    if(tree->right)
    {
        tree_name_print_tree_inorder(tree->right, ofp);
    }
}

void tree_name_print_tree_inorder_with_items(tree_name_node *tree, FILE *ofp)
{
    if(tree->left)
    {
        tree_name_print_tree_inorder_with_items(tree->left, ofp);
    }

    fprintf(ofp, "===%s===\n", tree->treeName);
    item_print_tree_inorder(tree->theTree, ofp);
    fprintf(ofp, "\n");
    if(tree->right)
    {
        tree_name_print_tree_inorder_with_items(tree->right, ofp);
    }
}

void print_first_half_of_output(tree_name_node *tree, FILE *ofp){
    tree_name_print_tree_inorder(tree, ofp);
    fprintf(ofp, "\n");
    tree_name_print_tree_inorder_with_items(tree, ofp);
}

tree_name_node *scan_in_trees(FILE* ifp, tree_name_node *upper_tree, int nTrees)
{
    int i = 0;
    char input_name[32];
    //Repeats as many times as there are trees
    for(i=0; i<nTrees; i++)
    {
        //Scans in input_name
        fscanf(ifp, "%s", input_name);
        //If our root is empty, this becomes the root
        if(upper_tree==NULL)
        {
            upper_tree = new_tree_name_node(input_name);
        }
        //If not, we insert it
        else
        {
            tree_name_bst_insert(upper_tree, new_tree_name_node(input_name));
        }
    }
    return upper_tree;
}
tree_name_node *scan_in_items(FILE* ifp, tree_name_node *upper_tree, int nItems)
{
    int i = 0;
    char sTree[32];
    char sItem[32];
    int cItem;

    for(i=0; i<nItems; i++)
    {
        //Scans in sTree, sItem, and cItem
        fscanf(ifp, "%s %s %d", sTree, sItem, &cItem);

        //Find the tree this item is supposed to go in
        tree_name_node *correct_tree = tree_name_bst_find(upper_tree, sTree);

        //Exit if the tree does not exist
        if(correct_tree==NULL)
        {
            //CHECKULTRA: Assume this kind of error will not happen once the code is working, remove before submission
            printf("Error: Could not find parent tree");
        }
        //If it does exist, then if the tree's root is null, insert there
        else if(correct_tree->theTree==NULL)
        {
            correct_tree->theTree = new_item_node(sItem, cItem);
        }
        //If the root is not null, use the insertion method
        else
        {
            item_bst_insert(correct_tree->theTree, new_item_node(sItem, cItem));
        }
    }
    return upper_tree;
}

int main(void)
{
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("cop3502-as4-input.txt", "r"); //Opens cop3502-as4-input.txt for reading
    ofp = fopen("cop3502-as4-output-bermudez-elier.txt", "w"); //Creates/Opens cop3502-as4-output-bermudez-elier.txt for writing

    //Scan in nTrees, nItems, and nCommands
    int nTrees, nItems, nCommands;
    fscanf(ifp, "%d %d %d", &nTrees, &nItems, &nCommands);

    //Setup the top half of trees
    tree_name_node *upper_tree = NULL;
    upper_tree = scan_in_trees(ifp, upper_tree, nTrees);

    //Setup the bottom half of trees
    upper_tree = scan_in_items(ifp, upper_tree, nItems);

    //So far everything works as intended!
    //Next step is to make a proper print statement for how the program wants it, and just test print based on that from now on
    //Then get rid of the old print test statements.
    //Modify item_print_tree_inorder to just print their names in one neat line

    print_first_half_of_output(upper_tree, ofp);

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
