#include "myHeader.h"
#include<iostream>
#include<set>
using namespace std;
int a,b;
int **a2, *a1, *b1;
int ***b2;
int main() {
	
	//a2 = &b1;
	/*a1 = &a;
	mustPointsTo(a1,a);
	b = *a1;
	a1 = b1;
	doesNotPointsTo(a1,a);
	b += *a1;*/
	//b = **a2;
	a1 = &a;
	a1 = **b2;
	doesNotPointsTo(a1,b1);
	b += *a1;
	
	
	return b;
}
