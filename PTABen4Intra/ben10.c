//capture change from may to must pointer analysis for p
#include "myHeader.h"

int a,b,*p,i,c;

int main(){


	if(i) 
		p =&a;
	else
		p = &b;

	mayPointsTo(p,a);
	mayPointsTo(p,b);
	*p = 10;

	mustPointsTo(p,a);
	doesNotPointsTo(p,b);
	c = *p;

	a = 5;

	b = 4;

	*p = 20;

	p = &a;
	mustPointsTo(p,a);
	doesNotPointsTo(p,b);
	*p = 10;
	doesNotPointsTo(p,a);

}
