/*void swap1(char **a, char **b) {
    char *c;
   c = *a;
   *a = * b;
   *b = c;
}

void swap(char ***a, char ***b) {
    swap1(*a, *b);
}*/
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


    char b;
    char a;
    char * p1, *p2, *t1, *t2;
    char ** pa, ** pb;
int main (){

    p1 = &a;
    p2 = &b;

    pa = & p1;
    pb = & p2;

    mustPointsTo(pa,p1);
    mustPointsTo(pb,p2);
    mustPointsTo(p1,a);
    mustPointsTo(p2,b);

    //swap 
    char *c;
    c = *pa;
    *pa = *pb;
    *pb = c;

    mustPointsTo(pa,p1);
    mustPointsTo(pb,p2);

    mustPointsTo(p1,b);
    mustPointsTo(p2,a);
    //swap (&pa, &pb);
    
    t1 = *pa;
    *t1 = 0;

    mustPointsTo(t1,b);
    t2= *pb;
    *t2 = 0;
    mustPointsTo(t2,a);

    if(pa)
	    return *t1;
    else
	    return *t2;
}


