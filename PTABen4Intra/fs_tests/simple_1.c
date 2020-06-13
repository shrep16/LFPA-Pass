/*
 * Simple program to test flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#include "myHeader.h"
	int *p, *q;
	int x, y;
int main() {
	p = &x;
	q = &y;
	mustPointsTo(p,x);
	mustPointsTo(q,y);
	x += *p+*q;
	//NOALIAS(p, q);
	p = q;
	mustPointsTo(p,y);
	x+= *p;
	//MUSTALIAS(p, q);
	return x;
}
