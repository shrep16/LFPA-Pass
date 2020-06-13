/*
 * Cycle
 * Author: Sen Ye
 * Date: 11/10/2013
 */
//#include "aliascheck.h"
#include "myHeader.h"
	int **x1, **y1, **z1;
	int *x2, *y2, *z2, *y2_;
	int x3, y3, z3, y3_;
int main() {
	x2 = &x3, y2 = &y3, z2 = &z3;
	x1 = &x2, y1 = &y2, z1 = &z2;
	// if the following branch is commented out,
	// the first alias check will fail while
	// the second one is OK.
	if (y3_) {
		y2_ = &y3_;
		y1 = &y2_;
	}
	
	mustPointsTo(x1,x2);
	mustPointsTo(y1,y2);
	mustPointsTo(z1,z2);
	mustPointsTo(y1,y2_);

	mayPointsTo(y1,y2_);
	mayPointsTo(y1,y2);

	mustPointsTo(*x1, x3);//not live
	mustPointsTo(*y1, y3);

	mayPointsTo(*y1,y3);
	mayPointsTo(*y1,y3_);

	mustPointsTo(*z1, z3);
	*x1 = *y1;
	*y1 = *z1;
	*z1 = *x1;
	// there should be a cycle from
	// y2 -> x2 -> z2 -> y2
	//MAYALIAS(x2, y2);
	mustPointsTo(*x1, y3);
	mustPointsTo(*y1, z3);
	mustPointsTo(*z1, y3);

	mustPointsTo(x2, y3);
	mustPointsTo(y2, z3);
	mustPointsTo(z2, y3);
	

	//will pass if branch is kept live
	mayPointsTo(*x1,y3);
	mayPointsTo(*x1,y3_);
	mayPointsTo(x2,y3);
	mayPointsTo(x2,y3_);

	mayPointsTo(y2, z3);
	mayPointsTo(y2_,z3);
	mayPointsTo(*y1, z3);
	
	mayPointsTo(*z1, y3);
	mayPointsTo(*z1, y3_);
	mayPointsTo(z2, y3);
	mayPointsTo(z2, y3_);

	//MAYALIAS(z2, x2);
	return **x1+**y1+**z1;
}
