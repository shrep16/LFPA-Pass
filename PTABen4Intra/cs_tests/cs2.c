//#include "aliascheck.h"
#include "myHeader.h"
int obj;
/*void foo(int **p, int **q, int **r){


	*r = *p;
	*q = &obj;

}*/


	int **a,**b,**c,**d,**e,**f,*x,*y,*z,*w,*k,x1,y1,z1,w1,k1;
int main(){

	x = &x1;
	y = &y1;
	z = &z1;
	w = &w1;
	k = &k1;

	a = &x;
	b = &y;
	c = &z;
	if(a){

		d = &w;
		e = &w;
		f = &k;
	}
	mustPointsTo(*a,x1);
	mustPointsTo(*b,y1);
	mustPointsTo(*c,z1);
	obj = **a+**b+**c;
	//foo(a,b,c);
	*c = *a;
	*b = &obj;
	mustPointsTo(*c,x1);
	mustPointsTo(z,x1);
	mustPointsTo(x,x1);
	mustPointsTo(*b,obj);
	mustPointsTo(*a,x1);
    //MUSTALIAS(x,z);
    //MUSTALIAS(y,&obj);
    	//should fail
	mustPointsTo(*d,w1);
	mustPointsTo(*e,w1);
	mustPointsTo(*f,k1);

	//will pass
	mustPointsTo(w,w1);
	mustPointsTo(k,k1);

	obj += **d+**e+**f;

	//foo(d,e,f);
	*f = *d;
	*e = &obj;

	mayPointsTo(k,k1);
	mayPointsTo(k,w1);
	mayPointsTo(w,w1);
	mayPointsTo(w,obj);

	obj += **d+**e+**f;
	obj +=**a+**b+**c;
	return obj;
    //NOALIAS(w,k);
    //MAYALIAS(w,&obj);
    //NOALIAS(x,w);
    //NOALIAS(z,k);
    //MAYALIAS(y,w);

}
