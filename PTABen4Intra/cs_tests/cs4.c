#include "myHeader.h"
/*#include "aliascheck.h"
void foo(int **x,int**y,int **z, int *w){
	int *t;
	*y = w;
	t = *x;
	*z = t;

}*/

	int *a,*b,*c,d,*p,*q,r,a1;
int main(){

    a = &a1;
    int **x,**y,**z,*w;
    x = &a;
    y = &b;
    z = &c;
    w = &d;

    int *t;
    *y = w;
    t = *x;
    *z = t;

    mustPointsTo(b,d);
    mustPointsTo(c,a1);

//	foo(&a,&b,&c,&d);
    //MUSTALIAS(b,&d);
    //MUSTALIAS(c,&a1);
	//foo(&p,&p,&q,&r);
    x = &p;
    y = &p;
    z = &q;
    w = &r;

    *y = w;
    t = *x;
    *z = t;
    mustPointsTo(p,r);
    mustPointsTo(q,r);

    //MUSTALIAS(p,&r);
    //MUSTALIAS(p,q);
    //NOALIAS(b,p);
    //NOALIAS(c,p);
    return *p+*q+*c+*b;
}
