//#include "aliascheck.h"
#include "myHeader.h"
int obj, t,s;
//void foo(int**, int**);
	int **x, **y;
	int *a, *b, *c, *d,*e;
int main(){
	x=&a; y =&b;
       *y = &obj;	
	//foo(x,y);
	mustPointsTo(b,obj);
	mustPointsTo(*y,obj);//y is not live
    //MUSTALIAS(b,&obj);
	*b = 5;
	c=&s;
	mustPointsTo(c,s);
	//a=c;
	if(t) { x =&c; y =&e;}
	else { x= &d; y = &d;}
	*y = &obj;
	mayPointsTo(y,b);//fail
	mustPointsTo(e,obj);
	mustPointsTo(d,obj);
	mustPointsTo(*x,s);//fail
	mayPointsTo(d,s);//fail
	mayPointsTo(*x,s);
	//foo(x,y);
    //MAYALIAS(e,d);
    //MAYALIAS(e,&obj);
	*e = 10;
	return **x+**y;
	
}

/*void foo(int **p, int **q){
	*q = &obj;
}*/
