#include "myHeader.h"

int ***z,***x;
int **a, **b, **m;
int *c, *d, *e, *f, *y;
int p;

int main(){
	x = &m;
	if(**m > **a) {
		a =&c;
		b = &e;
		z = &a;
	} else {
		a = &d;
		b = &f;
		z = &b;
	}    
	y = &p;
	doesNotPointsTo(y,p);
	mayPointsTo(y,p);
	mustPointsTo(y,p);
	if(p < 3) 
		**z = y;
	else
		z = x;
	mayPointsTo(c,p);
	mayPointsTo(d,p);
	mayPointsTo(e,p);
	mayPointsTo(f,p);
	mustPointsTo(c,p);
	mustPointsTo(d,p);
	mustPointsTo(e,p);
	mustPointsTo(f,p);
	return (*c + *d +*e + *f);
}
