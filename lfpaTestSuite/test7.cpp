#include "myHeader.h"
#include<iostream>
#include<set>
using namespace std;
int d,e;
int *p, *a;
bool hi;
set<int *> IN;
set<int > OUT;
int main() {
	IN.insert(a);
	OUT.insert(d);
	while(hi) {
		mustPointsTo(IN,OUT);
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
