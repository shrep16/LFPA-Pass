#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int *p, *q, c,d;

int main(){




	p  =&c;
	q = &d;
	mustPointsTo(p,c);
	mustPointsTo(q,d);
	p = q;

	doesNotPointsTo(p,c);
	mustPointsTo(p,d);

	*p = 100;
	*q = 100;
}
