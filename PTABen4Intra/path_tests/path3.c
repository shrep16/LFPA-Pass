//#include "aliascheck.h"
#include "myHeader.h"
    int **p,**q;
    int *a,*b;
    int *m,*n;
    int a1,b1,m1;
int main(){


    a = &a1;
    b = &b1;
    m = &m1;
	p = q = &a;

    if(a){
	    p = q = &b;
    }

    *p = m;
	n = *q;
	mayPointsTo(p,a);
	mayPointsTo(p,b);
	mayPointsTo(q,a);
	mayPointsTo(q,b);
	mayPointsTo(a,m1);
	mayPointsTo(b,m1);
	mustPointsTo(n,m1);
	mayPointsTo(n,a1);
	mayPointsTo(n,b1);
	mayPointsTo(n,m1);
	return **p+**q+*n;
    /*MAYALIAS(p,&a);
    MAYALIAS(p,&b);
    NOALIAS(n,&a1);
    NOALIAS(n,&b1);
    MUSTALIAS(n,m);*/
}
