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
	if(p < 3) 
		**z = y;
	else
		z = x;
	return (*c + *d +*e + *f);
}
