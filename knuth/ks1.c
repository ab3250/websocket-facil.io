#include <stdio.h>
#include <math.h>
#include <stdlib.h>
 
 
static int rand_int(int n) {
    int limit = RAND_MAX - RAND_MAX % n;
    int rnd;
 
    do {
        rnd = rand();
    } 
    while (rnd >= limit);
    return rnd % n;
}
 
void shuffle(int *array, int n) {
    int i, j, tmp;
 
    for (i = n - 1; i > 0; i--) {
        j = rand_int(i + 1);
        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
   }
}
int main(void)
{
 
    int i = 0;
    int numbers[50];
    for (i = 0; i < 50; i++)
        numbers[i]= i;
    shuffle(numbers, 50);
    printf("\nArray after shuffling is: \n");
    for ( i = 0; i < 50; i++)
        printf("%d\n", numbers[i]);
    return 0;
}


  int i,mark=0,temp;
  while(mark<lengthArray-1)
  {
    i=rand() % (lengthArray-mark) + mark;
    if(i>29) return 0;
    temp=arr[mark];
    arr[mark]=arr[i];
    arr[i]=temp;
    mark++;
  }