#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int a,b,*p,i,c;

int main(){


if(i)
    p = &a;
else
    p = &b;
    mayPointsTo(p,a);
    mayPointsTo(p,b);
	
    *p = 10;

    c = *p;

    a = 5;

    b = 4;

    *p = 20;

    p = &a;

    mustPointsTo(p,a);
    doesNotPointsTo(p,b);

    *p = 10;
    
    return 0;

}
