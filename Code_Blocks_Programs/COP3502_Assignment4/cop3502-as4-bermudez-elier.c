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
int returnMax(int a, int b)
{
    if(a>b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int item_bst_find_depth(item_node *parent, int count)
{
    int leftDeepest = 0;
    int rightDeepest = 0;
    //If the parent never existed, there's an error
    if(parent == NULL)
    {
        return -1;
    }
    //If the parent has no children, return its depth (this is a leaf)
    else if((parent->left==NULL)&&(parent->right==NULL))
    {
        return count;
    }
    //We know it has children so we iterate count here
    count++;

    //If it has left children, go left
    if(parent->left!=NULL)
    {
        leftDeepest = item_bst_find_depth(parent->left, count);
    }
    else
    {
        leftDeepest = 0;
    }
    //If it has right children, go right
    if(parent->right!=NULL)
    {
        rightDeepest = item_bst_find_depth(parent->right, count);
    }
    else
    {
        rightDeepest = 0;
    }
    return returnMax(leftDeepest, rightDeepest);
}

int item_bst_find_count(item_node *parent, int *count)
{
    //If the parent never existed, there's an error
    if(parent == NULL)
    {
        return 0;
    }
    else{
        //If it does exist, add it's count
        *count = *count + parent->count;

        //If it has left children, go left
        if(parent->left!=NULL)
        {
            item_bst_find_count(parent->left, count);
        }

        //If it has right children, go right
        if(parent->right!=NULL){
            item_bst_find_count(parent->right, count);
        }
    }
    return *count;
}

item_node *search_in_name_node(item_node *parent, char* input_name)
{
    return item_bst_find(parent, input_name);
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

void item_print_items_before(item_node *item, FILE *ofp, char* master_item, int *count)
{
    if(item->left)
    {
        *count = *count+1;
        item_print_items_before(item->left, ofp, master_item, count);
    }

    if(strcmp(item->name, master_item)==0)
    {
        fprintf(ofp, "item before %s: %d", master_item, *count);
    }

    if(item->right)
    {
        item_print_items_before(item->right, ofp, master_item, count);
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

tree_name_node *search_for_name_node(tree_name_node *parent, char* input_name)
{
    return tree_name_bst_find(parent, input_name);
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

void print_first_half_of_output(tree_name_node *tree, FILE *ofp)
{
    tree_name_print_tree_inorder(tree, ofp);
    fprintf(ofp, "\n");
    tree_name_print_tree_inorder_with_items(tree, ofp);
    fprintf(ofp, "=====Processing Commands=====\n");
}

void traverse_in_order(tree_name_node *tree, FILE *ofp)
{
    print_first_half_of_output(tree, ofp);
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

        //Exit and print an error statement if the tree does not exist
        if(correct_tree==NULL)
        {
            //This kind of error should not happen if the input is valid
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
void *command_search(FILE *ofp, tree_name_node *upper_tree, char* tempTreeName, char* tempItemName)
{
    /*
    Search for item item in tree tree. Print the count if it’s found, “<item> not found in <tree>”
    if the item doesn’t exist there, or “<tree> does not exist” if the tree doesn’t exist.
    */
    //Find the tree our item is in
    tree_name_node *tempTree = tree_name_bst_find(upper_tree, tempTreeName);
    //If the tree does not exist, print that and exit.
    if(tempTree == NULL)
    {
        fprintf(ofp, "%s does not exist", tempTreeName);
        return 0;
    }
    //Find the item in the tree
    else
    {
        item_node *tempItem = item_bst_find(tempTree->theTree, tempItemName);
        //If the item does not exist, print that and exit
        if(tempItem == NULL)
        {
            fprintf(ofp, "%s not found in %s", tempItemName, tempTreeName);
        }
        //If everything worked, print the count
        else
        {
            fprintf(ofp, "%d %s found in %s", tempItem->count, tempItemName, tempTreeName);
        }
    }
    return 0;
}

void *command_item_before(FILE *ofp, tree_name_node *upper_tree, char* tempTreeName, char* tempItemName)
{
    //Count the number of items lexographically before item item in tree tree.
    //Find the tree our item is in
    tree_name_node *tempTree = tree_name_bst_find(upper_tree, tempTreeName);
    //If the tree does not exist, print that and exit.
    if(tempTree == NULL)
    {
        fprintf(ofp, "%s does not exist", tempTreeName);
        return 0;
    }
    //Find the item in the tree, print the items before it.
    else
    {
        int count = 0;
        item_print_items_before(tempTree->theTree, ofp, tempItemName, &count);
    }
    return 0;
}
int absoluteVal(int a, int b){
    int dif = a-b;
    if(dif<0){
        dif = dif*(-1);
    }
    return dif;
}

void *command_height_balance(FILE *ofp, tree_name_node *upper_tree, char* tempTreeName)
{
    /*
    ▪ Prints the heights of the left and right subtrees of tree tree, and whether or not they’re balanced.
▪ A tree is balanced if the heights of its left and right subtrees differ by at most one.
▪ A tree with only a root has height 0.
▪ A null reference has height -1.
*/
/*animal: left height 1, right height 3, difference 2, not balanced*/

    //Find the tree we are using
    tree_name_node *tempTree = tree_name_bst_find(upper_tree, tempTreeName);
    //If the tree does not exist, print that and exit.
    if(tempTree == NULL)
    {
        fprintf(ofp, "%s does not exist", tempTreeName);
        return 0;
    }
    //Find the left and right depths
    else
    {
        //Find the left depth
        int leftDepth = 0;
        leftDepth = item_bst_find_depth(tempTree->theTree->left, 0);

        //Find the right depth
        int rightDepth = 0;
        rightDepth = item_bst_find_depth(tempTree->theTree->right, 0);

        //Find the difference, print half the message
        int dif = absoluteVal(leftDepth, rightDepth);
        fprintf(ofp, "%s: left height %d, right height %d, difference %d,", tempTreeName, leftDepth, rightDepth, dif);

        //Print not if the difference is too large, finish message.
        if(dif>1){
            fprintf(ofp, " not");
        }
        fprintf(ofp, " balanced");
    }
    return 0;
}

void *command_count(FILE *ofp, tree_name_node *upper_tree, char* tempTreeName)
{
    //int item_bst_find_count(item_node *parent, int *count)
    /*
            int count = 0;
        item_print_items_before(tempTree->theTree, ofp, tempItemName, &count);
        */

    //Find the tree we are using
    tree_name_node *tempTree = tree_name_bst_find(upper_tree, tempTreeName);
    //If the tree does not exist, print that and exit.
    if(tempTree == NULL)
    {
        fprintf(ofp, "%s does not exist", tempTreeName);
        return 0;
    }
    //If it does, find and print the count
    else{
        int count = 0;
        item_bst_find_count(tempTree->theTree, &count);
        //we did the wrong count. This wants total items, not total kinds of animals. Test if this works anyways
        fprintf(ofp, "%s count %d", tempTreeName, count);
    }

    return 0;
}

tree_name_node *command_delete(FILE *ofp, tree_name_node *upper_tree, char* tempTreeName, char* tempItemName)
{
    return upper_tree;
}

tree_name_node *command_delete_tree(FILE *ofp, tree_name_node *upper_tree, char* tempTreeName)
{
    return upper_tree;
}

tree_name_node *scan_in_commands(FILE* ifp, FILE* ofp, tree_name_node *upper_tree, int nCommands)
{
    int i = 0;
    char currCommand[32];
    char tempTreeName[32];
    char tempItemName[32];

    //For each of these, all we want it to do is scan in the information, then pass it to a different function.
    for(i=0; i<nCommands; i++)
    {
        fscanf(ifp, "%s", currCommand);
        if(strcmp(currCommand, "search")==0)
        {
            fscanf(ifp, "%s %s", tempTreeName, tempItemName);
            command_search(ofp, upper_tree, tempTreeName, tempItemName);
        }
        else if(strcmp(currCommand, "item_before")==0)
        {
            fscanf(ifp, "%s %s", tempTreeName, tempItemName);
            command_item_before(ofp, upper_tree, tempTreeName, tempItemName);
        }
        else if(strcmp(currCommand, "height_balance")==0)
        {
            fscanf(ifp, "%s", tempTreeName);
            command_height_balance(ofp, upper_tree, tempTreeName);
        }
        else if(strcmp(currCommand, "count")==0)
        {
            fscanf(ifp, "%s", tempTreeName);
            command_count(ofp, upper_tree, tempTreeName);
        }
        else if(strcmp(currCommand, "delete")==0)
        {
            fscanf(ifp, "%s %s", tempTreeName, tempItemName);
            upper_tree = command_delete(ofp, upper_tree, tempTreeName, tempItemName);
        }
        else if(strcmp(currCommand, "delete_tree")==0)
        {
            fscanf(ifp, "%s", tempTreeName);
            upper_tree = command_delete_tree(ofp, upper_tree, tempTreeName);
        }
        if(i!=(nCommands-1))
        {
            fprintf(ofp, "\n");
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

    //Print the top half
    print_first_half_of_output(upper_tree, ofp);

    //Scan in all commands
    upper_tree = scan_in_commands(ifp, ofp, upper_tree, nCommands);

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
    return 0;
}
