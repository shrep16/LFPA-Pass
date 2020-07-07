#define TYPE1 int *
#define TYPE2 int
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

#define TYPE1 int ***
#define TYPE2 int **
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

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
	mustPointsTo(z,a);
	mayPointsTo(z,b);
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
//int n;
//	n = *c;
//doesNotPointsTo(c,p);
//return n;
	return (*c + *d +*e + *f);
}
