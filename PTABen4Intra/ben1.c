#include "myHeader.h"
struct agg{

	int* f;
	int *g;

}agg;

int a1, b1,c1;
int *a, *b,*c;
struct agg temp;

int main(){
	struct agg agg1[100];
	a = &a1;
	b = &b1;
	mustPointsTo(a,a1);
	mustPointsTo(b,b1);
	
	agg1[1].f = a;
	agg1[1].g = b;
	mustPointsTo(a,a1);
	mustPointsTo(b,b1);
	//agg1[0].f = &c;

}
