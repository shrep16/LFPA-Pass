/*
 *Author- Prerona Chaudhuri
 *capture may points to of p
 */

#define TYPE1 int *
#define TYPE2 int
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

#define TYPE1 int **
#define TYPE2 int *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int d,e;
int *p;
bool hi;
int *a;
void func(int e) {
	e = 10;
}
int main() {

	while(*p) {
		mustPointsTo(a,d);
		mayPointsTo(a,d);
		p = a;
		mustPointsTo(p,d);
		mayPointsTo(p,d);
		e = *p+1 ;
		a = &d;
		mustPointsTo(a,d);
		mayPointsTo(a,d);
		func(e);
	}
	return e;
}
