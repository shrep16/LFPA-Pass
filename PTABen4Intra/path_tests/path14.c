//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int **p,*q;
    int *a,*b,c,d,e;
int main(){
    q = &c;
    if(a){
        p = &a;
        if(c){
            q = &d;
        }
	mayPointsTo(q,d);
	c = *q;
    }
    else{
        p = &b;
        q = &e;
    }

    *p = q;
    mayPointsTo(a,c);
    mayPointsTo(a,e);
    mayPointsTo(a,d);
    mayPointsTo(b,c);
    mayPointsTo(b,e);
    mayPointsTo(b,d);
    c+= **p+*q;
    return c;
    /*MAYALIAS(a,&c);
    MAYALIAS(a,&d);
    MAYALIAS(b,&e);
    NOALIAS(a,&e);
    NOALIAS(b,&c);
    NOALIAS(b,&d);*/

}
