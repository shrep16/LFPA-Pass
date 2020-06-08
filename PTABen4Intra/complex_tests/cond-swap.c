#include "myHeader.h"
/*void swap(char **a, char **b, int flag) {
   char * c;
   if (flag == 1) {
      c = *a;
      *a = * b;
      *b = c;
   }
}*/

    char a[20];
    char b[20];
    char * p1, *p2;
    char * pa, * pb;

int main (){
    p1 = a;
    p2 = b;

    mustPointsTo(p1,a);
   /* swap (&p1, &p2, 1);
    swap (&p1, &p2, 1);
    swap (&p1, &p2, 1);
    swap (&p1, &p2, 0);*/
    pa = p2;
    pb = p1;
    return *pb;
}


