/*
 * Context-insensitive.
 * Author: Sen Ye
 * Date: 10/10/2013
 */

//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int global;
int *p_global = &global;
int m;
void foo() {
	p_global = &global;
}

int main() {
	int *p_local;
	p_local = &global;
	mustPointsTo(p_local,global);
	mustPointsTo(p_global, global);
	//foo();
	//MAYALIAS(p_local, p_global);
	return *p_local + *p_global;
}
