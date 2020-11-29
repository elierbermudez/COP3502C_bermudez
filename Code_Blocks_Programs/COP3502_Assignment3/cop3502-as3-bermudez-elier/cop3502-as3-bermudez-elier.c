// all-monster-sorts.c - Sort monsters by name and weight.

//Elier Bermudez, COP3502 Fall 2020, Assignment3
/*
Note:
I'm really not sure if the sorts will work for ties or not since I didn't know how to make my own test cases for it.
If it doesn't work for test cases what I would have to change is that whenever I call compare_monsters I would need
to fiddle with whether I do >0, >-1, <1, or <0
*/
/* The idea of sorting is simple: take unordered objects, and arrange them in an
   order.  It has a lot of uses, so there's been a lot of work done with it.  Here,
   we're going to demonstrate a few of the simpler, more classic sorting techniques.
   */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

/* Monster structure and helper functions - DO NOT MODIFY THESE. */

typedef struct monster
{
    int id;
    char name[64];
    char element[64];
    int population;
    double weight;
} monster;

monster *make_some_monsters(int n)
{
    monster *monsters = malloc(sizeof(monster) * n);

    time_t t;

    srand((unsigned) time(&t));

    for(int i = 0; i < n; i++)
    {
        monsters[i].id = i;
        sprintf(monsters[i].name, "Monster #%d", rand());
        sprintf(monsters[i].element, "Element #%d", rand());
        monsters[i].population = rand();
        monsters[i].weight = 500.0 * ((double) rand() / (double) RAND_MAX);
    }

    return monsters;
}

void output_monster_list(monster *list, int n, char *title)
{
    printf("List %s:\n", title);
    for(int i = 0; i < n; i++)
    {
        printf("  Monster %d: %s %s %d %lf\n", i, list[i].name, list[i].element, list[i].population, list[i].weight);
    }
    printf("\n");
}

void print_clocks(clock_t clocks)
{
    printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}

void swap_monsters(monster *list, int i, int j)
{
    monster temp;

    memcpy(&temp, list + i, sizeof(monster));
    memcpy(list + i, list + j, sizeof(monster));
    memcpy(list + j, &temp, sizeof(monster));
}

//Custom function to compare ints like strcmp
int intcmp(int a, int b)
{
    if(a<b)
    {
        return -1;
    }
    else if(a>b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* The core comparison function. */

int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
    if(use_name!=0)
    {
        //Compare monsters based on name
        return strcmp(m1->name, m2->name);
    }
    //In class he said to return a negative number if monster 1 is greater than monster 2 but this
    //goes against how strcmp works which is what he said to base it on so this might need
    //to be reversed but if anything it would result in every list being backwards I believe
    else if(use_weight!=0)
    {
        return intcmp(m1->weight, m2->weight);
    }
    return 0;
}

void check_monster_sort(monster *list, int n, int use_name, int use_weight)
{
    for(int i = 1; i < n; i++)
    {
        if(compare_monsters(list + i - 1, list + i, use_name, use_weight) > 0)
        {
            printf("*** The list is NOT sorted.\n\n");
            return;
        }
    }
    printf("The list is sorted.\n\n");
}

/* Implement ascending quick sort. */

int repartition(monster *list, int low_index, int high_index, int *comparisons, int *swaps,
                int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
    //Built based on code from sorts.c

    /* Fill the lower elements of the array with the monsters "less than" the pivot monster,
        with pivot monster being the monster at high_index, and let the number of them we
        encounter determine our eventual pivot index. */

    int i = low_index;
    for(int j = low_index; j < high_index; j++)
    {
        (*comparisons)++;
        if(compare_monsters(list + j, list+high_index, use_name, use_weight)<1)
        {
            (*swaps)++;
            //Had to change this line
            swap_monsters(list, i, j);
            i++;
        }
    }

    /* We've now placed everything below pivot_value in list[low_index..i-1] - and that
       means we can just put our pivot value in list[i]! */

    //This line might have been wrong? Should be (*swaps)++ instead of swaps++ I think.
    (*swaps)++;
    //Had to change this line
    swap_monsters(list, i, high_index);
    return i;
}

/* Recursive function for quick sort. */

void quick_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *swaps,
                          int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
    //Built based on code from sorts.c
    int pivot_index = repartition(list, low_index, high_index, comparisons, swaps, use_name, use_weight);
    // Coming out of repartition, our pivot_index must already be in the right position.
    if(pivot_index-1 > low_index) quick_sort_recursive(list, low_index, pivot_index-1, comparisons, swaps, use_name, use_weight);
    if(high_index > pivot_index+1) quick_sort_recursive(list, pivot_index+1, high_index, comparisons, swaps, use_name, use_weight);

}

/* Shell function for quick sort. */

void quick_sort(monster *list, int n, int use_name, int use_weight)
{
    int comparisons = 0;
    int swaps = 0;
    clock_t start_cpu, end_cpu;

    printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

    start_cpu = clock();
    quick_sort_recursive(list, 0, n-1, &comparisons, &swaps, use_name, use_weight);
    end_cpu = clock();

    printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
    print_clocks(end_cpu - start_cpu);
}

/* Implement ascending bubble sort. */

void bubble_sort(monster *list, int n, int use_name, int use_weight)
{
    int i;
    int j;
    int temp;
    int comparisons = 0;
    int swaps = 0;
    clock_t start_cpu, end_cpu;

    printf("Bubble sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

    start_cpu = clock();

    //Based on bubble sort from sorts.c with small updates to make it work for monsters
    for(i = n-1; i >= 0; i--)
    {
        for(j = 0; j < i; j++)
        {
            comparisons++;
            //Had to change this line
            if(compare_monsters(list + j, list + j + 1, use_name, use_weight)>0) // Are our elements out of order?
            {
                swaps++;
                //Had to change this to swap_monsters instead of swap_integers
                swap_monsters(list, j, j+1);
            }
        }
    }
    end_cpu = clock();
    printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
    print_clocks(end_cpu - start_cpu);
}

/* Highest-value finder for selection sort. */

int find_highest(monster *list, int n, int *comparisons, int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
    //Built from code from sorts.c
    monster *highest_mon = list+0;
    //highest_loc means highest_location
    int highest_loc = 0;
    int i;

    for(i = 0; i <= n; i++)
    {
        (*comparisons)++;
        if(compare_monsters(list + i, highest_mon, use_name, use_weight)>0)
        {
            highest_loc = i;
            highest_mon = list+i;
        }
    }

    //free(highest_mon);
    return highest_loc;
}

/* Implement ascending selection sort. */

void selection_sort(monster *list, int n, int use_name, int use_weight)
{
    int i;
    int highest;
    int comparisons = 0;
    int swaps = 0;
    clock_t start_cpu, end_cpu;

    printf("Selection sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
    start_cpu = clock();

    // YOUR CODE GOES HERE.
    //Built from code from sorts.c
    for(i = n-1; i > 0; i--)
    {
        highest = find_highest(list, i, &comparisons, use_name, use_weight);
        if(highest != i)
        {
            swaps++;
            //Had to change this line
            swap_monsters(list, highest, i);
        }
    }
    end_cpu = clock();
    printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
    print_clocks(end_cpu - start_cpu);
}

/* Find position for insertion sort. */

int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int *comparisons, int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
    //I just didn't use a function to find_position
    //Here's a return statement to make the compiler shut up
    return 0;
}

/* Implement insertion sort. */

void insertion_sort_internal(monster *list, int n, int *comparisons, int *copies, int *block_copies, int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
    //If your list is only 1 element, exit this function immediately.
    if(n==1)
    {
        return;
    }
    else
    {
        //Step 1: Set i = 1
        int i = 1;
        int j = 0;
        for(i=1; i<n; i++)
        {
            //"Grab" the ith element of our list and make a copy of it
            //We have to ACTUALLY make a copy of it, don't just grab a pointer to it.
            monster monster_copy;

            memcpy(&monster_copy, &list[i], sizeof(list[i]));

            //Increase copies
            (*copies)++;
            //Iterate j from the front of the list, until you either reach i or find
            //an element with a value higher than element i. You are using j to look through your already-sorted list.
            for(j=0; j<i; j++)
            {
                //Iterate comparisons
                (*comparisons)++;
                //If our new element is greater than the element we are comparing it to, do nothing.
                if(compare_monsters(list + i, list + j, use_name, use_weight)>0)
                {
                    ;
                }
                else
                {
                    //Element j is the one that our copy is less than, so we want to take the entries starting at j and move them forward by 1
                    memmove(&list[j+1], &list[j], (i-j)*sizeof(list[j+1]));

                    //Iterate block_copies
                    (*block_copies)++;

                    //Iterate copies. i-j is how many things memmove just copied over
                    *copies = *copies + i-j;

                    //Put the copied element in the now "empty" space
                    list[j] = monster_copy;
                }
            }
        }
    }
}

/* Shell for insertion sort. */

void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
    int comparisons = 0;
    int block_copies = 0;
    int total_copies = 0;
    clock_t start_cpu, end_cpu;
    printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
    start_cpu = clock();

    insertion_sort_internal(list, n, &comparisons, &total_copies, &block_copies, use_name, use_weight);

    end_cpu = clock();
    printf("Sort complete with %d comparisons and %d block copies (%d total copies).\n", comparisons, block_copies, total_copies);
    print_clocks(end_cpu - start_cpu);
    return;
}

/* Merge function for merge sort.  Merges sub-arrays [l1..h1] and [l2..h2].
   Must have l2 = l1 + 1 or bad things will happen. */

void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2,
                      int *comparisons, int *copies, int *block_copies, int *mallocs,
                      int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
}

/* Recursive function for merge sort. */

void merge_sort_recursive(monster *list, int low_index, int high_index,
                          int *comparisons, int *copies, int *block_copies, int *mallocs,
                          int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
}

/* Implement merge sort. */

void merge_sort(monster *list, int n, int use_name, int use_weight)
{
    int comparisons = 0;
    int copies = 0;
    int block_copies = 0;
    int mallocs = 0;
    clock_t start_cpu, end_cpu;

    printf("Merge sort %d monsters...\n", n);

    start_cpu = clock();
    merge_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs,
                         use_name, use_weight);
    end_cpu = clock();

    printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
    print_clocks(end_cpu - start_cpu);
}

/* Recursive function for merge-insertion sort. */

void merge_insertion_sort_recursive(monster *list, int low_index, int high_index,
                                    int *comparisons, int *copies, int *block_copies, int *mallocs,
                                    int use_name, int use_weight)
{
    // YOUR CODE GOES HERE.
}

/* Implement merge sort. */

void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
    int comparisons = 0;
    int copies = 0;
    int block_copies = 0;
    int mallocs = 0;
    clock_t start_cpu, end_cpu;

    printf("Merge-insertion sort %d monsters...\n", n);

    start_cpu = clock();
    merge_insertion_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs,
                                   use_name, use_weight);
    end_cpu = clock();

    printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
    print_clocks(end_cpu - start_cpu);
}

/* Main program. */

void run_all_sorts(int n, int only_fast, int use_name, int use_weight)
{
    monster *our_list = make_some_monsters(n);
    monster *our_unsorted_list = malloc(sizeof(monster) * n);

    printf("SORT SET: n = %d, %s, by %s\n\n", n, only_fast ? "fast sorts only" : "all sorts", use_name ? "name" : "weight");

    if(only_fast == 0)
    {
        memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
        bubble_sort(our_unsorted_list, n, use_name, use_weight);
        check_monster_sort(our_unsorted_list, n, use_name, use_weight);

        memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
        selection_sort(our_unsorted_list, n, use_name, use_weight);
        check_monster_sort(our_unsorted_list, n, use_name, use_weight);

        memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
        insertion_sort(our_unsorted_list, n, use_name, use_weight);
        check_monster_sort(our_unsorted_list, n, use_name, use_weight);
    }

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    quick_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    merge_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    merge_insertion_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    printf("SORT SET COMPLETE\n\n");

    free(our_list);
    free(our_unsorted_list);
}

void print_monster_list(monster *list, int n)
{
    printf("List:\n");
    int i = 0;
    for(i = 0; i < n; i++)
    {
        printf("  Value %3d: %s %10f\n", i, list[i].name, list[i].weight);
    }
    printf("\n");
}

//Test function used while programming to test each sort by itself before moving on
void run_test_sorts(int n, int only_fast, int use_name, int use_weight)
{
    monster *our_list = make_some_monsters(n);
    monster *our_unsorted_list = malloc(sizeof(monster) * n);

    printf("SORT SET: n = %d, %s, by %s\n\n", n, only_fast ? "fast sorts only" : "all sorts", use_name ? "name" : "weight");
    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    print_monster_list(our_unsorted_list,n);
    //bubble_sort(our_unsorted_list, n, use_name, use_weight);
    //selection_sort(our_unsorted_list, n, use_name, use_weight);
    //quick_sort(our_unsorted_list, n, use_name, use_weight);
    insertion_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);
    print_monster_list(our_unsorted_list, n);


    printf("SORT SET COMPLETE\n\n");

    free(our_list);
    free(our_unsorted_list);
}

int main(void)
{
    run_all_sorts(50, 0, 0, 1);
    run_all_sorts(50, 0, 1, 0);
    run_all_sorts(500, 0, 0, 1);
    run_all_sorts(500, 0, 1, 0);
    run_all_sorts(5000, 0, 0, 1);
    run_all_sorts(5000, 0, 1, 0);
    run_all_sorts(50000, 1, 0, 1);
    run_all_sorts(50000, 1, 1, 0);
    run_all_sorts(500000, 1, 0, 1);
    run_all_sorts(500000, 1, 1, 0);
//run_test_sorts(10, 0, 0, 1);
}

/*
Here's my merge sort for Integers which works for up to n=3
//Custom function to compare ints like strcmp
int intcmp(int a, int b)
{
    if(a<b)
    {
        return (-1);
    }
    else if(a>b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void merge_sort_merge(int *list, int l1, int h1, int l2, int h2,
                      int *comparisons, int *copies, int *block_copies, int *mallocs)
{
    //Create a temporary list large enough to hold all the elements in both lists
    //Find how many elements are in both lists
    int total_elements = h2 - l1 +1;
    int *our_temporary_list = malloc(sizeof(int) * total_elements);
    (*mallocs)++;

    //Set a pointer to the first element in both lists, and to the first space in the temporary list.
    int *current_element_list_1 = list+l1;
    int *current_element_list_2 = list+l2;
    int *current_element_temp_list = our_temporary_list;

    //Keep track of what index we are on for list 1, and list 2
    int list_1_index = l1;
    int list_2_index = l2;

    //While there are still elements left in one list or the other
    while(list_1_index!=l2  ||  list_2_index!=(h2+1)){
        //If list 1 is empty,
        if(list_1_index==l2){
            //Copy list 2's entry into the the temporary list
            memcpy(current_element_temp_list, current_element_list_2, sizeof(int));

            //Increment temporary list pointer
            current_element_temp_list++;

            //Increment list pointer for list 2
            current_element_list_2++;

            //Increment my custom list_2_index variable
            list_2_index++;
        }
        //If list 2 is empty
        else if(list_2_index == (h2+1)){
            //Copy list 1's entry into the the temporary list
            memcpy(current_element_temp_list, current_element_list_1, sizeof(int));

            //Increment temporary list pointer
            current_element_temp_list++;

            //Increment list pointer for list 1
            current_element_list_1++;

            //Increment my custom list_1_index variable
            list_1_index++;
        }
        //If a < b, intcmp returns -1
        //If the thing we're looking at in list 1 is less than list 2
        //BIG NOTE: compare_monsters does not work the same intcmp, be careful whether you dereference this or not in the main code!!!!!!!!!!!!!!!!!
        else if(intcmp(current_element_list_1[0], current_element_list_2[0]) < 0){
            //This just makes it point to the same spot! Don't do this.
            //current_element_temp_list = current_element_list_1;

            //Copy list 1's entry into the the temporary list
            memcpy(current_element_temp_list, current_element_list_1, sizeof(int));

            //Increment temporary list pointer
            current_element_temp_list++;

            //Increment list pointer for list 1
            current_element_list_1++;

            //Increment my custom list_1_index variable
            list_1_index++;
        }
        //The thing we're looking for in list 2 is less than list 2 or equal
        else{
            //Copy list 2's entry into the the temporary list
            memcpy(current_element_temp_list, current_element_list_2, sizeof(int));

            //Increment temporary list pointer
            current_element_temp_list++;

            //Increment list pointer for list 2
            current_element_list_2++;

            //Increment my custom list_2_index variable
            list_2_index++;
        }
        (*copies)++;
        (*comparisons)++;
    }
    //Copy the temporary list back into the lists you were merging, writing over both.
    int i = 0;
    for(i=l1; i<h2+1; i++){
            //Copy temporary list's entry into the master list
            memcpy(list+i, our_temporary_list+i, sizeof(int));
    }
    //It's not this simple to free up all the memory for monsters but it's better than nothing
    free(our_temporary_list);
}

void merge_sort_recursive(int *list, int low_index, int high_index,
                          int *comparisons, int *copies, int *block_copies, int *mallocs)
{
    int number_of_elements = (high_index - low_index)+1;
    if(number_of_elements == 1){
        return;
    }
    else{
        //The high index of the bottom list is always going to be the average of the indices rounded down.
        int high_index_of_bottom_list = (low_index + high_index)/2;
        merge_sort_recursive(list, low_index, high_index_of_bottom_list, comparisons, copies, block_copies, mallocs);
        merge_sort_recursive(list, high_index_of_bottom_list+1, high_index, comparisons, copies, block_copies, mallocs);
        merge_sort_merge(list, low_index, high_index_of_bottom_list, high_index_of_bottom_list+1, high_index, comparisons, copies, block_copies, mallocs);
    }
}

void merge_insertion_sort_recursive(int *list, int low_index, int high_index,
                                    int *comparisons, int *copies, int *block_copies, int *mallocs)
{
    //int number_of_elements = (high_index - low_index)+1;
}
*/
