//capture must points to a
#include "myHeader.h"

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
