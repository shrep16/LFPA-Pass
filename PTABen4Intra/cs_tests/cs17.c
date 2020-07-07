/*#include "aliascheck.h"
void bar(int**k, int**s){

	*k = *s;

}

void foo(int**x,int**y,int**z){
	int t;
	*y = &t;
	*z = *x;
}*/
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


	int *p1,*q1,*r1,*a1,*b1,*c1,q2,a2;
	int **p = &p1;
	int **q = &q1;
	int **r = &r1;
	int **a = &a1;
	int **b = &b1;
	int **c = &c1;
int main(){
    q1 = &q2;
    a1 = &a2;
    int ***k = &p;
   int ***s = &q;
  	*k = *s;
       mustPointsTo(p,q1);
       mustPointsTo(q,q1);
	doesNotPointsTo(p,p1); 
  a2 += ***k+***s;	
	//bar(&p,&q);
    //MUSTALIAS(p,q);
    //NOALIAS(p,&p1);
	//foo(p,q,r);
	int **x,**y,**z;
	int t;
	x = p;
	y = q;
	z = r;
	*y = &t;
	*z = *x;
	mustPointsTo(q1,t);
	mustPointsTo(r1,t);
a2+=**x+**y+**z;

    //MUSTALIAS(q1,r1);
	//foo(a,b,c);
	x = a;
	y = b;
	z = c;
	*y = &t;
	*z = *x;
	mustPointsTo(c1,a2);
	mustPointsTo(a1,a2);
	a2 += **x+**y+**z;
	return a2;
    //MUSTALIAS(a1,c1);
    //NOALIAS(q1,c1);
    //NOALIAS(a1,r1);

}
