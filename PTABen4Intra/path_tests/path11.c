//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


    int **p,*q;
    int *a,*b,c,d;
//void foo(int**,int* );
int main(){

    if(a){
        p = &a;
        q = &c;
	*p = q;
        //foo(p,q);
    }
    else{
        p = &b;
        q = &d;
	*p = q;
        //foo(p,q);
    }
    mayPointsTo(a,c);
    doesNotPointsTo(a,d);
    mayPointsTo(b,d);
    doesNotPointsTo(b,c);
    d = **p+*q;

    *p = q;
    mayPointsTo(a,c);
    mayPointsTo(a,d);
    mayPointsTo(b,c);
    mayPointsTo(b,d);
    d += **p+*q;

    return d;
  /*  MAYALIAS(a,&c);
    MAYALIAS(b,&d);
    NOALIAS(a,&d);
    NOALIAS(b,&c);*/
}

//void foo(int**x,int *y){
//    *x = y;
//}

