/*#include "aliascheck.h"
void foo(int **p, int **q){

	*q = *p;

}*/
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


	int **a,**b,**c,**d,**e,**f,*x,*y,*z,*w,*k,x1,y1,z1,w1,k1;

int main(){

	x = &x1;
	y = &y1;
	w = &w1;
	k = &k1;

	a = &x;
	b = &y;
	c = &w;
	d = &k;

	//foo(a,b);
	*b = *a;
       mustPointsTo(x,x1);
	mustPointsTo(y,x1);       
    //MUSTALIAS(x,y);
	//foo(c,d);
	*d = *c;
	mustPointsTo(w,w1);
	mustPointsTo(k,w1);
	doesNotPointsTo(x,w1);
	doesNotPointsTo(y,w1);
	return **a+**b+**c+**d;
    //MUSTALIAS(w,k);
    //NOALIAS(x,k);
    //NOALIAS(y,w);

}
