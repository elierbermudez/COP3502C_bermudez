#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	"leak_detector_c.h"
//Elier Bermudez, COP3502, Assignment 5
void read_in_array(FILE *ifp, int *array, int count)
{
    int i;
    for(i = 0; i<count; i++)
    {
        fscanf(ifp, "%d", &array[i]);
    }
}

void print_array(FILE *ofp, int *array, int count)
{
    int i;
    for( i = 0; i<count; i++)
    {
        fprintf(ofp, "%d ", array[i]);
        //printf("%d ", array[i]);
    }
    fprintf(ofp, "\n");
    //printf("\n");
}

void siftdown(FILE *ofp, int *array, int count, int sifter_index)
{
    int left_index = 2*sifter_index+1;
    int right_index = left_index+1;
    int left_valid = 0;
    int right_valid = 0;
    int temp = 0;
    int chosen_index = -1;

    //Mark the left child as valid if it is greater than its parent and in bounds
    if(array[sifter_index]<array[left_index]    &&  left_index<count)
    {
        left_valid=1;
    }
    //Mark the right child as valid if it is greater than its parent and in bounds
    if(array[sifter_index]<array[right_index]    &&  right_index<count)
    {
        right_valid=1;
    }
    //If neither are valid, do nothing
    if(left_valid==0 && right_valid==0)
    {
        ;
    }
    //If the left is valid, set it as chosen_index
    else if(left_valid==1 && right_valid==0)
    {
        chosen_index = left_index;
    }
    //If the right is valid, set it as chosen_index
    else if(left_valid==0 && right_valid==1)
    {
        chosen_index = right_index;
    }
    //If both are valid, set the greater one as chosen_index
    else if(left_valid==1 && right_valid==1)
    {
        if(array[left_index]<array[right_index])
        {
            chosen_index = right_index;
        }
        else
        {
            chosen_index = left_index;
        }
    }
    //If the chosen_index is valid, swap the parent and child then call siftdown again
    if(chosen_index!=-1)
    {
        temp = array[sifter_index];
        array[sifter_index] = array[chosen_index];
        array[chosen_index] = temp;
        siftdown(ofp, array, count, chosen_index);
    }
}
void heapify(FILE *ofp, int *array, int count)
{
    //Iterate through the array backwards
    int i;
    for( i = count-1; i>(-1); i--)
    {
        //I am only calling siftdown when something should actually be sifted down so my output may look different
        //If a child is greater than it's parent at (n-1)/2, call siftdown on its parent and print the array
        if(array[i]>array[(i-1)/2])
        {
            siftdown(ofp, array, count, (i-1)/2);
            //Output the array after each call to siftdown() from heapify().
            print_array(ofp, array, count);
        }
    }
}

void remove_root(FILE *ofp, int *array, int *count)
{
    //Set root to last valid element
    array[0] = array[*count-1];

    //Delete last valid element
    array[*count-1] = 0;
    *count = *count-1;

    //Perform siftdown on new root
    siftdown(ofp, array, *count, 0);
}

void remove_all(FILE *ofp, int *array, int count)
{
    //Iterate through the array
    int i;
    int original_count = count;
    for( i = 0; i<original_count; i++)
    {
        //Output each integer removed from the heap, on a line by itself.  Print nothing else here.
        fprintf(ofp, "%d\n", array[0]);
        //printf("%d\n", array[0]);

        remove_root(ofp, array, &count);

        //Output the heap array after each integer is removed.
        print_array(ofp, array, count);
    }
}

int main(void)
{
    atexit(report_mem_leak); //This will create a leak_info.txt file which describes any memory leaks your code has
    FILE *ifp;
    FILE *ofp;

    /*
    Opens cop3502-as5-input.txt for reading
    Creates/Opens cop3502-as5-output-bermudez-elier.txt for writing
    */
    ifp = fopen("cop3502-as5-input.txt", "r");
    ofp = fopen("cop3502-as5-output-bermudez-elier.txt", "w");

    //Get number of integers
    int count;
    fscanf(ifp, "%d", &count);

    //Allocate an array with enough space to hold count integers.
    int *array;
    array = (int*)malloc(count * sizeof(int));

    //Read the integers into the array.
    read_in_array(ifp, array, count);

    //Output the initially-read array.
    print_array(ofp, array, count);

    //Heapify the array.
    heapify(ofp, array, count);

    //Now start removing integers
    //Remove the integers from the heap, and print them, in order (descending).
    remove_all(ofp, array, count);

    free(array);

    fclose(ifp);
    fclose(ofp);
}
