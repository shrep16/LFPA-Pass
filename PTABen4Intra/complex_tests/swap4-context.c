  
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

    //char b[20];
    //char a[20];
 

/*void swap(char **a, char **b) {
   char * c;
   c = *a;
   *a = * b;
   *b = c;
}

void f(char **a, char **b) {
  swap(a, b);
}

void g(char **a, char **b) {
   swap(a, b);
   swap(a, b);
}

void h(char **a, char **b) {
   f(a, b);
   f(a, b);
   g(a, b);
}*/


//capture the must points to after swap
    char * p1, *p2;
    char * pa, * pb;
    char a,b;
int main (){

    char *c;
    p1 = &a;
    p2 = &b;
    
    mustPointsTo(p1,a);
    mustPointsTo(p2,b);
    //swap
    c = p1;
    p1 = p2;
    p2 = c;
    mustPointsTo(p2,a);
    mustPointsTo(p1,b);

    
    
    pa = p2;
    pb = p1;
    mustPointsTo(pa,a);
    mustPointsTo(pb,b);
    if(a)
	    return *pa;
    else
	    return *pb;
}


