//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int obj, t,s;
int *k =&s;
	int **x, **y;
	int *a, *b, *c, *d,*e;
//void foo(int**, int**);
int main(){
    a = &t;
	x=&a; y =&b;
        *x = *y;
        *y = &obj;
	mustPointsTo(a,t);//fail
	mustPointsTo(a,obj);//fail
	mustPointsTo(b,obj);
	t = **x+**y;

	//foo(x,y);
    //NOALIAS(a,b);
    //MUSTALIAS(b,&obj);
	*b = 5;
	c=&t;
	c=&s;
	mustPointsTo(c,s);
	a=c;
	mustPointsTo(a,s);
	t+=*a;
    //MUSTALIAS(a,c);
	if(t) {c=&obj; x =&c; y =&e;}
	else { x= &d; 
		y = &d;
	}
	e = &t;
	mustPointsTo(c,obj);//will fail
	mayPointsTo(c,s);
	mayPointsTo(c,obj);
	t+=*c;
	//foo(x,y);
	*x = *y;
	*y = &obj;
	//mustPointsTo(b,obj);//should pass
	mustPointsTo(*y,obj);//fail
	mustPointsTo(e,obj);//fail
	mustPointsTo(d,obj);//fail
	mayPointsTo(*y,obj);//pass
	mayPointsTo(e,obj);//pass
	mayPointsTo(d,obj);//pass
	return **x+**y+t;



    /*MAYALIAS(c,d);
    MUSTALIAS(d,&obj);
    MAYALIAS(a,c);
    NOALIAS(a,d);
    MAYALIAS(c,d);*/
	//*e = 10;
	
}

void foo(int **p, int **q){
	*p = *q;
	*q = &obj;
}
