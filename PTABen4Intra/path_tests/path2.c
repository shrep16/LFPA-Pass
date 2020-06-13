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
	p = &a;
	q = &b;

    if(a){
	    p = &b;
	    q = &a;
    }

    *p = m;
	n = *q;
	mustPointsTo(n,m1);
	mayPointsTo(p,a);
	mayPointsTo(p,b);
	mayPointsTo(q,a);
	mayPointsTo(q,b);

	mustPointsTo(a,m1);
	mayPointsTo(a,a1);
	mustPointsTo(b,m1);
	return **p+**q+*n;
    /*MAYALIAS(p,&a);
    MAYALIAS(p,&b);
    MAYALIAS(n,&a1);
    MAYALIAS(n,&b1);
    NOALIAS(p,q);
    NOALIAS(n,&m1);*/
}
