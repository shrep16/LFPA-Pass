//capture may points To of a
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int **p, *q,*a,b,c;
int main(){

	//p = q;
	p = &a;
	a = &c;
	mustPointsTo(p,a);
	mustPointsTo(a,c);
	if(q)
	q = &b;
	mustPointsTo(q,b);
	mayPointsTo(q,b);
	if(a)
	*p = q;
	mustPointsTo(a,c);
	mustPointsTo(a,b);
	mayPointsTo(a,b);
	mayPointsTo(a,c);
	//p = q;
	return *a;
	//printf("%d,%d %d %d",*p,*q,a,b);

}
