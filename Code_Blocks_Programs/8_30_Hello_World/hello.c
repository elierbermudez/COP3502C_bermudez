//Always include these
#include <stdio.h>
#include <stdlib.h>

//This is how you make a structure called poine_struct which is kind of like an object
struct point_struct
{
    int count;
    double xval;
    double yval;
    double zval;
    float alpha;
    char name[20];
};

//This line gives us a nickname for point_struct so we can call it point
typedef struct point_struct point;

//Main method
int main(void)
{
    printf("Hello, world!\n");

    //Declaring a null pointer then allocating an amount of memory to it equal to the sizeof our point structure
    point *p = NULL;
    p = malloc(sizeof(point));

    //For our pointer we are assigning it values for each value of the structure
    p->count = 4;
    p->xval = 3.0;
    p->yval = 5.0;
    p->zval = 8.0;
    p->alpha = 2.0;
    //C handles strings weird. p->name = "Bob"; would not work
    strcpy(p->name, "Elier Bermudez");

    printf("Hello, world! We allocated %d bytes. \n", sizeof(point));
    printf("This is Elier Bermudez!");
    free(p);
}
