//#include "aliascheck.h"
#include "myHeader.h"

int **p,**q;
int *x,*y,*z;
int a,b,c;
int main() {
	x=&a;
	y=&b;
	x=&b;
	x=&a;
	z=&b;
	mustPointsTo(x,a);
	mustPointsTo(y,b);
	mustPointsTo(z,b);
	//NOALIAS(x,y);
	//NOALIAS(x,z);
	return *x+*y+*z;
}
