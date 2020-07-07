//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


    int **p,**q;
    int **x,**y;
    int *a,*b,*x1,a1,b1;
    int *m,*n,n1;
int main(){

    /// Note that n needs to be initialized
    n = &n1;
    a = &a1;
    b = &b1;
    x = y = &x1;
    mustPointsTo(x,x1);
    n1 = **x+**y;
    p = q = &a;
    mustPointsTo(p,a);
    n1+=**p;
    if(a){
        p = x;
        q = y;
	mustPointsTo(p,x1);
	mustPointsTo(q,x1);
	n1+=**p+**q;
    }

    *p = n;
    m = *q;
    mayPointsTo(x1,n1);
    mustPointsTo(x1,n1);//fail
    mayPointsTo(a,n1);
    mayPointsTo(a,a1);
    mayPointsTo(m,a1);
    //mayPointsTo(m,x1);
    mustPointsTo(m,a1);
    n1 += **p+**q+*m;

    return n1;

    /*MAYALIAS(p,&a);
    MAYALIAS(p,&x1);
    NOALIAS(m,&a1);
    NOALIAS(m,&x1);
    MUSTALIAS(n,m);*/

}
