/*
 *Author- Prerona Chaudhuri
 *capture may points to of p
 */
#include "myHeader.h"
int d,e;
int *p, *a;
bool hi;
int main() {
	while(*p) {
		mustPointsTo(a,d);
		mayPointsTo(a,d);
		p = a;
		mustPointsTo(p,d);
		mayPointsTo(p,d);
		e = *p + 1;
		a = &d;
		mustPointsTo(a,d);
		mayPointsTo(a,d);
	}
	return e;
}
