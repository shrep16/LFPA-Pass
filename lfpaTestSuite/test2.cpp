#define TYPE1 int *
#define TYPE2 int
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int **y, *v, **z, *u, *x, w, b,a,c,p;


int main(){
    b = **z;
    x = u;
    *z = x;
    v = &a;
    mustPointsTo(v,a);    
	if(z == &x)
    return *v;
	else
		return 0;
}
