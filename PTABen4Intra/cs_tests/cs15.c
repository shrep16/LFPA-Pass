//#include "aliascheck.h"
#include "myHeader.h"
/*void foo(int **w,int**x,int **y, int *z){
	int *t;
	t = *x;
	*y = z;
	*t = w;

}

void bar(int **p1,int**p2,int **p3, int *p4){
	foo(p1,p2,p3,p4);
}*/

	int **a,**b,**c,*d,*a1,*b1,*c1,d1;
int main(){

	a = &a1;
	b = &b1;
	c = &b1;
	d = &d1;
	mustPointsTo(a,a1);
	mustPointsTo(b,b1);
	mustPointsTo(c,b1);
	mustPointsTo(d,d1);
	int **w = a;
	int **x = b;
	int **y = c;
	int *z = d;
	int *t;
	t = *x;
	*y = z;
	*w = t;
	mustPointsTo(a,a1);
	return **w+**x+**y+*z;


	//bar(a,b,c,d);
//	foo(a,b,c,d);
//	foo(b,a,c,d);
}
