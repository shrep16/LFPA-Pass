//capture must points to a
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int *a,*b;
int p;
int main(){

	b = &p;

	mustPointsTo(b,p);
	if(10)
	a = b;
	mustPointsTo(a,p);
	doesNotPointsTo(b,p);

	return *a;
	//printf("%d,%d",a,b);

}
