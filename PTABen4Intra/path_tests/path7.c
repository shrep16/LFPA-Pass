//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

    int **p,*q;
    int *b,*c,d,e;
int main(){


    p = &c;
    q = &e;

    if(d){
        p = &b;
    }
    else{
        q = &d;
    }


    *p = q;
    mayPointsTo(b,e);
    mayPointsTo(b,d);
    mayPointsTo(c,e);
    mayPointsTo(c,d);
    return **p+*q;

    /*MAYALIAS(b,&e);
    MAYALIAS(c,&d);
    NOALIAS(c,&e);
    NOALIAS(b,&d);*/
}
