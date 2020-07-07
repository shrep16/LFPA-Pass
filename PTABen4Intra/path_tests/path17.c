//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int **p,*q;
    int *x;
    int *a,*b,c,d;
int main(){


    p = &a;
    if(p){
        if(c){
            q = &c;
        }
    }
    else{
        if(c)
            p = &b;
        q = &d;
    }
    if(p){
        *p = q;
    }

    mayPointsTo(a,d);
    mayPointsTo(a,c);
    mayPointsTo(b,c);
    mayPointsTo(b,d);
    //mayPointsTo(b,?);
    return **p+*q;
    /*MAYALIAS(a,&d);
    MAYALIAS(a,&c);
    MAYALIAS(b,&d);
    NOALIAS(b,&c);*/


}
