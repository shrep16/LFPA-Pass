/*#include "aliascheck.h"
void foo(int **p, int **q, int **r,int *s){

	*r = *p;
	*q = s;

}*/
#include "myHeader.h"


	int **a,**b,**c,**d,**e,**f,*x,*y,*z,*w,*g,*k,x1,y1,z1,w1,k1,g1;
int main(){

	x = &x1;
	y = &y1;
	z = &z1;
	w = &w1;
	g = &g1;
	k = &k1;

	a = &x;
	b = &y;
	c = &z;

	//foo(a,b,c,k);
	*c = *a;
	*b = k;
	mustPointsTo(z,*x);
	mustPointsTo(*b,k1);
    //MUSTALIAS(x,z);
    //MUSTALIAS(y,&k1);
	d = &w;
	e = &w;
	f = &g;
	mustPointsTo(k,k1);
	//foo(d,e,f,k);
	*f = *d;
	*e = k;
	
	mustPointsTo(*f,w1);
	mustPointsTo(*d,k1);
	mustPointsTo(*e,k1);
    /*NOALIAS(w,g);
    MUSTALIAS(w,&k1);
    NOALIAS(x,w);
    NOALIAS(z,g);
    MUSTALIAS(w,y);*/
	return **a+**b+**c+**d+**e+**f;

}
