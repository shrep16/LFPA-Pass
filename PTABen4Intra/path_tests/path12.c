//#include "aliascheck.h"
#include "myHeader.h"

int obj, t,s;
//void foo(int**, int**);

	int **x, **y;
	int *a, *b, *c, *d,*e;
int main(){
	e = &t; d = &obj;
	c = &s;
	if(t) { x =&c; y =&e;}
	else { x= &d; y = &d;}
	
	*x = *y;
	mayPointsTo(d,t);
	mayPointsTo(d,obj);
	mayPointsTo(c,s);
	mayPointsTo(c,t);
	mayPointsTo(c,obj);
	mustPointsTo(e,t);
	mustPointsTo(*y,t);
	return **x+**y;
    //foo(x,y);
    //NOALIAS(c,&obj);
    //NOALIAS(d,&t);
	
}

void foo(int **p, int **q){
	*p = *q;
}
