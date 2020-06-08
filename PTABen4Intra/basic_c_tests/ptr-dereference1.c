/*
 * Simple alias check
 * Author: Sen Ye
 * Date: 06/09/2013
 */
#include "myHeader.h"

	int a,b,*c,*d;
int main()
{

	c = &a;
	d = &a;
	mustPointsTo(c,a);
	mustPointsTo(d,a);
	a = *c + *d;
	mustPointsTo(c,a);
	c = &b;
	// In LLVM, top level variables
	// will be converted into SSA, so
	// different versions should have
	// different values.
	mustPointsTo(c,b);
	mustPointsTo(d,a);
	return *c+*d+a;
}
